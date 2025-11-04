#include "../../Header/authHeader.h"

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
    FILE *fp = fopen("../../Data/Users.txt", "r");
    if (!fp) {
        printf("No users found. Please signup first.\n");
        return 0;
    }
    char line[256], fileEmail[100], filePass[20];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, "|");
        if (token != NULL) strcpy(fileEmail, token);
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(filePass, token);
        if (strcmp(email, fileEmail) == 0 && strcmp(hashed, filePass) == 0) {
            fclose(fp);
            printf("✅ Login successful!\n");
            return 1;
        }
    }
    fclose(fp);
    printf("❌ Login failed! Incorrect email or password.\n");
    return 0;
}

