#include "../../Header/utilityHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>  /* for _mkdir */
#else
#include <unistd.h>  /* for getcwd */
#include <sys/types.h>
#endif

// Platform-specific functions
static int make_directory(const char* path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0777);
#endif
}

void get_current_dir(char* buffer, size_t size) {
#ifdef _WIN32
    GetCurrentDirectory(size, buffer);
#else
    if (getcwd(buffer, size) == NULL) {
        fprintf(stderr, "Error getting current directory: %s\n", strerror(errno));
        buffer[0] = '.';
        buffer[1] = '\0';
    }
#endif
}

// Get the path for CB.txt file
const char* getCBFilePath(void) {
    static char cb_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    char outputDir[MAX_PATH_LENGTH];
    
    get_current_dir(currentPath, MAX_PATH_LENGTH);
    
    // Create full path for Output directory
#ifdef _WIN32
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
#else
    snprintf(outputDir, MAX_PATH_LENGTH, "%s/Output", currentPath);
#endif

    // Create directory and handle errors
    if (make_directory(outputDir) != 0 && errno != EEXIST) {
        fprintf(stderr, "Error creating directory %s: %s\n", outputDir, strerror(errno));
        return NULL;  // Return NULL on error
    }

    // Create full path for file
#ifdef _WIN32
    snprintf(cb_path, MAX_FILE_PATH, "%s\\CB.txt", outputDir);
#else
    snprintf(cb_path, MAX_FILE_PATH, "%s/CB.txt", outputDir);
#endif
    return cb_path;
}

// Get the path for IOSB.txt file
const char* getISOBFilePath(void) {
    static char isob_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    char outputDir[MAX_PATH_LENGTH];
    
    get_current_dir(currentPath, MAX_PATH_LENGTH);
    
    // Create full path for Output directory
#ifdef _WIN32
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
#else
    snprintf(outputDir, MAX_PATH_LENGTH, "%s/Output", currentPath);
#endif

    // Create directory and handle errors
    if (make_directory(outputDir) != 0 && errno != EEXIST) {
        fprintf(stderr, "Error creating directory %s: %s\n", outputDir, strerror(errno));
        return NULL;  // Return NULL on error
    }

    // Create full path for file
#ifdef _WIN32
    snprintf(isob_path, MAX_FILE_PATH, "%s\\IOSB.txt", outputDir);
#else
    snprintf(isob_path, MAX_FILE_PATH, "%s/IOSB.txt", outputDir);
#endif
    return isob_path;
}

void getFilePaths(char *input_file, char *cb_output, char *isob_output) {
    char currentPath[MAX_PATH_LENGTH];
    
    // Get current directory path
    get_current_dir(currentPath, MAX_PATH_LENGTH);
    
    // Create Data and Output directories
    char dataDir[MAX_PATH_LENGTH];
    char outputDir[MAX_PATH_LENGTH];
    
    // Create paths using platform-specific separators
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

    // Create directories if they don't exist and handle errors
    if (make_directory(dataDir) != 0 && errno != EEXIST) {
        fprintf(stderr, "Error creating Data directory: %s\n", strerror(errno));
    }
    if (make_directory(outputDir) != 0 && errno != EEXIST) {
        fprintf(stderr, "Error creating Output directory: %s\n", strerror(errno));
    }
    
    // Check if the input file exists
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
        fprintf(stderr, "Warning: Input file %s not found\n", input_file);
    } else {
        fclose(fp);
    }
}