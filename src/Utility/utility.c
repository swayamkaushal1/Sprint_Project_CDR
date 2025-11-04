#include "../../Header/utilityHeader.h"

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