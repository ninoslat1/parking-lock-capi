#include <synchapi.h>
#include <winsock2.h>
#include <mysql/mysql.h>

#ifndef THREAD_H
#define THREAD_H

#define THREAD_POOL_SIZE  8
#define QUEUE_CAPACITY    1024

typedef struct {
    SOCKET queue[QUEUE_CAPACITY];
    int head, tail, count;
    CRITICAL_SECTION lock;
    CONDITION_VARIABLE not_empty;
} ThreadPool;

#endif