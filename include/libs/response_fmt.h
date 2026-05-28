#ifndef RESPONSE_FMT_H
#define RESPONSE_FMT_H

#include "../include/structs/http_request.h"

char *httpResponseFormat(
    int statusCode,
    const char *statusResponse,
    const char *contentType,
    const char *response
);

#endif