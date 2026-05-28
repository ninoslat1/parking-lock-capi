#include <stdio.h>
#include <string.h>

#include "../include/routes/user_route.h"
#include "../include/handlers/user_handlers.h"

int user_routes(SOCKET client_socket, HttpRequest *req) {

    if (strcmp(req->method, "GET") == 0 && strcmp(req->path, "/") == 0) {
        handle_user_root(client_socket);
        return 1;
    }

    if (strcmp(req->method, "GET") == 0 && strcmp(req->path, "/users") == 0) {
        handle_user_get(client_socket);
        return 1;
    }

    if (strcmp(req->method, "POST") == 0 && strcmp(req->path, "/users") == 0) {
        handle_user_post(client_socket, req->body);
        return 1;
    }

    if (strcmp(req->method, "DELETE") == 0 && strcmp(req->path, "/users") == 0) {
        int id;
        sscanf(req->query, "id=%d", &id);
        handle_user_delete(client_socket, id);
        return 1;
    }

    return 0;
}