//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include "socket.h"

#include "main.h"

int* count_codon(int codon) {
    static int codons[CODON_AMT];
    codons[codon]++;
    return codons;
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

    char buf = 0;
    ssize_t bytes;
    int* codons = NULL;
    while (1) {
        bytes = socket_receive(&client, &buf, 1);
        if (bytes < 1) {
            break;
        }
        codons = count_codon(buf);
    }

    if (codons) {
        for (int i = 0; i < CODON_AMT; i++) {
            if (codons[i]) {
                printf("CODON %i: %i\n", i, codons[i]);
            }
        }
    }
    socket_destroy(&server);
    socket_destroy(&client);

    if (bytes < 0) { // error in receiving
        return 1;
    }
    return 0;
}
