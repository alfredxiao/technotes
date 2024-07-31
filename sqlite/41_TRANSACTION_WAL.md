# Approach
Contrary to rollback journal, in WAL, the original content is preserved in the database file and the changes are appended into a separate WAL file.
- A `COMMIT` occurs when a *special record* indicating a commit is appended to the WAL.
- Thus a `COMMIT` can happen without ever writing to the original database, which allows readers to continue operating from the original unaltered database while changes are simultaneously being committed into the WAL.
- Multiple transactions can be appended to the end of a single WAL file.
- A particular database will use either a rollback journal or a WAL, but *not both* at the same time.

# Checkpointing
Of course, one wants to eventually transfer all the transactions that are appended in the WAL file back into the original database.

Moving the WAL file transactions back into the database is called a **checkpoint**.

# Reader Algorithm
reader: SQL statement(s) run as a unit, that only queries data.

1. Locate a Page:
To read a page from the database (call it page number `P`), a reader first checks the WAL to see if it contains page `P`. If so, then the last valid instance of page P that is followed by a commit frame or is a commit frame itself becomes the value read. If the WAL contains no copies of page P that are valid and which are a commit frame or are followed by a commit frame, then page `P` is read from the database file.

2. Records `mxFrame`
To start a read transaction, the reader records the number of **valid frames** in the WAL as `"mxFrame"`.
- Call this point the *"end mark"*.
- The reader uses this recorded `mxFrame` value for all subsequent read operations.
- New transactions can be appended to the WAL, but as long as the reader uses its original `mxFrame` value and ignores subsequently appended content, the reader will see a **consistent snapshot** of the database from a single point in time.
  - This technique allows *multiple concurrent readers* to view different versions of the database content simultaneously.

The reader algorithm in the previous paragraphs works correctly, but because frames for page `P` can appear *anywhere* within the WAL, the reader has to scan the entire WAL looking for page `P` frames. If the WAL is large (multiple megabytes is typical) that scan can be slow, and read performance suffers. To overcome this problem, a separate data structure called the `wal-index` is maintained to expedite the search for frames of a particular page.

# Writer Algorithm
writer: SQL statement(s) run as a unit, that modifies data and most likely queries data as well.
- Writers **merely append** new content to the end of the WAL file.

# Checkpoint Algorithm
1. On a checkpoint, the WAL is first flushed to persistent storage using the `xSync` method of the `VFS`.
2. Then valid content of the WAL is transferred into the database file.
  - no need to prevent readers from reading WAL or database file as this is writing into pages of database file that have newer copies in WAL
3. Finally, the database is flushed to persistent storage using another `xSync` method call.
  - The `xSync` operations serve as write barriers - all writes launched before the `xSync` must complete before any write that launches after the `xSync` begins.

A checkpoint need not run to completion, meaning not necessary all frames in WAL are transferred in one checkpointing operation, details see below.

# Concurrency
## For Readers
- Because the WAL can be growing and adding new commit records while various readers connect to the database, each reader can potentially have its own *end mark*.
- But for any particular reader, the end mark is unchanged for the duration of the transaction, thus ensuring that a single read transaction only sees the database content as it existed at a single point in time.
- When a reader needs a page of content, it first checks the WAL to see if that page appears there, and if so it pulls in **the last copy** of the page that occurs in the WAL *prior* to the reader's end mark.
- Read performance deteriorates as the WAL file grows in size since each reader must check the WAL file for the content and the time needed to check the WAL file is proportional to the size of the WAL file.
  - The `wal-index` helps find content in the WAL file much faster, but performance still falls off with increasing WAL file size.
  - Hence, to maintain good read performance it is important to keep the WAL file size down by running checkpoints at regular intervals.

## For Writers
- Because writers do nothing that would interfere with the actions of readers, writers and readers can run *at the same time*.
- However, since there is only one WAL file, there can only be **one writer at a time**.
- Whenever a write operation occurs, the writer checks how much progress the checkpointer has made, and if the entire WAL has been transferred into the database and synced and if no readers are making use of the WAL, then the writer will rewind the WAL back to the beginning and start putting new transactions at the beginning of the WAL. This mechanism prevents a WAL file from growing without bound.
  - this is when condition `mxFrame==nBackfill` is met
- Write transactions are very fast since they only involve writing the content once (versus twice for rollback-journal transactions) and because the writes are all sequential.
- Further, syncing the content to the disk is not required, as long as the application is willing to sacrifice durability following a power loss or hard reboot.
  - (Writers sync the WAL on every transaction commit if `PRAGMA synchronous` is set to `FULL` but omit this sync if `PRAGMA synchronous` is set to `NORMAL`.)

