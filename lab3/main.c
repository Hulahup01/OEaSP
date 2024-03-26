#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_utils.h"

int main(int argc, char *argv[]) {
    FILE *inputFile;
    FILE *outputFile;

    if (argc < 2) {
        printf("Usage: %s <input_file> [output_file]\n", argv[0]);
        return 1;
    }

    inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file");
        return 2;
    }

    outputFile = (argc >= 3) ? fopen(argv[2], "w") : stdout;
    if (!outputFile) {
        perror("Error opening output file");
        return 3;
    }

    invertLines(inputFile, outputFile);

    fclose(inputFile);
    if (argc >= 3) {
        fclose(outputFile);
    }

    return 0;
}
