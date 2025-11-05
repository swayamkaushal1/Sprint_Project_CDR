#ifndef UTILITY_HEADER_H
#define UTILITY_HEADER_H

#include "../Header/mainHeader.h"

/**
 * @brief Get the current working directory in a platform-independent way
 * @param buffer The buffer to store the path in
 * @param size The size of the buffer
 */
void get_current_dir(char* buffer, size_t size);

/* File Path Functions */
void getFilePaths(char *input_file, char *cb_output, char *isob_output);

/**
 * @brief Get the path for Customer Billing output file (CB.txt)
 * @return const char* Path to CB.txt file
 */
const char* getCBFilePath(void);

/**
 * @brief Get the path for Interoperator Settlement Billing output file (ISOB.txt)
 * @return const char* Path to ISOB.txt file
 */
const char* getISOBFilePath(void);

#endif /* UTILITY_HEADER_H */