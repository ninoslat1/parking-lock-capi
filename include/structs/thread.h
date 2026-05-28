#include <synchapi.h>
#include <winsock2.h>
#include <mysql/mysql.h>
#ifndef THREAD_H
#define THREAD_H

typedef struct {
    SOCKET  client_socket;
    MYSQL  *conn;
} ClientArgs;

typedef struct {
    ClientArgs queue[1024];
    int head, tail, count;
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE not_empty;
} ThreadPool;

#endif