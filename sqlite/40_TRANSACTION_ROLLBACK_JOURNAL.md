Two ways of achieving atomic transaction:
1. rollback mode
2. WAL - write ahead log

# Assumptions & Foundation
## `sector`
Disk is written in chunks which we call a `sector`.
- It is not possible to modify any part of the disk smaller than a sector.
- To change a part of the disk smaller than a sector, you have to **read in the full sector** that contains the part you want to change, make the change, then **write back out the complete sector**.
- SQLite is only concerned with the **minimum write amount** and so for the purposes of this article, when we say `sector` we mean the **minimum amount** of data that can be written to mass storage in a single go.
- usually size `512byte`, although recently there is push to `4k bytes`
- SQLite has traditionally **assumed that a sector write is not atomic**.
  - SQLite 3.5.0 introduced an VFS interface, there is a new method `xDeviceCharacteristics`
  - which interrogates the underlying filesystem and indicate that sector writes are atomic, and if it does so indicate, SQLite will try to take advantage of that fact.
  - But the default `xDeviceCharacteristics` method for both Unix and Windows does not indicate atomic sector writes and so these optimizations are normally omitted.
- However, SQLite does always assume that a sector write is **linear**.
  - By **"linear"** we (SQLite) mean that SQLite assumes that when writing a `sector`, the hardware begins at one end of the data and writes byte by byte until it gets to the other end. We do not know if this assumption is always true but it seems **reasonable**.

## `fsync`
- SQLite further assumes that write operations will be reordered by the operating system.
  - For this reason, SQLite does a "`flush`" or "`fsync`" operation at key points.
  - SQLite assumes that the `flush` or `fsync` will not return until all pending write operations for the file that is being flushed have completed.
    - We are told that the `flush` and `fsync` primitives are **broken** on some versions of Windows and Linux. This is unfortunate. It opens SQLite up to the possibility of database corruption following a power loss in the middle of a commit. However, there is nothing that SQLite can do to test for or remedy the situation.
  - SQLite assumes that the operating system that it is running on works as advertised.
    - If that is not quite the case, well then hopefully you will not lose power too often.

## File Content Change
- SQLite assumes that when a file grows in length that the new file space originally contains garbage and then later is filled in with the data actually written.
  - In other words, SQLite assumes that the **file size is updated before the file content**.
  - This is a pessimistic assumption and SQLite has to do some extra work to make sure that it does not cause database corruption if power is lost between the time when the file size is increased and when the new content is written.
- The `xDeviceCharacteristics` method of the **VFS** *might* indicate that the filesystem will always write the data before updating the file size.
  - (This is the `SQLITE_IOCAP_SAFE_APPEND` property for those readers who are looking at the code.)
  - When the `xDeviceCharacteristics` method indicates that files content is written before the file size is increased, SQLite can forego some of its pedantic database protection steps and thereby decrease the amount of disk I/O needed to perform a commit.
    - The current implementation, however, makes *no such assumptions* for the default VFSes for Windows and Unix.

## File Deletion
- SQLite assumes that a file deletion is atomic from the point of view of a user process.
  - By this we mean that if SQLite requests that a file be deleted and the power is lost during the delete operation, once power is restored either the file will exist completely with all if its original content unaltered, or else the file will not be seen in the filesystem at all.
  - If after power is restored the file is only partially deleted, if some of its data has been altered or erased, or the file has been truncated but not completely removed, then database corruption will likely result.

## Checksum
- SQLite assumes that the detection and/or correction of bit errors caused by cosmic rays, thermal noise, quantum fluctuations, device driver bugs, or other mechanisms, is the responsibility of the underlying hardware and operating system.
- SQLite **does not add any redundancy** to the database file for the purpose of detecting corruption or I/O errors.
- SQLite assumes that the data it reads is exactly the same data that it previously wrote.

## Powersafe Overwrite
- When an application writes a range of bytes in a file, no bytes outside of that range will change, even if the write occurs just before a crash or power failure.
- This is nothing about the state of the bytes that were written.
  - Those bytes might contain their old values, their new values, random values, or some combination of these.
- The powersafe overwrite property merely states that writes cannot change bytes outside of the range of bytes written.
  - In other words, powersafe overwrite means that there is no "collateral damage" when a power loss occurs while writing. Only those bytes actually being written might be damaged.

# File Locks
Notice that the shared lock is on the operating system **disk cache**, *not on the disk itself*. File locks really are just **flags** within the operating system kernel, usually.
- Hence, the lock will instantly vanish if the operating system crashes or if there is a power loss. It is usually also the case that the lock will vanish if the process that created the lock exits.

- **Shared Lock**
  - allows two or more database connections to read from the database file at the same time.
  - also prevents another database connection from writing to the database file while we are reading it.
  - there can be multiple shared locks on the database file at the same time.
