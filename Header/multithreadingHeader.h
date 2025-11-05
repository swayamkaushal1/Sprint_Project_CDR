#ifndef MULTITHREADING_HEADER_H
#define MULTITHREADING_HEADER_H

#ifdef _WIN32
    #include <windows.h>
    typedef HANDLE thread_handle_t;
    #define THREAD_RETURN_TYPE DWORD WINAPI
    #define THREAD_PARAM LPVOID
#else
    #include <pthread.h>
    typedef pthread_t thread_handle_t;
    #define THREAD_RETURN_TYPE void*
    #define THREAD_PARAM void*
#endif

#include "custHeader.h"
#include "interopHeader.h"

// Cross-platform thread creation wrapper
#ifdef _WIN32
    #define CREATE_THREAD(handle, func, arg) \
        ((handle = CreateThread(NULL, 0, func, arg, 0, NULL)) != NULL)
    #define JOIN_THREAD(handle) \
        WaitForSingleObject(handle, INFINITE)
    #define CLOSE_THREAD(handle) \
        CloseHandle(handle)
#else
    #define CREATE_THREAD(handle, func, arg) \
        (pthread_create(&handle, NULL, func, arg) == 0)
    #define JOIN_THREAD(handle) \
        pthread_join(handle, NULL)
    #define CLOSE_THREAD(handle) \
        (void)handle // No explicit cleanup needed for pthreads
#endif

/* ============================================================
   Function Declarations
   ============================================================ */


int processCDRWithThreads(const char* input_file, const char* cb_output, const char* isob_output);

#endif /* MULTITHREADING_HEADER_H */