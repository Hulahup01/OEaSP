#include "text_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void invertLines(FILE *inputFile, FILE *outputFile) {
    char **lines = malloc(MAX_LINE_LENGTH * sizeof(char *));
    char line[MAX_LINE_LENGTH];
    int lineCount = 0;
    
    while (fgets(line, MAX_LINE_LENGTH, inputFile) != NULL) {
        lines[lineCount] = strdup(line);
        lineCount++;
    }
    
    lines[lineCount - 1][strlen(lines[lineCount - 1])] = '\n';
 
    for (int i = lineCount - 1; i >= 0; i--) {
        fputs(lines[i], outputFile);
        free(lines[i]);
    }

    free(lines);
}