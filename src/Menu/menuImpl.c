#include "../../Header/menuHeader.h"

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

void displaySearchMenu(void) {
    printf("\nSelect Billing Type:\n");
    printf("1. Customer Billing\n");
    printf("2. Interoperator Settlement Billing\n");
    printf("3. Exit\n");
    printf("Choice: ");
}

void handleCustomerBillingMenu(void) {
    char cb_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    snprintf(cb_path, MAX_FILE_PATH, "%s\\Output\\CB.txt", currentPath);

    printf("\nCustomer Billing Options:\n");
    printf("1. Search for MSISDN to get the required result on the user screen\n");
    printf("2. Get the all the content of the processed data for Customer Billing in the file called CB.txt\n");
    printf("3. Exit\n");
    printf("Choice: ");
    
    int custChoice;
    scanf("%d", &custChoice);
    
    switch (custChoice) {
        case 1: {
            clearInputBuffer();
            long msisdn;
            printf("\nEnter MSISDN (phone number) to search: ");
            if (scanf("%ld", &msisdn) == 1) {
                search_msisdn(cb_path, msisdn);
            } else {
                printf("Invalid MSISDN format. Please enter a valid number.\n");
                clearInputBuffer();
            }
            break;
        }
        case 2:
            printf("\nDisplaying contents of CB.txt:\n\n");
            display_customer_billing_file(cb_path);
            break;
        case 3:
            printf("\nReturning to search menu...\n");
            break;
        default:
            printf("\nInvalid choice!\n");
            clearInputBuffer();
    }
}

void handleInteroperatorBillingMenu(void) {
    char isob_path[MAX_FILE_PATH];
    char currentPath[MAX_PATH_LENGTH];
    GetCurrentDirectory(MAX_PATH_LENGTH, currentPath);
    snprintf(isob_path, MAX_FILE_PATH, "%s\\Output\\ISOB.txt", currentPath);

    printf("\nInteroperator Settlement Billing Options:\n");
    printf("1. Search for Brand Name/Operator ID identified by Operator MMC/MNC to get the required result on the user screen\n");
    printf("2. Get the all the content of the processed data for Interoperator Settlement Billing in the file called ISOB.txt\n");
    printf("3. Exit\n");
    printf("Choice: ");
    
    int interopChoice;
    scanf("%d", &interopChoice);
    
    switch (interopChoice) {
        case 1: {
            clearInputBuffer();
            char operator_input[50];
            printf("\nEnter Brand Name or Operator MMC/MNC to search: ");
            if (fgets(operator_input, sizeof(operator_input), stdin) != NULL) {
                // Remove trailing newline
                operator_input[strcspn(operator_input, "\n")] = 0;
                search_operator(isob_path, operator_input);
            } else {
                printf("Error reading input.\n");
            }
            break;
        }
        case 2:
            printf("\nDisplaying contents of ISOB.txt:\n\n");
            display_file(isob_path);
            break;
        case 3:
            printf("\nReturning to search menu...\n");
            break;
        default:
            printf("\nInvalid choice!\n");
            clearInputBuffer();
    }
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
                {
                    int searchChoice;
                    displaySearchMenu();
                    
                    if (scanf("%d", &searchChoice) != 1) {
                        clearInputBuffer();
                        printf("Invalid input!\n");
                        break;
                    }

                    switch (searchChoice) {
                        case 1:
                            handleCustomerBillingMenu();
                            break;

                        case 2:
                            handleInteroperatorBillingMenu();
                            break;

                        case 3:
                            printf("\nReturning to main menu...\n");
                            break;

                        default:
                            printf("\nInvalid choice!\n");
                    }
                }
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