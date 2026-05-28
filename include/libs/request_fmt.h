#ifndef REQUEST_H
#define REQUEST_H

#include "../include/structs/http_request.h"


// typedef struct {
//     char method[16];
//     char path[256];
//     char query[256];
//     char body[2048];
// } HttpRequest;

void parse_request(char *raw, HttpRequest *req);

#endif