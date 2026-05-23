#include <stdio.h>
#include <string.h>

#include "../include/libs/request_fmt.h"
#include "../include/structs/http_request.h"

void parse_request(char *raw, HttpRequest *req) {
    sscanf(raw, "%s %s", req->method, req->path);

    char *query_start = strchr(req->path, '?');

    if (query_start != NULL) {

        strcpy(req->query, query_start + 1);

        *query_start = '\0';
    }

    char *body_start = strstr(raw, "\r\n\r\n");

    if (body_start != NULL) {
        strcpy(req->body, body_start + 4);
    }
}