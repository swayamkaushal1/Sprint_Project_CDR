#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../../Header/custHeader.h"
#include "../../Header/interopHeader.h"

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
   Main Function - Parallel Processing
   ============================================================ */

int main() {
    HANDLE hCustThread, hInteropThread;
    ThreadArgs custArgs, interopArgs;
    
    const char* input_file = "../../Data/data.cdr";
    const char* cb_output = "../../Output/CB.txt";
    const char* isob_output = "../../Output/ISOB.txt";
    
    printf("Processing CDR data...\n");
    
    custArgs.input_file = input_file;
    custArgs.output_file = cb_output;
    interopArgs.input_file = input_file;
    interopArgs.output_file = isob_output;
    
    // Create customer billing thread
    hCustThread = CreateThread(NULL, 0, customerBillingThread, &custArgs, 0, NULL);
    if (hCustThread == NULL) {
        fprintf(stderr, "Error creating customer billing thread\n");
        return 1;
    }
    
    // Create interoperator billing thread
    hInteropThread = CreateThread(NULL, 0, interoperatorBillingThread, &interopArgs, 0, NULL);
    if (hInteropThread == NULL) {
        fprintf(stderr, "Error creating interoperator billing thread\n");
        CloseHandle(hCustThread);
        return 1;
    }
    
    // Wait for completion and cleanup
    WaitForSingleObject(hCustThread, INFINITE);
    WaitForSingleObject(hInteropThread, INFINITE);
    CloseHandle(hCustThread);
    CloseHandle(hInteropThread);
    
    printf("CDR data processing completed\n");
    return 0;
}
