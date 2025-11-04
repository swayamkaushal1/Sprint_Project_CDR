#ifndef MULTITHREADING_HEADER_H
#define MULTITHREADING_HEADER_H

#include <windows.h>
#include "custHeader.h"
#include "interopHeader.h"

/* ============================================================
   Function Declarations
   ============================================================ */

/**
 * @brief Processes CDR data using parallel threads for customer and interoperator billing
 * @param input_file Path to the input CDR file
 * @param cb_output Path to the customer billing output file
 * @param isob_output Path to the interoperator billing output file
 * @return int Returns 0 on success, 1 on failure
 */
int processCDRWithThreads(const char* input_file, const char* cb_output, const char* isob_output);

#endif /* MULTITHREADING_HEADER_H */