//
// Created by lucas on 30/03/17.
//

#include <stdio.h>
#include "codon.h"
#include "common.h"

int* codon_count(int codon) {
    static int codons[CODON_AMT];
    codons[codon]++;
    return codons;
}

int write_return_msg(int* codons, char* buf, size_t len) {
    int bytes_printed = 0;
    for (int i = 0; i < CODON_AMT; i++) {
        if (codons[i]) {
            bytes_printed += snprintf(buf + bytes_printed, len,
                                    "CODON %i: %i\n", i, codons[i]);
        }
    }
    return 0;
}
