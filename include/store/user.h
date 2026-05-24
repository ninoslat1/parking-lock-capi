#ifndef USER_STORE_H
#define USER_STORE_H

#include "../include/structs/user.h"
#include <mysql/mysql.h>

User *get_all_users(MYSQL *conn, int *count);
User *get_user_by_id(MYSQL *conn, int id);
User *get_user_by_username(MYSQL *conn, const char *username);
int   create_user(MYSQL *conn, const char *username, const char *usercode, const char *password);
int   delete_user(MYSQL *conn, int id);

#endif