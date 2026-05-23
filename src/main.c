#include "../include/server.h"
#include "../include/store/user.h"
#include "../include/config/env.h"

int main() {
    load_env(".env"); 
    add_user("Alice");
    add_user("Bob");

    start_server(8080);

    return 0;
}