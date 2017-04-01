//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include "socket.h"
#include "codon.h"
#include "common.h"

int send_result(socket_t* client, int* codons) {
    char result_msg[MSG_SIZE];
    write_return_msg(codons, result_msg, MSG_SIZE);

    // First send over the message length
    unsigned int len = (int) strlen(result_msg);
    uint32_t nlen = htonl(len);
    socket_send(client, (char*) &nlen, sizeof(nlen));

    socket_send(client, result_msg, len);
    return 1;
}

int process_codons(socket_t* client) {
    char buf = 0;
    ssize_t bytes;
    int* codons = NULL;
    while (buf != EOF_CHAR) {
        bytes = socket_receive(client, &buf, 1);
        if (!bytes) {
            fprintf(stderr, "Error in receiving data from the client\n");
            return 1;
        }
        codons = codon_count(buf);
    }

    if (codons) {
        send_result(client, codons);
    }


    return 0;
}

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

    // Connection established
    if (process_codons(&client)) {
        return 1;
    }

    socket_destroy(&server);
    socket_destroy(&client);

    return 0;
}
