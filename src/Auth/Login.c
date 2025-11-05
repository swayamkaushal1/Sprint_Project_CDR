#include "../../Header/mainHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

static void hashPassword(const char *password, char *hashed) {
    unsigned int sum = 0;
    for (int i = 0; password[i]; i++)
        sum += password[i] * (i + 1);
    sprintf(hashed, "%u", sum);
}

int loginUser() {
    char email[100], password[100], hashed[20];
    printf("Enter Email: ");
    scanf("%s", email);
    printf("Enter Password: ");
    scanf("%s", password);
    hashPassword(password, hashed);
    
    char currentPath[1024];
    GetCurrentDirectory(1024, currentPath);
    char dataPath[1124];
    char filePath[1224];

#ifdef _WIN32
    sprintf(dataPath, "%s\\Data", currentPath);
    sprintf(filePath, "%s\\Users.txt", dataPath);
#else
    sprintf(dataPath, "%s/Data", currentPath);
    sprintf(filePath, "%s/Users.txt", dataPath);
#endif
    
    // Try to open the users file
    FILE *fp = fopen(filePath, "r");
    if (!fp) {
        printf("Error opening file: %s\n", strerror(errno));
        printf("No users found. Please signup first.\n");
        return LOGIN_FAILED;
    }

    char line[256], fileEmail[100], filePass[20];
    int userFound = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Remove newline if present
        line[strcspn(line, "\n")] = 0;
        
        // Parse the line
        char *token = strtok(line, "|");
        if (token != NULL) {
            strcpy(fileEmail, token);
            token = strtok(NULL, "|");
            if (token != NULL) {
                strcpy(filePass, token);
                
                // Check credentials
                if (strcmp(email, fileEmail) == 0) {
                    userFound = 1;
                    if (strcmp(hashed, filePass) == 0) {
                        fclose(fp);
                        printf("✅ Login successful!\n");
                        return LOGIN_SUCCESS;
                    }
                }
            }
        }
    }
    
    fclose(fp);
    
    if (!userFound) {
        printf("❌ User not found!\n");
        return LOGIN_USER_NOT_FOUND;
    }
    
    printf("❌ Login failed! Incorrect password.\n");
    return LOGIN_FAILED;
}

