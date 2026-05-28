#ifndef REQUEST_H
#define REQUEST_H

#include "../include/structs/http_request.h"

void parse_request(char *raw, HttpRequest *req);

#endif