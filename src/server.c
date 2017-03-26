//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include "socket.h"

int init_server(unsigned int port) {
    socket_t server, client;
    if (socket_create(&server)) {
        perror("Error creating the socket");
        return 1;
    }

    if (socket_bind_and_listen(&server, port)) {
        perror("Error binding the socket");
        return 1;
    }


    if (socket_accept(&server, &client)) {
        perror("Error accepting a connection");
        return 1;
    }

    char buf = 0;
    ssize_t bytes = 1;
    while (bytes > 0) {
        bytes = socket_receive(&client, &buf, 1);
        printf("MESSAGE RECEIVED: %d\n", buf);
    }

    socket_send(&client, "Hi man!", 7);
    socket_destroy(&server);
    socket_destroy(&client);
    return 0;
}
