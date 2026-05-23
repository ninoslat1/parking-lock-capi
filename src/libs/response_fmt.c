#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/libs/response_fmt.h"

char *httpResponseFormat(int statusCode, const char *statusResponse, const char *contentType, const char *response){
    char *buffer = malloc(4096);

    sprintf(
        buffer,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "\r\n"
        "%s",
        statusCode,
        statusResponse,
        contentType,
        response
    );

    return buffer;
}