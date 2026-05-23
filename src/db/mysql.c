#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "../include/config/env.h"

MYSQL *connect_db() {

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
            getenv("DB_NAME"),
            atoi(getenv("DB_PORT")),
            NULL,
            0
        ) == NULL
    ) {

        printf("Connection failed: %s\n", mysql_error(conn));

        mysql_close(conn);

        return NULL;
    }

    return conn;
}