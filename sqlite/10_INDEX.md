index structure is ?

how to travel index?

An SQLite database file is essentially a collection of B-trees [ 23]. A B-tree is either a table
B-tree or an index B-tree. Table B-trees always use a 64-bit signed
integer key and store data in the leaves. Index B-trees use arbitrary
keys and store no data at all. Each table in the database schema is
represented by a table B-tree. The key of a table B-tree is the implicit
rowid column of the table. For INTEGER PRIMARY KEY tables, the
primary key column replaces the rowid as the B-tree key.

Tables declared with the specification `WITHOUT ROWID` are a special case;
these tables are stored entirely in index B-trees.
The B-tree key for a `WITHOUT ROWID` table is composed of the columns of the primary
key followed by all remaining columns of the table. There is one
index B-tree for each index in the database schema, unless that
index is already represented by a table B-tree, as in the case of
INTEGER PRIMARY KEY tables.
