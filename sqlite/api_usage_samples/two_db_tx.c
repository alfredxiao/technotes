#include <stdio.h>
#include "sqlite3.h"

int main(void) {
    sqlite3* db = 0;
    sqlite3_open("db1", &db); /* Open a database named MyDB */
    sqlite3_exec(db, "attach database db2 as db2", 0, 0, 0);
    sqlite3_exec(db, "begin", 0, 0, 0);
    sqlite3_exec(db, "insert into Students values(2000)", 0, 0, 0);
    sqlite3_exec(db, "insert into Courses values('SQLite Database', 2000)", 0, 0, 0);
    sqlite3_exec(db, "commit", 0, 0, 0);

    // technically SQLite permits multiple SQL statements in a single exec API call;
    // the same batch of commands can be executed by passing this sequence of statements in a single exec call
    sqlite3_close(db);
    return SQLITE_OK;
}