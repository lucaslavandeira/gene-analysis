#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "client.h"
#include "server.h"

void print_error_string() {
    fprintf(stderr, "Wrong input, usage:\n"
                    "\t./tp client <server> <port> <input-file>\n"
                    "OR\t./tp server <port>\n");
}

int main(int argc, char** argv) {
    /* arguments in order:  client <server> <port> <input-file>
     *                 OR   server <port>
     */
    if(argc == 1) {
        print_error_string();
        return 1;
    }

    if (!strcmp(argv[1], "client")) {
        char* address = argv[2];
        unsigned int port = (unsigned int)atoi(argv[3]);
        FILE* f = fopen(argv[4], "r");
        if (!f) {
            fprintf(stderr, "File input error!\n");
            return 1;
        }
        init_client(address, port, f);
        fclose(f);

    } else if (!strcmp(argv[1], "server")) {
        if(argc < 3) {
            print_error_string();
            return 1;
        }
        unsigned int port = (unsigned int) atoi(argv[2]);
        init_server(port);
    } else {
        print_error_string();
        return 1;
    }
    return 0;
}
