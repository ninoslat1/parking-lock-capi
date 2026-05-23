#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config/env.h"

void load_env(const char *filepath) {

    FILE *file = fopen(filepath, "r");

    if (!file) {
        printf("Warning: could not open %s\n", filepath);
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\r\n")] = 0;

        char key[128], value[256];

        if (sscanf(line, "%127[^=]=%255s", key, value) == 2) {
            _putenv_s(key, value);
        }
    }

    fclose(file);
}