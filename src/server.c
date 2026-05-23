#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "../include/server.h"
#include "../include/router.h"
#include "../include/db/mysql.h"
#include <mysql/mysql.h>

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

    printf("Server running at http://localhost:%d\n", port);

    MYSQL *db = connect_db();

    if (db != NULL) {
        printf("Connected!\n");
    }
    
    while (1) {

        int addrlen = sizeof(address);

        SOCKET client_socket =
            accept(server_fd, (struct sockaddr *)&address, &addrlen);

        if (client_socket != INVALID_SOCKET) {
            route_request(client_socket);
        }
    }


    closesocket(server_fd);
    WSACleanup();
}