//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "client.h"
#include "socket.h"
#include "codon.h"
#include "common.h"

#define BASES "AUGC"
#define CODON_LENGTH 3


int validate_base(char base) {
    /*  If char base is equal to any of
     * the BASES defined in client.h, returns the index
     * of said char in the BASES string, -1 otherwise
    */
    char* valid_bases = BASES;
    size_t len = strlen(valid_bases);
    for (int i = 0; i < len; i++) {
        if (base == valid_bases[i]) {
            return i;
        }
    }
    return -1;
}

char codify_codon(char *codon) {
    /* Validates source array codon to contain only characters in the BASES
     * tag in client.h and returns a char byte,
     * codifying each char by their position in the BASES string, being
     * A = 00
     * U = 01
     * G = 10
     * C = 11
     * or returns -1 if an invalid char is found.
     * with CODON_LENGTH = 3, and BASES = AUGC,
     * you end up with a number between 0 - 63 (4^3 choices)
    */

    unsigned char byte = 0;
    for (int i = 0; i < CODON_LENGTH; i++) {
        int result = validate_base(codon[i]);
        if (result < 0) {
            return -1;
        }
        result = result << (2 * (CODON_LENGTH - i - 1));
        byte += result;
    }

    return byte;
}

int send_input(FILE *f, socket_t* client) {
    /* Read in chunks of 3 characters, codifying them into a single byte as per
     * the codify_codon function, and pass it on to the server. Returns -1 in
     * case of failure, 0 otherwise.
     *
    */
    fseek(f, 0L, SEEK_END);
    size_t len = (size_t) ftell(f);

    if (len%3) {
        fprintf(stderr, "Invalid amount of codons, must be a multiple of 3\n");
        return -1;
    }
    fseek(f, 0L, SEEK_SET);


    char codon_buffer[CODON_LENGTH];
    size_t read;
    while (1) {
        read = fread(codon_buffer, sizeof(char), CODON_LENGTH, f);
        if (read < 1) {
            break;
        }
        char code = codify_codon(codon_buffer);
        if (code < 0) {
            fprintf(stderr, "ERROR: Wrong file format, "
                    "only include characters A, U, G or C\n");
            return -1;
        }
        socket_send(client, &code, sizeof(char));
    }

    // Send EOF_CHAR over to server to inform we've sent all our codons
    char end = EOF_CHAR;
    socket_send(client, &end, sizeof(char));

    return (int) read; // -1 if error, 0 if EOF reached normally
}

int receive_response(socket_t* client) {
    uint32_t len;
    socket_receive(client, (char*) &len, sizeof(uint32_t));
    printf("LENGTH: %u\n", ntohl(len));
    char message[MSG_SIZE] = "";
    socket_receive(client, message, (size_t) len);
    printf("%s", message);
    return 1;
}

int init_client(const char *address, unsigned int port, FILE *input_file) {
    socket_t client;
    if (socket_create(&client)) {
        perror("Error creating the socket");
        return 1;
    }
    if (socket_connect(&client, address, port)) {
        perror("Error connecting to server");
        return 1;
    }


    if (send_input(input_file, &client) < 0) {
        return 1;
    }


    if (receive_response(&client) < 0) {
        return 1;
    }
    socket_shutdown(&client, SHUT_WR);
    socket_destroy(&client);
    return 0;
}
