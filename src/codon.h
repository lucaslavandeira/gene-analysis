//
// Created by lucas on 30/03/17.
//

#ifndef GENE_ANALYSIS_CODON_H
#define GENE_ANALYSIS_CODON_H

#include <stdlib.h>

#define CODON_AMT 20

void codon_count(char *codes, size_t len, int *count);
int write_return_msg(int* codons, char* buf, size_t len);

#endif //GENE_ANALYSIS_CODON_H
