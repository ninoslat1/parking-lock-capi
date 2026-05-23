#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "../include/router.h"
#include "../include/user_handlers.h"
#include "../include/libs/request_fmt.h"
#include "../include/structs/http_request.h"

void route_request(SOCKET client_socket) {
    HttpRequest req;

    char buffer[4096] = {0};

    recv(client_socket, buffer, sizeof(buffer), 0);
    parse_request(buffer, &req);

    if (
        strcmp(req.method, "GET") == 0 && 
        strcmp(req.path, "/") == 0
    ) {
        handle_user_root(client_socket);
    }

    else if (strncmp(buffer, "GET /users ", 11) == 0) {
       handle_user_get(client_socket);
    }

    else if (strncmp(buffer, "POST /users ", 12) == 0) {
        handle_user_post(client_socket, req.body);
    }

    else if (strncmp(buffer, "DELETE /users ", 14) == 0) {
        
        int id;
        sscanf(req.query, "id=%d", &id);

        handle_user_delete(client_socket, id);
    }

    else {
        handle_user_not_found(client_socket);
    }

    closesocket(client_socket);
}