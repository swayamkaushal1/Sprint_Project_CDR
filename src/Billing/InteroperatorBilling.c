#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Header/interopHeader.h"

#define MAX_LINE 1024

// Helper function to convert a string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void search_operator(const char *filename, const char *operator_input) {
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE];
    int found = 0;

    if (!file) {
        printf("Error opening file: %s\n", strerror(errno));
        printf("Filename: %s\n", filename);
        printf("Note: Please process the CDR data first using option 1 from the main menu.\n");
        return;
    }

    // Convert user input to lowercase
    char operator_lower[50];
    strncpy(operator_lower, operator_input, sizeof(operator_lower));
    operator_lower[sizeof(operator_lower) - 1] = '\0';
    to_lowercase(operator_lower);

    while (fgets(line, sizeof(line), file)) {
        // Make a lowercase copy of the line
        char line_lower[MAX_LINE];
        strncpy(line_lower, line, sizeof(line_lower));
        line_lower[sizeof(line_lower) - 1] = '\0';
        to_lowercase(line_lower);

        if (strstr(line_lower, operator_lower)) {
            found = 1;
            printf("%s", line);
            for (int i = 0; i < 6; i++) {
                if (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
            }
            break;
        }
    }

    if (!found) {
        printf("Operator '%s' not found.\n", operator_input);
    }

    fclose(file);
}

void display_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    char buffer[4096];

    if (!file) {
        printf("Error opening file: %s\n", strerror(errno));
        printf("Filename: %s\n", filename);
        printf("Note: Please process the CDR data first using option 1 from the main menu.\n");
        return;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    fclose(file);
}

