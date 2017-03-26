//
// Created by lucas on 25/03/17.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "client.h"
#include "socket.h"

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

char send_input(FILE *f) {
    fseek(f, 0L, SEEK_END);
    size_t len = (size_t) ftell(f);

    if (len%3) {
        fprintf(stderr, "Invalid amount of codons, must be a multiple of 3\n");
        return -1;
    }
    fseek(f, 0L, SEEK_SET);


    char codon_buffer[CODON_LENGTH] = "";
    size_t read = fread(codon_buffer, sizeof(char), CODON_LENGTH, f);
    if (!read) {
        fprintf(stderr, "ERROR: Failed to read the file properly\n");
        return -1;
    }
    char code = codify_codon(codon_buffer);
    if (code < 0) {
        fprintf(stderr, "ERROR: Wrong file format, "
                "only include characters A, U, G or C\n");
        return -1;
    }


    return code;
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


int init_client(const char *address, unsigned int port, FILE *input_file) {
    socket_t client;
    if (connect_to_server(&client, address, port)) {
        return 1;
    }

    char input = send_input(input_file);
    if (input < 0) {
        return 1;
    }


    socket_send(&client, &input, sizeof(char));

    socket_destroy(&client);
    return 0;
}
