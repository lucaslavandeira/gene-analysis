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
    codon_write_return_msg(codons, result_msg, MSG_SIZE);

    // First send over the message length
    unsigned int len = (int) strlen(result_msg);
    uint32_t nlen = htonl(len);
    socket_send(client, (char*) &nlen, sizeof(nlen));

    socket_send(client, result_msg, len);
    return 1;
}

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

    // Connection established
    uint32_t len = 0;
    socket_receive(&client, (char*) &len, sizeof(len));
    len = ntohl(len);
    char codons[MAX_CODONS];
    socket_receive(&client, codons, len);

    int count[CODON_AMT] = {0};
    memset(count, 0, sizeof(count));
    codon_count(codons, len, count);

    char msg[MSG_SIZE] = "";

    if (codon_write_return_msg(count, msg, MSG_SIZE)) {
        fprintf(stderr, "Error writing message, check validity of " \
                        "codons.txt and codon_types.txt\n");

        socket_destroy(&server);
        socket_destroy(&client);
        return 1;
    }

    len = ntohl((uint32_t) strlen(msg));
    socket_send(&client, (char*) &len, sizeof(len));

    socket_send(&client, msg, strlen(msg));

    socket_destroy(&server);
    socket_destroy(&client);

    return 0;
}
