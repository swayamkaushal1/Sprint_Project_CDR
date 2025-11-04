#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
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

/* External function declarations */
extern int loginUser(void);
extern int signupUser(void);

#endif /* MAIN_HEADER_H */