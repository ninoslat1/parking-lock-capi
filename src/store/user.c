#include <string.h>

#include "../include/store/user.h"

User users[MAX_USERS];

int user_count = 0;

void add_user(const char *name) {

    User user;

    user.id = user_count + 1;

    strcpy(user.name, name);

    users[user_count] = user;

    user_count++;
}

void delete_user(int id) {

    for (int i = 0; i < user_count; i++) {

        if (users[i].id == id) {

            for (int j = i; j < user_count - 1; j++) {
                users[j] = users[j + 1];
            }

            user_count--;

            break;
        }
    }
}

User *get_user_by_name(const char *name) {

    for (int i = 0; i < user_count; i++) {

        if (strcmp(users[i].name, name) == 0) {

            return &users[i];
        }
    }

    return NULL;
}

User *get_user_by_id(int id) {

    for (int i = 0; i < user_count; i++) {

        if (users[i].id == id) {

            return &users[i];
        }
    }

    return NULL;
}