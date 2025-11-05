#include "../../Header/menuHeader.h"
#include "../../Header/utilityHeader.h"

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
    get_current_dir(currentPath, MAX_PATH_LENGTH);

#ifdef _WIN32
    snprintf(cb_path, MAX_FILE_PATH, "%s\\Output\\CB.txt", currentPath);
#else
    snprintf(cb_path, MAX_FILE_PATH, "%s/Output/CB.txt", currentPath);
#endif

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
    // Get the ISOB file path using our utility function
    const char* isob_path = getISOBFilePath();
    if (!isob_path) {
        printf("Error getting ISOB file path\n");
        return;
    }

    // Check if file exists
    FILE* test_file = fopen(isob_path, "r");
    if (!test_file) {
        printf("\nInteroperator Settlement Billing file not found.\n");
        printf("Please process the CDR data first using option 1 from the main menu.\n");
        return;
    }
    fclose(test_file);

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