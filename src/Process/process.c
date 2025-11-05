#include "../../Header/multithreadingHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   Thread Structure and Functions
   ============================================================ */

typedef struct {
    const char* input_file;
    const char* output_file;
} ThreadArgs;

#ifdef _WIN32
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
#else
    void* customerBillingThread(void* args) {
        ThreadArgs* targs = (ThreadArgs*)args;
        CustomerBillingFile(targs->input_file, targs->output_file);
        return NULL;
    }

    void* interoperatorBillingThread(void* args) {
        ThreadArgs* targs = (ThreadArgs*)args;
        InteroperatorBillingFile(targs->input_file, targs->output_file);
        return NULL;
    }
#endif

/* ============================================================
   Main Processing Function
   ============================================================ */

int processCDRWithThreads(const char* input_file, const char* cb_output, const char* isob_output) {
#ifdef _WIN32
    HANDLE hCustThread, hInteropThread;
#else
    pthread_t custThread, interopThread;
#endif
    ThreadArgs custArgs, interopArgs;
    
    // Setup thread arguments
    custArgs.input_file = input_file;
    custArgs.output_file = cb_output;
    interopArgs.input_file = input_file;
    interopArgs.output_file = isob_output;
    
#ifdef _WIN32
    // Windows thread creation
    hCustThread = CreateThread(NULL, 0, customerBillingThread, &custArgs, 0, NULL);
    if (hCustThread == NULL) {
        return 1;
    }
    
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
#else
    // POSIX thread creation
    if (pthread_create(&custThread, NULL, customerBillingThread, &custArgs) != 0) {
        return 1;
    }
    
    if (pthread_create(&interopThread, NULL, interoperatorBillingThread, &interopArgs) != 0) {
        pthread_cancel(custThread);
        return 1;
    }
    
    // Wait for completion
    pthread_join(custThread, NULL);
    pthread_join(interopThread, NULL);
#endif
    
    return 0;
}
