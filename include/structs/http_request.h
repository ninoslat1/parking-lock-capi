#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

typedef struct {
    char method[16];
    char path[256];
    char query[256];
    char body[2048];
} HttpRequest;

#endif