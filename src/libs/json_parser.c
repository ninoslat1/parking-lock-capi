#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../include/libs/json_parser.h"

int json_get_string(const char *json, const char *key, char *out, size_t out_size) {

    char search[128];
    snprintf(search, sizeof(search), "\"%s\"", key);

    const char *pos = strstr(json, search);
    if (!pos) return 0;

    pos += strlen(search);

    while (*pos && (isspace((unsigned char)*pos) || *pos == ':')) pos++;

    if (*pos != '"') return 0;
    pos++; // skip "

    size_t i = 0;
    while (*pos && *pos != '"' && i < out_size - 1) {
        out[i++] = *pos++;
    }
    out[i] = '\0';

    return 1;
}