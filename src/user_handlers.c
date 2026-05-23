#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "../include/user_handlers.h"
#include "../include/helper.h"
#include "../include/libs/response_fmt.h"
#include "../include/store/user.h"

void send_res(SOCKET client_socket, char *response){
    send(client_socket, response, strlen(response), 0);
}

void handle_user_root(SOCKET client_socket){
    char *response = httpResponseFormat(
        200,
        "OK",
        "application/json",
        JSON(
            {
                "message": "Healthy"
            }
        )
    );

    send_res(client_socket, response);
}

void handle_user_get(SOCKET client_socket) {

    char json[2048] = "[";

    for (int i = 0; i < user_count; i++) {

        char temp[256];

        sprintf(
            temp,
            "{\"id\":%d,\"name\":\"%s\"}",
            users[i].id,
            users[i].name
        );

        strcat(json, temp);

        if (i < user_count - 1) {
            strcat(json, ",");
        }
    }

    strcat(json, "]");

    char *response = httpResponseFormat(
        200,
        "OK",
        "application/json",
        json
    );

    send_res(client_socket, response);

    free(response);
}

void handle_user_post(SOCKET client_socket, const char *body) {
    char name[100] = {0};
    char *response;

    if (sscanf(body, "{\"name\":\"%[^\"]\"}", name) != 1) {
        response = httpResponseFormat(
            400,
            "Bad Request",
            "application/json",
            JSON(
                {
                    "message": "Invalid body"
                }
            )
        );

        send_res(client_socket, response);

        free(response);

        return;
    }
    
    size_t len = strlen(name);

    if (len == 0){
        response = httpResponseFormat(
            400,
            "Bad Request",
            "application/json",
            JSON(
                {
                    "message": "User created"
                }
            )
        );

        send_res(client_socket, response);

        free(response);

        return;
    }

    User *user = get_user_by_name(name);

    if(user != NULL){
        response = httpResponseFormat(
            409,
            "Conflict",
            "application/json",
            JSON(
                {
                    "message": "User created"
                }
            )
        );

        send_res(client_socket, response);

        free(response);

        return;

    } else {
        add_user(name);
    
        response = httpResponseFormat(
            201,
            "Created",
            "application/json",
            JSON(
                {
                    "message": "User created"
                }
            )
        );

        send_res(client_socket, response);

        free(response);

        return;
    }
}

void handle_user_delete(SOCKET client_socket, int id) {
    char *response;

    User *user = get_user_by_id(id);

    if(user == NULL){
        response = httpResponseFormat(
            404,
            "Not Found",
            "application/json",
            JSON(
                {
                    "message": "User not found"
                }
            )
        );

        send_res(client_socket, response);

        free(response);
    } else {
        delete_user(id);
        response = httpResponseFormat(
            200,
            "OK",
            "application/json",
            JSON(
                {
                    "message": "User deleted"
                }
            )
        );

        send_res(client_socket, response);

        free(response);
    }
}

void handle_user_not_found(SOCKET client_socket){
    char response[] = 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Not Found!";

    send_res(client_socket, response);
}