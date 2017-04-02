//
// Created by lucas on 30/03/17.
//

#include <stdio.h>
#include <string.h>
#include "codon.h"

#define kLineLength 4
#define kNameLength 25
#define FREQUENT_AMOUNT 3
#define STOP "STOP"


struct codon {
    char name[kNameLength];
    int count;
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

    fclose(codes);
}

int codon_compare(const struct codon* a, const struct codon* b) {
    if (a->count > b->count) {
        return -1;
    } else if (a->count < b->count) {
        return 1;
    }

    if (!strncmp(a->name, "Ácido", 6)) {
        if (b->name[0] == 'A') {
            return strcmp(a->name+2, b->name+1);
        }
        return -1;
    }

    if (!strncmp(b->name, "Ácido", 6)) {
        if (a->name[0] == 'A') {
            return strcmp(a->name+1, b->name+2);
        }
        return 1;
    }
    return strcmp(a->name, b->name);
}


int codon_write_return_msg(int *codons, char *buf, size_t len) {
    FILE* names = fopen("codon_types.txt", "r");
    if (!names) {
        return -1;
    }


    struct codon codon_array[CODON_AMT];
    for (int i = 0; i < CODON_AMT; i++) {
        if (!fgets(codon_array[i].name, kNameLength, names)) { // EOF
            break;
        }
        // Remove the newline char
        codon_array[i].name[strlen(codon_array[i].name) - 1] = '\0';
        codon_array[i].count = codons[i];
    }

    int bytes_printed = 0;
    for (int i = 0; i < CODON_AMT; i ++) {
        if (!strcmp(STOP, codon_array[i].name)) {
            bytes_printed = snprintf(buf,
                                     len,
                                     "Cantidad de proteínas encontradas:" \
                                             " %i\n\n",
                                     codon_array[i].count);

            codon_array[i].count = 0;
        }
    }
    bytes_printed += snprintf(buf + bytes_printed, len,
    "Aminoácidos más frecuentes:\n");

    for (int i = 0; i < FREQUENT_AMOUNT; i++) {
        int max = 0;
        for (int j = 1; j < CODON_AMT; j++) {
            if (codon_compare(&codon_array[j], &codon_array[max]) < 0) {
                max = j;
            }
        }
        bytes_printed += snprintf(buf + bytes_printed, len,
                                  "%i) %s: %i\n", i+1,
                                  codon_array[max].name,
                                  codon_array[max].count);
        codon_array[max].count = 0; // Next max will surely be different!
    }

    fclose(names);
    return 0;
}
