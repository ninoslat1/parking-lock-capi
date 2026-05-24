#ifndef JSON_PARSE_H
#define JSON_PARSE_H

#include <stddef.h>

int json_get_string(const char *json, const char *key, char *out, size_t out_size);

#endif