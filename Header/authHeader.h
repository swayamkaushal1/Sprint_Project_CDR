#ifndef MAINHEADER_H
#define MAINHEADER_H

// --------------------
// Common Includes
// --------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// --------------------
// Constants & Macros
// --------------------
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

// File paths (can be centralized here)
#define USERS_FILE_PATH "./data/Users.txt"
#define CB_FILE_PATH "./outputs/CB.txt"
#define IOSB_FILE_PATH "./outputs/IOSB.txt"

// --------------------
// Utility Function Prototypes (optional)
// --------------------
void logMessage(const char *message);  // Example: for logging

#endif // MAINHEADER_H
