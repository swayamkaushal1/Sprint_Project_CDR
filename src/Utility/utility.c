#include "../../Header/utilityHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

// Platform-specific directory creation
static int make_directory(const char* path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0777);
#endif
}

// Get the path for CB.txt file
const char* getCBFilePath(void) {
    static char cb_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    char outputDir[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    
#ifdef _WIN32
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
    make_directory(outputDir);
    snprintf(cb_path, MAX_FILE_PATH, "%s\\CB.txt", outputDir);
#else
    snprintf(outputDir, MAX_PATH_LENGTH, "%s/Output", currentPath);
    make_directory(outputDir);
    snprintf(cb_path, MAX_FILE_PATH, "%s/CB.txt", outputDir);
#endif
    return cb_path;
}

// Get the path for IOSB.txt file
const char* getISOBFilePath(void) {
    static char isob_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    char outputDir[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    
#ifdef _WIN32
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
    make_directory(outputDir);
    snprintf(isob_path, MAX_FILE_PATH, "%s\\IOSB.txt", outputDir);
#else
    snprintf(outputDir, MAX_PATH_LENGTH, "%s/Output", currentPath);
    make_directory(outputDir);
    snprintf(isob_path, MAX_FILE_PATH, "%s/IOSB.txt", outputDir);
#endif
    return isob_path;
}

void getFilePaths(char *input_file, char *cb_output, char *isob_output) {
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    
    // Create Data and Output directories
    char dataDir[MAX_PATH_LENGTH];
    char outputDir[MAX_PATH_LENGTH];
    
#ifdef _WIN32
    snprintf(dataDir, MAX_PATH_LENGTH, "%s\\Data", currentPath);
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
    snprintf(input_file, MAX_FILE_PATH, "%s\\data.cdr", dataDir);
    snprintf(cb_output, MAX_FILE_PATH, "%s\\CB.txt", outputDir);
    snprintf(isob_output, MAX_FILE_PATH, "%s\\IOSB.txt", outputDir);
#else
    snprintf(dataDir, MAX_PATH_LENGTH, "%s/Data", currentPath);
    snprintf(outputDir, MAX_PATH_LENGTH, "%s/Output", currentPath);
    snprintf(input_file, MAX_FILE_PATH, "%s/data.cdr", dataDir);
    snprintf(cb_output, MAX_FILE_PATH, "%s/CB.txt", outputDir);
    snprintf(isob_output, MAX_FILE_PATH, "%s/IOSB.txt", outputDir);
#endif

    // Create directories if they don't exist
    if (make_directory(dataDir) == 0) {
        printf("Created Data directory\n");
    }
    if (make_directory(outputDir) == 0) {
        printf("Created Output directory\n");
    }
    
    // Check if the input file exists
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        printf("Warning: Input file %s not found\n", input_file);
    } else {
        fclose(fp);
    }
}