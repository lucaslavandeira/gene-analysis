//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "client.h"
#include "socket.h"

char* read_input(FILE* f) {
    fseek(f, 0L, SEEK_END);
    size_t len = (size_t) ftell(f);
    fseek(f, 0L, SEEK_SET);
    char* buf = malloc(len);
    size_t read = fread(buf, 1, len, f);
    if(!read) {
        printf("ERROR: Failed to read the file properly\n");
        return NULL;
    }


    return buf;
}

int connect_to_server(socket_t* client,
                      const char* address,
                      unsigned int port)
{
    if (socket_create(client)) {
        perror("Error creating the socket");
        return 1;
    }
    if (socket_connect(client, address, port)) {
        perror("Error connecting to server");
        return 1;
    }
    return 0;
}


void init_client(const char* address, unsigned int port, FILE* input_file) {
    printf("Running as client\n");


    socket_t client;
    if (connect_to_server(&client, address, port)) {
        return;
    }

    char* input = read_input(input_file);
    if(!input) {
        return;
    }

    socket_send(&client, input, strlen(input));

    char buf[7];
    socket_receive(&client, buf, 7);
    printf("A server message: %s\n", buf);
    socket_destroy(&client);
    free(input);
}