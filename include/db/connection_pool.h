#ifndef CONN_POOL_H
#define CONN_POOL_H

#include <mysql/mysql.h>
#include "../include/structs/mysql_pool.h"

void   init_conn_pool();
MYSQL *borrow_conn(DbTarget db);
void   return_conn(DbTarget db, MYSQL *conn);

#endif