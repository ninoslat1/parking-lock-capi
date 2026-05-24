#include "../include/server.h"
#include "../include/store/user.h"
#include "../include/config/env.h"

int main() {
    load_env(".env"); 
    start_server(8080);

    return 0;
}