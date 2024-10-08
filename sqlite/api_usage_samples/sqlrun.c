#include <stdio.h>
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **colName) {
    // Loop over each column in the current row
    int i;
    for (i = 0; i < argc; i++){
        printf("%s = %s, ", colName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv) {
    sqlite3* db = 0;
    char* zErrMsg = 0;
    int rc;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s DATABASE-FILE-NAME SQL-STATEMENT\n", argv[0]);
        return -1;
    }

    rc = sqlite3_open(argv[1], &db);
    if (rc != SQLITE_OK){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -2;
    }

    rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
    if (rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
    }

    sqlite3_close(db);
    return rc;
}
