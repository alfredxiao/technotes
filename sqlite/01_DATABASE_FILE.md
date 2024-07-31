# What Files
- The complete state of an SQLite database is usually contained in a single file on disk called the `main database file`.
- During a transaction, SQLite stores additional information in a second file called the `rollback journal`, or if SQLite is in **WAL mode**, a `write-ahead log file`.

# Pages
- The `main database file` consists of one or many **pages**, normal page size at present is `4k`, same as OS page size.
- The first page is called `page 1`

## The Database Header
- The first 100 bytes of the `page 1` is `the database header`
- Which tells many pieces of metadata, e.g.
  - `page size`
  - page count
  - how many free pages
  - where to find head of free page linked list

## Page 1
- `Page 1` of a database file is the **root page** of a table b-tree that holds a special table named `sqlite_schema` or `sqlite_master`.
- This b-tree is known as the `schema table` since it stores the complete database schema.
- Table `sqlite_master` in `Page 1` holds rows (not only) about tables and their `rootpage` number, so can be used to lookup those tables' root page.
