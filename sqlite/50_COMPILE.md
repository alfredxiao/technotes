
tar xzf sqlite.tar.gz    ;#  Unpack the source tree into "sqlite"
mkdir bld                ;#  Build will occur in a sibling directory
cd bld                   ;#  Change to the build directory
../sqlite/configure      ;#  Run the configure script
make                     ;#  Builds the "sqlite3" command-line tool
make sqlite3.c           ;#  Build the "amalgamation" source file
make devtest             ;#  Run some tests (requires Tcl)


To Generate dynamic library:
`gcc -dynamiclib sqlite3.c -o libsqlite3.0.dylib -lm -lpthread`

To use the generated library:
`DYLD_LIBRARY_PATH=$PWD python3 -c "import sqlite3; print(sqlite3.sqlite_version)"`

To compile with debug enabled:
`gcc shell.c sqlite3.c -lpthread -ldl -lm -DSQLITE_DEBUG -o sqlite3`
or `configure CPPFLAGS=-DSQLITE_DEBUG`
`PRAGMA vdbe_trace=ON;`

Python Sample:
```
import sqlite3

c = sqlite3.connect("./fun.sqlite")
with open('/usr/share/dict/words') as f:
    c.execute("create table fun (id int not null primary key, word varchar(120));")
    for i, word in enumerate(f):
        word = word.strip()
        c.execute("INSERT INTO fun VALUES (?, ?);", (i, word))
c.commit()
c.close()
```



```
static MemPage *btreePageFromDbPage
static void btreeParseCell(
```
