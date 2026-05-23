#ifndef USERS_HANDLERS_H
#define USERS_HANDLERS_H

#include <winsock2.h>

void handle_user_root(SOCKET client_socket);
void handle_user_get(SOCKET client_socket);
void handle_user_post(SOCKET client_socket, const char *body);
void handle_user_delete(SOCKET client_socket, int id);
// void handle_user_patch(SOCKET client_socket);
void handle_user_not_found(SOCKET client_socket);

#endif