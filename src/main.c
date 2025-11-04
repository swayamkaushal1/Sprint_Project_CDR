#include "../Header/mainHeader.h"

void displayAuthMenu(void) {
    printf("\n=== Authentication Menu ===\n");
    printf("1. Signup\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void displayCDRMenu(void) {
    printf("\n=== CDR Processing System ===\n");
    printf("1. Process the CDR data\n");
    printf("2. Print/Search the CDR data\n");
    printf("3. Logout\n");
    printf("Enter your choice: ");
}

void clearInputBuffer(void) {
    while (getchar() != '\n');
}

void handleInvalidInput(MenuType menu_type) {
    clearInputBuffer();
    printf("\nInvalid input! Please enter a number");
    if (menu_type == MENU_AUTH) {
        printf(" between 1 and 3");
    }
    printf(".\n");
}

void getFilePaths(char *input_file, char *cb_output, char *isob_output) {
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    
    snprintf(input_file, MAX_FILE_PATH, "%s\\Data\\data.cdr", currentPath);
    snprintf(cb_output, MAX_FILE_PATH, "%s\\Output\\CB.txt", currentPath);
    snprintf(isob_output, MAX_FILE_PATH, "%s\\Output\\ISOB.txt", currentPath);
    
    // Ensure Output directory exists
    char outputDir[MAX_PATH_LENGTH];
    snprintf(outputDir, MAX_PATH_LENGTH, "%s\\Output", currentPath);
    CreateDirectory(outputDir, NULL);
}

int handleCDRMenu(void) {
    int choice;
    int running = 1;
    char input_file[MAX_FILE_PATH];
    char cb_output[MAX_FILE_PATH];
    char isob_output[MAX_FILE_PATH];
    
    getFilePaths(input_file, cb_output, isob_output);

    while (running) {
        displayCDRMenu();
        if (scanf("%d", &choice) != 1) {
            handleInvalidInput(MENU_CDR);
            continue;
        }

        switch ((CDRMenuChoice)choice) {
            case CDR_PROCESS:
                printf("\nProcessing CDR data...\n");
                if (processCDRWithThreads(input_file, cb_output, isob_output) == 0) {
                    printf("✅ CDR data processed successfully!\n");
                } else {
                    printf("❌ Error occurred while processing CDR data.\n");
                }
                break;

            case CDR_SEARCH:
                printf("\nℹ️ Feature coming soon: Print/Search CDR data\n");
                break;

            case CDR_LOGOUT:
                printf("\n👋 Logging out from CDR system...\n");
                running = 0;
                break;

            default:
                handleInvalidInput(MENU_CDR);
        }
    }
    return 0;
}

int main(void) {
    int choice;
    int running = 1;
    int loginStatus = 0;

    while (running) {
        displayAuthMenu();
        if (scanf("%d", &choice) != 1) {
            handleInvalidInput(MENU_AUTH);
            continue;
        }

        switch ((AuthMenuChoice)choice) {
            case AUTH_SIGNUP:
                {
                    int result = signupUser();
                    if (result == SIGNUP_SUCCESS) {
                        printf("\nWould you like to login now? (1 for yes, 0 for no): ");
                        int loginNow;
                        if (scanf("%d", &loginNow) == 1 && loginNow) {
                            loginStatus = loginUser();
                            if (loginStatus) {
                                handleCDRMenu();
                            }
                        } else {
                            clearInputBuffer();
                        }
                    }
                }
                break;
            
            case AUTH_LOGIN:
                loginStatus = loginUser();
                if (loginStatus) {
                    handleCDRMenu();
                }
                break;
            
            case AUTH_EXIT:
                printf("\n👋 Thank you for using the system. Goodbye!\n");
                running = 0;
                break;
            
            default:
                handleInvalidInput(MENU_AUTH);
        }
    }

    return 0;
}
