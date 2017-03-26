//
// Created by lucas on 25/03/17.
//

#ifndef GENE_ANALYSIS_CLIENT_H
#define GENE_ANALYSIS_CLIENT_H
#include <stdio.h>

#define BASES "AUGC"
#define CODON_LENGTH 3
int init_client(const char *address, unsigned int port, FILE *input);

#endif //GENE_ANALYSIS_CLIENT_H
