#ifndef USER_STORE_H
#define USER_STORE_H

#include "../include/structs/user.h"

#define MAX_USERS 100

extern User users[MAX_USERS];
extern int user_count;

void add_user(const char *name);

void delete_user(int id);

User *get_user_by_name(const char *name);

User *get_user_by_id(int id);
#endif