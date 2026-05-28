#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <mysql/mysql.h>

#include "../include/router.h"
#include "../include/libs/request_fmt.h"
#include "../include/routes/user_route.h"


void route_request(SOCKET client_socket) {
    HttpRequest req;
    int matched = 0;
    char buffer[4096] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    parse_request(buffer, &req);

    matched = matched || user_routes(client_socket, buffer, &req);
    
    if (!matched) {
        char *response = httpResponseFormat(
            404, "Not Found", "application/json",
            "{\"message\": \"Route not found\"}"
        );
        send_res(client_socket, response);
        free(response);
    }

    closesocket(client_socket);
}