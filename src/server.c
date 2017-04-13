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


int init_server(unsigned int port) {
    socket_t server, client;

    if (socket_create(&server)) {
        perror("Error creating the socket");
        return 1;
    }

    if (socket_bind_and_listen(&server, port)) {
        perror("Error binding the socket");
        socket_destroy(&server);
        return 1;
    }


    if (socket_accept(&server, &client)) {
        perror("Error accepting a connection");
        socket_destroy(&server);
        return 1;
    }


    char codons[MAX_CODONS];
    size_t len = (size_t) socket_receive(&client, codons, MAX_CODONS);

    int count[CODON_AMT] = {0};
    codon_count(codons, len, count);



    char msg[MSG_SIZE];
    if (codon_write_return_msg(count, msg, MSG_SIZE)) {
        fprintf(stderr, "Error writing message, check validity of " \
                        "codons.txt and codon_types.txt\n");

        socket_destroy(&server);
        socket_destroy(&client);
        return 1;
    }

    len = strlen(msg);
    size_t offset = 0;
    while (len) {
        size_t to_send = len;
        if (to_send > CHUNK_SIZE) {
            to_send = CHUNK_SIZE;
        }
        ssize_t sent = socket_send(&client, msg + offset, to_send);
        if (sent < 0) {
            perror("Error sending message to client");
        }
        offset += sent;
        len -= sent;
    }
    socket_shutdown(&client, SHUT_WR);

    socket_destroy(&server);
    socket_destroy(&client);

    return 0;
}
