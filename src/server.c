//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include "socket.h"
#include "codon.h"
#include "common.h"


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

    char buf;
    ssize_t bytes;
    int* codons = NULL;
    while (1) {
        bytes = socket_receive(&client, &buf, 1);
        if (buf == EOF_CHAR) {
            break;
        }
        codons = count_codon(buf);
    }

    if (codons) {
        char result_msg[MSG_SIZE];
        write_return_msg(codons, result_msg, MSG_SIZE);
        socket_send(&client, result_msg, MSG_SIZE);
    }

    socket_destroy(&server);
    socket_destroy(&client);

    if (bytes < 0) {
        fprintf(stderr, "Error in receiving data from the client\n");
        return 1;
    }
    return 0;
}
