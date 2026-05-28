#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H

#include <mysql/mysql.h>
#include <windows.h>

#define CONN_POOL_SIZE 8
typedef CRITICAL_SECTION  PoolMutex;
typedef CONDITION_VARIABLE PoolCond;

typedef struct {
    MYSQL  *conns[CONN_POOL_SIZE];
    int     in_use[CONN_POOL_SIZE];
    PoolMutex lock;
    PoolCond  available;
} ConnPool;

#endif