//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include "socket.h"

void init_server(unsigned int port) {
    socket_t server, client;
    if (socket_create(&server)) {
        perror("Error creating the socket");
        return;
    }

    if (socket_bind_and_listen(&server, port)) {
        perror("Error binding the socket");
        return;
    }


    if (socket_accept(&server, &client)) {
        perror("Error accepting a connection");
        return;
    }

    char buf[10];
    socket_receive(&client, buf, 10);

    printf("MESSAGE RECEIVED: %s\n", buf);
    socket_send(&client, "Hi man!", 7);
    return;
}
