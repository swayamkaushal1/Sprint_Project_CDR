#include "../../Header/custHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

// Platform-specific directory creation
static int ensure_directory_exists(const char* path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0777);
#endif
}

void search_msisdn(const char *filename, long msisdn) {
    // First, ensure the Output directory exists
    char outputDir[1024];
    char *lastSlash = strrchr(filename, '/');
    char *lastBackslash = strrchr(filename, '\\');
    char *separator = lastSlash ? lastSlash : lastBackslash;
    
    if (separator) {
        strncpy(outputDir, filename, separator - filename);
        outputDir[separator - filename] = '\0';
        ensure_directory_exists(outputDir);
    }

    FILE *file = fopen(filename, "r");
    char line[1024];
    int found = 0;
    
    if (!file) {
        printf("Error opening file: %s\n", strerror(errno));
        printf("Filename: %s\n", filename);
        printf("Note: Please process the CDR data first using option 1 from the main menu.\n");
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
    // First, ensure the Output directory exists
    char outputDir[1024];
    char *lastSlash = strrchr(filename, '/');
    char *lastBackslash = strrchr(filename, '\\');
    char *separator = lastSlash ? lastSlash : lastBackslash;
    
    if (separator) {
        strncpy(outputDir, filename, separator - filename);
        outputDir[separator - filename] = '\0';
        ensure_directory_exists(outputDir);
    }

    // Try to open the file
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", strerror(errno));
        printf("Filename: %s\n", filename);
        printf("Note: Please process the CDR data first using option 1 from the main menu.\n");
        return;
    }

    // Read and display the file contents
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    fclose(file);
}
