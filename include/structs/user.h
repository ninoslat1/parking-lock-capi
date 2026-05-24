#ifndef USER_H
#define USER_H

typedef struct {
    int id;
    char username[128];
    char usercode[64];
    char password[256];
} User;

#endif