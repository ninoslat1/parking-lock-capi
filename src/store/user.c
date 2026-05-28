#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/store/user.h"

User *get_all_users(MYSQL *conn, int *count) {

    if (mysql_query(conn, "SELECT id, username, usercode, password FROM myuser")) {
        printf("Query failed: %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return NULL;

    int num_rows = (int)mysql_num_rows(res);
    User *users = malloc(sizeof(User) * num_rows);

    MYSQL_ROW row;
    int i = 0;

    while ((row = mysql_fetch_row(res))) {
        users[i].id = atoi(row[0]);
        strncpy(users[i].username, row[1], sizeof(users[i].username) - 1);
        strncpy(users[i].usercode, row[2], sizeof(users[i].usercode) - 1);
        strncpy(users[i].password, row[3], sizeof(users[i].password) - 1);
        i++;
    }

    mysql_free_result(res);
    *count = num_rows;

    return users;
}

User *get_user_by_id(MYSQL *conn, int id) {

    char query[256];
    snprintf(query, sizeof(query),
        "SELECT id, username, usercode, password FROM myuser WHERE id = %d", id);

    if (mysql_query(conn, query)) {
        printf("Query failed: %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return NULL;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return NULL;
    }

    User *user = malloc(sizeof(User));
    user->id = atoi(row[0]);
    strncpy(user->username, row[1], sizeof(user->username) - 1);
    strncpy(user->usercode, row[2], sizeof(user->usercode) - 1);
    strncpy(user->password, row[3], sizeof(user->password) - 1);

    mysql_free_result(res);

    return user;
}

User *get_user_by_username(MYSQL *conn, const char *username) {

    char escaped[256];
    mysql_real_escape_string(conn, escaped, username, strlen(username));

    char query[512];
    snprintf(query, sizeof(query),
        "SELECT id, username, usercode, password FROM myuser WHERE username = '%s'", escaped);

    if (mysql_query(conn, query)) {
        printf("Query failed: %s\n", mysql_error(conn));
        return NULL;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return NULL;

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return NULL;
    }

    User *user = malloc(sizeof(User));
    user->id = atoi(row[0]);
    strncpy(user->username, row[1], sizeof(user->username) - 1);
    strncpy(user->usercode, row[2], sizeof(user->usercode) - 1);
    strncpy(user->password, row[3], sizeof(user->password) - 1);

    mysql_free_result(res);

    return user;
}

int create_user(MYSQL *conn, const char *username, const char *usercode, const char *password) {

    char esc_username[256], esc_usercode[128], esc_password[512];

    mysql_real_escape_string(conn, esc_username, username, strlen(username));
    mysql_real_escape_string(conn, esc_usercode, usercode, strlen(usercode));
    mysql_real_escape_string(conn, esc_password, password, strlen(password));

    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO myuser (username, usercode, password) VALUES ('%s', '%s', '%s')",
        esc_username, esc_usercode, esc_password);

    if (mysql_query(conn, query)) {
        printf("Insert failed: %s\n", mysql_error(conn));
        return -1;
    }

    return (int)mysql_insert_id(conn);
}

int delete_user(MYSQL *conn, int id) {

    char query[256];
    snprintf(query, sizeof(query), "DELETE FROM myuser WHERE id = %d", id);

    if (mysql_query(conn, query)) {
        printf("Delete failed: %s\n", mysql_error(conn));
        return -1;
    }

    return (int)mysql_affected_rows(conn);
}