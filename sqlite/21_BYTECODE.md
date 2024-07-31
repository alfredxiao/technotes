# How SQLite implements SQL
SQLite works by translating SQL statements into bytecode and then running that bytecode in a **virtual machine**. This is similar to how Java compiler translates Java source code into JVM bytecode and then they are executed in JVM.

# What is special about SQLite Bytecode
Compared with other kinds of bytecodes, SQLite bytecode has these kinds of low-level instructions, too. But SQLite bytecode also contains some *high-level* operations that are specific to the needs of a database engine. Here are just a few examples:

- `OP_Column` → Extract the value from the N-th column of the database row that a particular cursor is currently pointing at.
- `OP_CreateBtree` → Allocate space for a new B-Tree in the database file.
- `OP_ParseSchema` → Reread and reparse all or part of the sqlite_schema table and refresh internal symbol tables accordingly.
- `OP_SeekGE` → Move a cursor on a particular B-Tree to the first entry that is greater than or equal to a given key.
- `OP_Next` → Advance a cursor on a particular B-Tree to the next entry in the B-Tree and jump, or fall through if there are no more entries in that B-Tree.

In other words, the "bytecode" used by SQLite is not so much a set of CPU instructions as it is a list of database primitives that are to be run in a particular order.

# Prepared Statement
A **prepared statement** in SQLite is mostly just the bytecode needed to implement the corresponding SQL.

The `sqlite3_prepare_v2()` interface is a compiler that translates SQL into bytecode.

The `sqlite3_step()` interface is the virtual machine that runs the bytecode contained within the prepared statement. 
