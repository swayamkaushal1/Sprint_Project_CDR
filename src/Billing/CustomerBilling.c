#include "../../Header/custHeader.h"

void search_msisdn(const char *filename, long msisdn) {
    FILE *file = fopen(filename, "r");
    char line[1024];
    int found = 0;
    
    if (!file) {
        perror("Error opening file");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        // Look for line starting with "Customer ID: "
        if (strstr(line, "Customer ID: ") != NULL) {
            long current_msisdn;
            if (sscanf(line, "Customer ID: %ld", &current_msisdn) == 1) {
                if (current_msisdn == msisdn) {
                    found = 1;
                    // Print this line and next 11 lines for complete customer info
                    printf("%s", line);
                    for (int i = 0; i < 11; i++) {
                        if (fgets(line, sizeof(line), file)) {
                            printf("%s", line);
                        }
                    }
                    break;
                }
            }
        }
    }

    if (!found) {
        printf("Customer with MSISDN %ld not found.\n", msisdn);
    }

    fclose(file);
}

void display_customer_billing_file(const char *filename) {
    char ch;
    FILE *file = fopen(filename, "r");

    if (!file) {
        perror("Error opening file");
        return;
    }

    while (!feof(file)) {
        ch = fgetc(file);
        if (ch != EOF) {
            putchar(ch);
        }
    }

    fclose(file);
}
