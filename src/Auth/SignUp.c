#include "../../Header/authHeader.h"

//Simple hash function
static void hashPassword(const char *password, char *hashed) {
    unsigned int sum = 0;
    for (int i = 0; password[i]; i++)
        sum += password[i] * (i + 1);
    sprintf(hashed, "%u", sum); // store as string
}

// Function to validate email format
int validateEmail(const char *email) {
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');

    // Basic structure: must have one '@' before a '.', not at start or end
    if (!at || !dot || at == email || dot <= at + 1 || dot == email + strlen(email) - 1)
        return 0;
    return 1;
}

// Function to validate password requirements
int validatePassword(const char *password) {
    int hasLower = 0, hasUpper = 0, hasDigit = 0, hasSpecial = 0;

    if (strlen(password) < 6)
        return 0;

    for (int i = 0; password[i] != '\0'; i++) {
        if (islower(password[i])) hasLower = 1;
        else if (isupper(password[i])) hasUpper = 1;
        else if (isdigit(password[i])) hasDigit = 1;
        else if (ispunct(password[i])) hasSpecial = 1;
    }

    return (hasLower && hasUpper && hasDigit && hasSpecial);
}

// Check if user already exists
int userExists(const char *email) {
    FILE *fp = fopen("../../Data/Users.txt", "r");
    if (fp == NULL) return 0; // file not exist yet, so user does not exist

    char line[256];
    char fileEmail[100];

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0; // remove newline
        char *token = strtok(line, "|");
        if (token != NULL) strcpy(fileEmail, token);

        if (strcmp(email, fileEmail) == 0) {
            fclose(fp);
            return 1; // user exists
        }
    }

    fclose(fp);
    return 0; // user does not exist
}

// Signup function — saves credentials to Users.txt and returns success
int signupUser() {
    char email[100];
    char password[100];
    char hashed[20];
    FILE *fp;

    // Email input loop
    while (1) {
        printf("Enter Email: ");
        scanf("%s", email);

        // Validate email format
        if (!validateEmail(email)) {
            printf("❌ Invalid email format!\n");
            continue;
        }

        // Check if user already exists
        if (userExists(email)) {
            printf("❌ User already exists! Redirecting to login...\n");
            return SIGNUP_USER_EXISTS;
        }

        break; // valid email & not exists
    }

    // Password input loop
    while (1) {
        printf("Enter Password: ");
        scanf("%s", password); // or use getpass() if available

        if (validatePassword(password)) break;
        else {
            printf("❌ Invalid password! Requirements:\n");
            printf("   - Minimum 6 characters\n");
            printf("   - At least 1 lowercase letter\n");
            printf("   - At least 1 uppercase letter\n");
            printf("   - At least 1 digit\n");
            printf("   - At least 1 special character\n\n");
        }
    }

    // Hash the password
    hashPassword(password, hashed);

    // Save to Users.txt
    fp = fopen("../../Data/Users.txt", "a");
    if (!fp) {
        printf("Error opening Users.txt!\n");
        return SIGNUP_FAILED;
    }
    fprintf(fp, "%s|%s\n", email, hashed);
    fclose(fp);

    printf("✅ Signup successful!\n");
    return SIGNUP_SUCCESS;
}
