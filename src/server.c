#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "../include/server.h"
#include "../include/router.h"
#include "../include/db/mysql.h"
#include "../include/libs/logger.h"
#include "../include/libs/thread_pool.h"
#include "../include/db/connection_pool.h"

void start_server(int port) {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    int server_fd;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 10);

    init_conn_pool();
    init_thread_pool();  

    LOG_INFO("Server running at http://localhost:%d\n", port);

    while (1) {

        int addrlen = sizeof(address);

        SOCKET client_socket =
            accept(server_fd, (struct sockaddr *)&address, &addrlen);

        if (client_socket == INVALID_SOCKET){
            LOG_WARN("accept() failed: %d", WSAGetLastError());
            continue;
        }

        enqueue_client(client_socket);
    }


    closesocket(server_fd);
    WSACleanup();
}