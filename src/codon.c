//
// Created by lucas on 30/03/17.
//

#include <stdio.h>
#include <string.h>
#include "codon.h"
#include "common.h"

#define kLineLength 4
enum codons {
    STOP = 0,
    Fen,
    Leu,
    Ser,
    Tir,
    Cis,
    Trp,
    Pro,
    His,
    Glu,
    Ile,
    Met,
    Tre,
    Asn,
    Lys,
    Arg,
    Val,
    Ala,
    Asp,
    Gli
};

void codon_count(char *codon, size_t len, int* count) {
    FILE* codes = fopen("codons.txt", "r");
    if (!codes) {
        return;
    }
    char line[kLineLength];
    int index = 0;
    while (1) {
        if (!fgets(line, kLineLength, codes)) { // EOF
            break;
        }
        for (int i = 0; i < len; i++) {
            int codon_id = atoi(line);
            if (index == codon[i]) {
                count[codon_id]++;
            }
        }
        index++;
    }

    return;
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
