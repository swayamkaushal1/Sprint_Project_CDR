#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/types.h>
    #define PATH_SEPARATOR "/"
    #define GetCurrentDirectory(size, buf) getcwd(buf, size)
#endif

#include "multithreadingHeader.h"
#include "custHeader.h"
#include "interopHeader.h"

/* ============================================================
   Constants & Macros
   ============================================================ */
#define MAX_EMAIL_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define HASHED_LENGTH 20
#define LINE_BUFFER_SIZE 256

// Signup return codes
#define SIGNUP_SUCCESS 0
#define SIGNUP_FAILED 1
#define SIGNUP_USER_EXISTS 2

// Login return codes
#define LOGIN_SUCCESS 1
#define LOGIN_FAILED 0
#define LOGIN_USER_NOT_FOUND 2
#define MAX_PATH_LENGTH 1024
#define MAX_FILE_PATH 256

/* ============================================================
   Type Definitions
   ============================================================ */
typedef enum {
    MENU_AUTH,
    MENU_CDR
} MenuType;

typedef enum {
    AUTH_SIGNUP = 1,
    AUTH_LOGIN = 2,
    AUTH_EXIT = 3
} AuthMenuChoice;

typedef enum {
    CDR_PROCESS = 1,
    CDR_SEARCH = 2,
    CDR_LOGOUT = 3
} CDRMenuChoice;

typedef enum {
    BILL_CUSTOMER = 1,
    BILL_INTEROP = 2,
    BILL_EXIT = 3
} BillingTypeChoice;

typedef enum {
    CUST_SEARCH_MSISDN = 1,
    CUST_VIEW_ALL = 2,
    CUST_EXIT = 3
} CustomerBillChoice;

typedef enum {
    INTEROP_SEARCH_OPERATOR = 1,
    INTEROP_VIEW_ALL = 2,
    INTEROP_EXIT = 3
} InteropBillChoice;

/* ============================================================
   Function Declarations
   ============================================================ */

/**
 * @brief Display the authentication menu options
 */
void displayAuthMenu(void);

/**
 * @brief Display the CDR processing menu options
 */
void displayCDRMenu(void);

/**
 * @brief Handle the CDR processing menu and its operations
 * @return int Returns 0 on successful completion, 1 on error
 */
int handleCDRMenu(void);

/**
 * @brief Get the file paths for CDR processing
 * @param input_file Buffer to store input file path
 * @param cb_output Buffer to store customer billing output file path
 * @param isob_output Buffer to store interoperator billing output file path
 */
void getFilePaths(char *input_file, char *cb_output, char *isob_output);

/**
 * @brief Clear the input buffer
 */
void clearInputBuffer(void);

/**
 * @brief Handle invalid input
 * @param menu_type The current menu type
 */
void handleInvalidInput(MenuType menu_type);

/**
 * @brief Handle Customer Billing search and display operations
 */
void handleCustomerBillingSearch(void);

/**
 * @brief Handle Interoperator Billing search and display operations
 */
void handleInteroperatorBillingSearch(void);

/**
 * @brief Display billing type selection menu
 */
void displayBillingTypeMenu(void);

/**
 * @brief Display customer billing search menu
 */
void displayCustomerBillMenu(void);

/**
 * @brief Display interoperator billing search menu
 */
void displayInteropBillMenu(void);

/**
 * @brief Search for specific MSISDN in customer billing data
 * @param msisdn The MSISDN to search for
 */
void searchCustomerByMSISDN(long msisdn);

/**
 * @brief Search for operator in interoperator billing data
 * @param operator_id The operator ID or name to search for
 */
void searchByOperator(const char* operator_id);

/**
 * @brief Display all contents of a file
 * @param filepath Path to the file to display
 * @return int Returns 0 on success, 1 if file not found
 */
int displayFileContents(const char* filepath);

/* External function declarations */
extern int loginUser(void);
extern int signupUser(void);

#endif /* MAIN_HEADER_H */