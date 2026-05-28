#include <stdio.h>
#include <stdlib.h>   
#include "../include/db/connection_pool.h"
#include "../include/db/mysql.h"
#include "../include/libs/logger.h"

static ConnPool pools[DB_COUNT];

static const char *db_env_keys[DB_COUNT] = {
    "DB_MST",   
    "DB_USR",   
    "DB_TRS",   
};


void init_conn_pool() {
    for (int d = 0; d < DB_COUNT; d++){
        InitializeCriticalSection(&pools[d].lock);
        InitializeConditionVariable(&pools[d].available);
    
        const char *db_name = getenv(db_env_keys[d]);
        for(int i=0; i < CONN_POOL_SIZE; i++){
            pools[d].conns[i] = connect_db_to(db_name);
            pools[d].in_use[i] = 0;
    
            if(!pools[d].conns[i]){
                LOG_ERROR("Failed to initialize db connection %d", i);
            }
        }
    
        LOG_INFO("Connection pool ready (%d connections)", CONN_POOL_SIZE);
    }
}

MYSQL *borrow_conn(DbTarget db) {
    ConnPool *p = &pools[db];
    EnterCriticalSection(&p->lock);
    while(1) {
        for(int i = 0;i<CONN_POOL_SIZE; i++){
            if(!p->in_use[i]){
                p->in_use[i] = 1;
                LeaveCriticalSection(&p->lock);
                return p->conns[i];
            }
        }

        SleepConditionVariableCS(&p->available, &p->lock, INFINITE);
    }
}

void return_conn(DbTarget db, MYSQL *conn){
    ConnPool *p = &pools[db];

    EnterCriticalSection(&p->lock);
    for (int i =0; i< CONN_POOL_SIZE; i++){
        if(p->conns[i] == conn){
            p->in_use[i] = 0;
            break;
        }
    }
    WakeConditionVariable(&p->available);
    LeaveCriticalSection(&p->lock);
}
