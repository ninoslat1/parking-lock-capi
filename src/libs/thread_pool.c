#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#include "../include/libs/thread_pool.h"
#include "../include/router.h"
#include "../include/libs/logger.h"
#include "../include/db/connection_pool.h"

static ThreadPool pool;

static DWORD WINAPI worker_thread(LPVOID arg) {
    (void) arg;

    while (1) {
        EnterCriticalSection(&pool.lock);

        while(pool.count == 0)
            SleepConditionVariableCS(&pool.not_empty, &pool.lock, INFINITE);
        
        SOCKET client_socket = pool.queue[pool.head];
        pool.head = (pool.head + 1) % QUEUE_CAPACITY;
        pool.count--;

        LeaveCriticalSection(&pool.lock);

        LOG_DEBUG("Thread picked up client, queue remaining: %d", pool.count);
        route_request(client_socket);
    }

    return 0;
}

void init_thread_pool() {
    InitializeCriticalSection(&pool.lock);
    InitializeConditionVariable(&pool.not_empty);

    pool.head  = 0;
    pool.tail  = 0;
    pool.count = 0;

    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        HANDLE t = CreateThread(NULL, 0, worker_thread, NULL, 0, NULL);

        if (t == NULL) {
            LOG_ERROR("Failed to create worker thread %d", i);
        } else {
            LOG_DEBUG("Worker thread %d started", i);
            CloseHandle(t);
        }
    }

    LOG_INFO("Thread pool initialized (%d workers)", THREAD_POOL_SIZE);
}

void enqueue_client(SOCKET client_socket) {
    EnterCriticalSection(&pool.lock);

    if (pool.count >= QUEUE_CAPACITY) {
        LOG_WARN("Queue full — dropping client connection");
        LeaveCriticalSection(&pool.lock);
        closesocket(client_socket);
        return;
    }

    pool.queue[pool.tail] = client_socket;
    pool.tail  = (pool.tail + 1) % QUEUE_CAPACITY;
    pool.count++;

    LOG_DEBUG("Client enqueued, queue size: %d", pool.count);

    WakeConditionVariable(&pool.not_empty);
    LeaveCriticalSection(&pool.lock);
}