## For Checkpointing
- A checkpoint operation takes content from the WAL file and transfers it back into the original database file.
- A checkpoint can run concurrently with readers, however the checkpoint must **stop** when it reaches a page in the WAL that is *past the end mark of any current reader*.
  - The checkpoint has to stop at that point because otherwise it might overwrite part of the database file that the reader is actively using, making the reader's snapshot view non-consistent.
  - actually, if the reader has loaded such page(s) into its application buffer (not OS page cache), would this be OK for checkpointing to write into database file?
- The checkpoint remembers (`nBackfill` in the wal-index) how far it got and will *resume* transferring content from the WAL to the database from where it left off on the next invocation.
- Thus a long-running read transaction can prevent a checkpointer from making progress.
  - But presumably every read transaction will eventually end and the checkpointer will be able to continue.

## Concurrency when Checkpointing
### `Sync`
- Checkpointing does require `sync` operations in order to avoid the possibility of database corruption following a power loss or hard reboot.
-  The WAL must be synced to persistent storage prior to moving content from the WAL into the database and the database file must by synced prior to resetting the WAL.
- Checkpoint also requires more seeking. The checkpointer makes an effort to do as many sequential page writes to the database as it can (the pages are transferred from WAL to database in ascending order) but even then there will typically be many seek operations interspersed among the page writes. These factors combine to make checkpoints slower than write transactions.
- Note that with `PRAGMA` synchronous set to `NORMAL`, the checkpoint is the only operation to issue an I/O barrier or sync operation (`fsync()` on unix or `FlushFileBuffers()` on windows).
  - If an application therefore runs checkpoint in a separate thread or process, the main thread or process that is doing database queries and updates will never block on a sync operation.
  - This helps to prevent "latch-up" in applications running on a busy disk drive. The downside to this configuration is that transactions are no longer durable and might rollback following a power failure or hard reset.

### When to Checkpoint
- By default, SQLite will automatically checkpoint whenever a `COMMIT` occurs that causes the WAL file to be `1000` pages or more in size, or when the *last* database connection on a database file *closes*.
- By default, the checkpoint will be run automatically by the *same thread* that does the `COMMIT` that pushes the WAL over its size limit.
  - This has the effect of causing most COMMIT operations to be very fast but an occasional `COMMIT` (those that trigger a checkpoint) to be much slower.
  - If that effect is undesirable, then the application can disable automatic checkpointing and run the periodic checkpoints in a separate thread, or separate process.

# Performance Tradeoff
Notice too that there is a tradeoff between average read performance and average write performance.
- To maximize *read performance*, one wants to keep the WAL as small as possible and hence run checkpoints **frequently**, perhaps as often as every `COMMIT`.
- To maximize *write performance*, one wants to run checkpoints **infrequently** and let the WAL grow as large as possible before each checkpoint.
- The decision of how often to run checkpoints may therefore vary from one application to another depending on the relative read and write performance requirements of the application.

- The default strategy is to run a checkpoint once the WAL reaches `1000` pages and this strategy seems to work well in test applications on workstations, but other strategies might work better on different platforms or for different workloads.

# File Format
## Files On Disk
When in active use, the state of a WAL mode database is described by three separate files:
1. The main database file with an arbitrary name **"X"**.
2. The write-ahead log file, usually named **"X-wal"**.
3. The wal-index file, usually named **"X-shm"**.

## WAL File Format
- A WAL file consists of a header followed by zero or more `"frames"`.
- Each frame records the revised content of a single page from the database file.
- All changes to the database are recorded by writing frames into the WAL.
- Transactions commit when a frame is written that contains a **commit marker**.
- A single WAL can and usually does record multiple transactions.
- A WAL always grows from beginning toward the end.

### WAL File Header
It is 32 byte, covering (and others):
- Database page size
- Checkpoint sequence number

### WAL Frame
Each frame consists of a 24-byte **frame-header** followed by a page-size bytes of page data.
#### WAL Frame Header
24-byte, consisting of 6 integers, covering (and others):
- *page number*
- *"size of database"*: For commit records, the size of the database file in pages after the commit. For all other records, zero.

## WAL Index Format
Two important fields:
1. **The `mxFrame` field**
An integer that is the *number of valid frames* in the WAL.
- WAL frame are numbered starting with 1, `mxFrame` is also the index of the last valid commit frame in the WAL.
  - A commit frame is a frame that has a *non-zero* *size of database* of the frame header, and that indicates the end of a transaction.

Possible Cases:
- `mxFrame==0`: indicating that the WAL is empty and that all content should be obtained directly from the database file.
- `mxFrame==nBackfill`: indicating that all content in the WAL has been written back into the database. In that case, all content can be read directly from the database. Furthermore, the next writer is free to reset the WAL.

2. **The `nBackfill` field**
An integer that holds the number of frames in the WAL file which have been copied back into the main database.

# Reference
- https://www.sqlite.org/wal.html