- **Reserved Lock**
  - having similar attributes to shared lock
  - but there is **only one** reserved lock on the database file
  - The idea is that it signals that a process *intends* to modify the database file in the near future but has not yet started to make the modifications.
  - And because the modifications have not yet started, other processes can continue to read from the database. However, no other process should also begin trying to write to the database.
- **Pending Lock**
  - allows other processes that already have a shared lock to continue reading the database file.
  - But it prevents new shared locks from being established.
  - so eventually all shared locks will clear and the `pending lock` will then be able to escalate into an `exclusive lock`.
- **Exclusive Lock**
  - no other processes are reading from the file
  - it is safe to write changes to the database file

In Linux, there are 'shared lock' and 'exclusive' lock, other locks are SQLite only concept?

# The Commit Process
1. Acquire a *Read Lock* (*shared lock*)
2. Read data/pages from the database file
  - Usually only a subset of the pages in the database file are read.
  - read pages are thus in OS page cache and application buffer
3. Acquire a *Reserved Lock*
  - only one process holds this lock
4. Create a **Rollback Journal File**
  - file name is same name as the database file with the addition of a `-journal` suffix
  - it contains all information needed to restore the database back to its original state, including
    - original database file size
    - copies of pages (before they are updated)
    - the page number is stored together with each database page that is written into the rollback journal
    - count of pages (in journal header)
  - File is created at OS page cache, **not yet persisted** to storage device
  - **"Always Journal Complete Sectors"**
  - When the original content of a database page is written into the rollback journal (as shown in section 3.5), SQLite always writes a **complete sector** of data, even if the page size of the database is smaller than the sector size.
    - It is important to store all pages of a sector in the rollback journal in order to prevent database corruption following a power loss while writing the sector. Suppose that pages `1, 2, 3`, and `4` are all stored in sector 1 and that page 2 is modified. In order to write the changes to page `2`, the underlying hardware must also rewrite the content of pages `1, 3, and 4` since the hardware must write the complete sector. If this write operation is interrupted by a power outage, one or more of the pages `1, 3, or 4` might be left with incorrect data. Hence, to avoid lasting corruption to the database, the original content of all of those pages must be contained in the rollback journal.
5. Changing data pages in user space (application buffer)
  - at this stage, other processes can still read from database file
6. Flush the Rollback Journal File
  - This is a **two step process**
  - First, flush rollback journal content (with page count of zero)
  - Next, flush header with modified page count (non-zero)
  - Reason for two steps here is to protect against crash between two steps of file update
    1. file is extended to expected size (with garbage/random data) first
    2. then those file space is updated with correct content
  - SQLite introduces page checksum here to help detect data corruption
7. Acquire a *Exclusive Lock*
  - Acquire a *Pending Lock* then escalate into an exclusive lock
8. Write Changes to the database file
  - note usually the changes hit OS page cache only and not yet arrive in Disk
9. Flush Changes to the database file (to disk)
10. Delete the Rollback Journal
  - this step marks the end of a transaction commit
  - If a power failure or system crash occurs *prior to this point*, then recovery processes to be described later make it appear as if no changes were ever made to the database file.
  - If a power failure or system crash occurs *after the rollback journal is deleted*, then it appears as if all changes have been written to disk.
  - Thus, SQLite gives the appearance of having made no changes to the database file or having made the complete set of changes to the database file **depending on whether or not the rollback journal file exists**.
11. Release the *Exclusive Lock*

# The Rollback Process
1. Verify whether rollback journal is *hot*
2. Acquire an Exclusive Lock on the database file
3. Write into the database file with rollback journal
4. Delete the hot journal
5. Reduce Exclusive Lock to Shared Lock

# The Multi-File Commit Process
SQLite allows a single database connection to talk to two or more database files simultaneously through the use of the `ATTACH DATABASE` command. When multiple database files are modified within a single transaction, all files are updated atomically. In other words, either all of the database files are updated or else none of them are.
1. Create a **Rollback Journal File** for each database file
2. Create the master or super-journal
  - which contains fullpath to every participating journal files
3. Flush super-journal to disk
4. Updating Rollback Journal Headers
  - record the full pathname of the super-journal file in the header of every rollback journal
5. Flush each journal to disk
  - The content of each rollback journal is flushed to disk both **before and after** the super-journal filename is written into the rollback journal header.
  - It is important to do both of these flushes.
    - Fortunately, the second flush is usually inexpensive since typically only a single page of the journal file (the first page) has changed.
6. Acquire Exclusive Locks for each database file
7. Write Changes to each database file
8. Flush each database file (to disk)
9. Delete super-journal
  - This is the point where the multi-file transaction commits.
  - Upon restart, SQLite only considers a journal to be hot and will only playback the journal if there is no super-journal filename in the header (which is the case for a single-file commit) or if the super-journal file still exists on disk.
10. Delete each rollback journal files
11. Release Exclusive Locks

# Reference
- https://www.sqlite.org/atomiccommit.html
