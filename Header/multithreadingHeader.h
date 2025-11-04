#ifndef MULTITHREADING_HEADER_H
#define MULTITHREADING_HEADER_H

#include <windows.h>
#include "custHeader.h"
#include "interopHeader.h"

/* ============================================================
   Function Declarations
   ============================================================ */


int processCDRWithThreads(const char* input_file, const char* cb_output, const char* isob_output);

#endif /* MULTITHREADING_HEADER_H */