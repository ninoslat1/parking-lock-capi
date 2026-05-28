#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "../include/user_handlers.h"
#include "../include/libs/response_fmt.h"
#include "../include/store/user.h"
#include "../include/libs/json_parser.h"
#include "../include/db/connection_pool.h"

static void send_res(SOCKET client_socket, char *response) {
    send(client_socket, response, strlen(response), 0);
}

void handle_user_root(SOCKET client_socket){
    char *response = httpResponseFormat(
        200,
        "OK",
        "application/json",
        "{\"message\": \"Healthy\"}"
    );

    send_res(client_socket, response);
}

void handle_user_get(SOCKET client_socket) {

    MYSQL *conn = borrow_conn();

    int count = 0;
    User *users = get_all_users(conn, &count);

    if (!users) {
        char *response = httpResponseFormat(
            500,
            "Internal Server Error",
            "application/json",
            "{\"message\": \"Failed to fetch users\"}"
        );
        send_res(client_socket, response);
        free(response);
        return;
    }

    char json[4096] = "[";

    for (int i = 0; i < count; i++) {
        char temp[512];
        snprintf(
            temp, sizeof(temp),
            "%s{\"id\":%d,\"username\":\"%s\",\"usercode\":\"%s\"}",
            i > 0 ? "," : "",
            users[i].id,
            users[i].username,
            users[i].usercode
        );
        strncat(json, temp, sizeof(json) - strlen(json) - 1);
    }

    strncat(json, "]", sizeof(json) - strlen(json) - 1);
    free(users);

    char *response = httpResponseFormat(200, "OK", "application/json", json);
    send_res(client_socket, response);
    free(response);
}

void handle_user_post(SOCKET client_socket, const char *body) {

    MYSQL *conn = borrow_conn();
    char username[128] = {0}, usercode[64] = {0}, password[256] = {0};
    char *response;

    json_get_string(body, "username", username, sizeof(username));
    json_get_string(body, "usercode", usercode, sizeof(usercode));
    json_get_string(body, "password", password, sizeof(password));

    if (strlen(username) == 0 || strlen(password) == 0) {
        response = httpResponseFormat(
            400, "Bad Request", "application/json",
            "{\"message\": \"Username and password are required\"}"
        );
        send_res(client_socket, response);
        free(response);
        return;
    }

    User *existing = get_user_by_username(conn, username);
    if (existing != NULL) {
        free(existing);
        response = httpResponseFormat(
            409, "Conflict", "application/json",
            "{\"message\": \"Username already exists\"}"
        );
        send_res(client_socket, response);
        free(response);
        return;
    }

    int id = create_user(conn, username, usercode, password);
    if (id < 0) {
        response = httpResponseFormat(
            500, "Internal Server Error", "application/json",
            "{\"message\": \"Failed to create user\"}"
        );
        send_res(client_socket, response);
        free(response);
        return;
    }

    response = httpResponseFormat(
        201, "Created", "application/json",
       "{\"message\": \"User created\"}"
    );
    send_res(client_socket, response);
    free(response);
}

void handle_user_delete(SOCKET client_socket, int id) {

    MYSQL *conn = borrow_conn();
    char *response;

    User *user = get_user_by_id(conn, id);
    if (user == NULL) {
        response = httpResponseFormat(
            404, "Not Found", "application/json",
             "{\"message\": \"User not found\"}"
        );
        send_res(client_socket, response);
        free(response);
        return;
    }

    free(user);

    int affected = delete_user(conn, id);
    if (affected < 0) {
        response = httpResponseFormat(
            500, "Internal Server Error", "application/json",
             "{\"message\": \"Failed to delete user\"}"
        );
        send_res(client_socket, response);
        free(response);
        return;
    }

    response = httpResponseFormat(
        200, "OK", "application/json",
         "{\"message\": \"User deleted\"}"
    );
    send_res(client_socket, response);
    free(response);
}

void handle_user_not_found(SOCKET client_socket) {
    char *response = httpResponseFormat(
        404, "Not Found", "application/json",
        "{\"message\": \"Route not found\"}"
    );
    send_res(client_socket, response);
    free(response);
}