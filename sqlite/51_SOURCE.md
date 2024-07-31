Key files:

`sqlite.h.in` - This file defines the public interface to the SQLite library. Readers will need to be familiar with this interface before trying to understand how the library works internally.

`sqliteInt.h` - this header file defines many of the data objects used internally by SQLite. In addition to "sqliteInt.h", some subsystems have their own header files.

`parse.y` - This file describes the LALR(1) grammar that SQLite uses to parse SQL statements, and the actions that are taken at each step in the parsing process.

`vdbe.c` - This file implements the virtual machine that runs prepared statements. There are various helper files whose names begin with "vdbe". The VDBE has access to the vdbeInt.h header file which defines internal data objects. The rest of SQLite interacts with the VDBE through an interface defined by vdbe.h.

`where.c` - This file (together with its helper files named by "where*.c") analyzes the WHERE clause and generates virtual machine code to run queries efficiently. This file is sometimes called the "query optimizer". It has its own private header file, whereInt.h, that defines data objects used internally.

`btree.c` - This file contains the implementation of the B-Tree storage engine used by SQLite. The interface to the rest of the system is defined by "btree.h". The "btreeInt.h" header defines objects used internally by btree.c and not published to the rest of the system.

`pager.c` - This file contains the "pager" implementation, the module that implements transactions. The "pager.h" header file defines the interface between pager.c and the rest of the system.

`os_unix.c` and os_win.c - These two files implement the interface between SQLite and the underlying operating system using the run-time pluggable VFS interface.

`shell.c.in` - This file is not part of the core SQLite library. This is the file that, when linked against sqlite3.a, generates the "sqlite3.exe" command-line shell. The "shell.c.in" file is transformed into "shell.c" as part of the build process.

`tclsqlite.c` - This file implements the Tcl bindings for SQLite. It is not part of the core SQLite library. But as most of the tests in this repository are written in Tcl, the Tcl language bindings are important.

`test*.c` - Files in the src/ folder that begin with "test" go into building the "testfixture.exe" program. The testfixture.exe program is an enhanced Tcl shell. The testfixture.exe program runs scripts in the test/ folder to validate the core SQLite code. The testfixture program (and some other test programs too) is built and run when you type "make test".
