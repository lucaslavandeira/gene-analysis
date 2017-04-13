//
// Created by lucas on 30/03/17.
//

#ifndef GENE_ANALYSIS_CODON_H
#define GENE_ANALYSIS_CODON_H

#include <stdlib.h>

#define CODON_AMT 20

/*
 * Counts occurrences of 'codons' (numbers up to CODON_AMT) in the byte array
 * 'codes', and stores the count in the int array 'count'
 */
void codon_count(char *codes, size_t len, int *count);

/*
 * Writes a formatted return message in 'buf' using the data set from 'codons',
 * as specified by the assignment.
 */
int codon_write_return_msg(int *codons, char *buf, size_t len);

#endif //GENE_ANALYSIS_CODON_H
