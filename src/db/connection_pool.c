#include <stdio.h>
#include "../include/db/connection_pool.h"
#include "../include/db/mysql.h"
#include "../include/libs/logger.h"

static ConnPool pool;

void init_conn_pool() {
    InitializeCriticalSection(&pool.lock);
    InitializeConditionVariable(&pool.available);

    for(int i=0; i < CONN_POOL_SIZE; i++){
        pool.conns[i] = connect_db();
        pool.in_use[i] = 0;

        if(!pool.conns[i]){
            LOG_ERROR("Failed to initialize db connection %d", i);
        }
    }

    LOG_INFO("Connection pool ready (%d connections)", CONN_POOL_SIZE);
}

MYSQL *borrow_conn() {
    EnterCriticalSection(&pool.lock);
    while(1) {
        for(int i = 0;i<CONN_POOL_SIZE; i++){
            if(!pool.in_use[i]){
                pool.in_use[i] = 1;
                LeaveCriticalSection(&pool.lock);
                return pool.conns[i];
            }
        }

        SleepConditionVariableCS(&pool.available, &pool.lock, INFINITE);
    }
}

void return_conn(MYSQL *conn){
    EnterCriticalSection(&pool.lock);
    for (int i =0; i< CONN_POOL_SIZE; i++){
        if(pool.conns[i] == conn){
            pool.in_use[i] = 0;
            break;
        }
    }
    WakeConditionVariable(&pool.available);
    LeaveCriticalSection(&pool.lock);
}
