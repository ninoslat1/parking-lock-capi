#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "../include/config/env.h"

static MYSQL *_connect(const char *db_name) {

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL) {
        printf("mysql_init failed\n");
        return NULL;
    }

    if (
        mysql_real_connect(
            conn,
            getenv("DB_HOST"),
            getenv("DB_USER"),
            getenv("DB_PASSWORD"),
            db_name,              
            atoi(getenv("DB_PORT")),
            NULL,
            0
        ) == NULL
    ) {
        printf("Connection failed [%s]: %s\n", db_name, mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    return conn;
}

// MYSQL *connect_db() {
//     return _connect(getenv("DB_NAME"));
// }

MYSQL *connect_db_to(const char *db_name) {
    return _connect(db_name);
}
