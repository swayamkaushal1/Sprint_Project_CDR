#include "../../Header/utilityHeader.h"

// Get the path for CB.txt file
const char* getCBFilePath(void) {
    static char cb_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    snprintf(cb_path, MAX_FILE_PATH, "%s\\Output\\CB.txt", currentPath);
    return cb_path;
}

// Get the path for ISOB.txt file
const char* getISOBFilePath(void) {
    static char isob_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    snprintf(isob_path, MAX_FILE_PATH, "%s\\Output\\ISOB.txt", currentPath);
    return isob_path;
}

void getFilePaths(char *input_file, char *cb_output, char *isob_output) {
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    
    snprintf(input_file, MAX_FILE_PATH, "%s\\Data\\data.cdr", currentPath);
    snprintf(cb_output, MAX_FILE_PATH, "%s\\Output\\CB.txt", currentPath);
    snprintf(isob_output, MAX_FILE_PATH, "%s\\Output\\ISOB.txt", currentPath);
    
    // Ensure Output directory exists
    char outputDir[MAX_PATH_LENGTH];
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
    CreateDirectory(outputDir, NULL);
}