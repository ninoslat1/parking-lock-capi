#ifndef MYSQL_DB_H
#define MYSQL_DB_H

#include <mysql/mysql.h>

MYSQL *connect_db_to(const char *db_name);

#endif