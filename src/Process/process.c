#include "../../Header/multithreadingHeader.h"

/* ============================================================
   Thread Structure and Functions
   ============================================================ */

typedef struct {
    const char* input_file;
    const char* output_file;
} ThreadArgs;

DWORD WINAPI customerBillingThread(LPVOID args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    CustomerBillingFile(targs->input_file, targs->output_file);
    return 0;
}

DWORD WINAPI interoperatorBillingThread(LPVOID args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    InteroperatorBillingFile(targs->input_file, targs->output_file);
    return 0;
}

/* ============================================================
   Main Processing Function
   ============================================================ */

int processCDRWithThreads(const char* input_file, const char* cb_output, const char* isob_output) {
    HANDLE hCustThread, hInteropThread;
    ThreadArgs custArgs, interopArgs;
    
    // Setup thread arguments
    custArgs.input_file = input_file;
    custArgs.output_file = cb_output;
    interopArgs.input_file = input_file;
    interopArgs.output_file = isob_output;
    
    // Create customer billing thread
    hCustThread = CreateThread(NULL, 0, customerBillingThread, &custArgs, 0, NULL);
    if (hCustThread == NULL) {
        return 1;
    }
    
    // Create interoperator billing thread
    hInteropThread = CreateThread(NULL, 0, interoperatorBillingThread, &interopArgs, 0, NULL);
    if (hInteropThread == NULL) {
        CloseHandle(hCustThread);
        return 1;
    }
    
    // Wait for completion and cleanup
    WaitForSingleObject(hCustThread, INFINITE);
    WaitForSingleObject(hInteropThread, INFINITE);
    CloseHandle(hCustThread);
    CloseHandle(hInteropThread);
    
    return 0;
}
