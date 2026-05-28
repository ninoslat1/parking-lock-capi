#ifndef ROUTER_H
#define ROUTER_H

#include <winsock2.h>
#include <mysql/mysql.h>

void route_request(SOCKET client_socket);

#endif