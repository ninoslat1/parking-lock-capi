#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <winsock2.h>
#include <mysql/mysql.h>
#include "../include/structs/thread.h"

void init_thread_pool();
void enqueue_client(SOCKET client_socket);

#endif