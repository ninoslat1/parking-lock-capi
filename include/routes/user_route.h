#ifndef USER_ROUTES_H
#define USER_ROUTES_H

#include <winsock2.h>
#include "../include/libs/request_fmt.h"

int user_routes(SOCKET client_socket, const char *buffer, HttpRequest *req);

#endif