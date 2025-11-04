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
    printf("\nCustomer Billing Options:\n");
    printf("1. Search for MSISDN to get the required result on the user screen\n");
    printf("2. Get the all the content of the processed data for Customer Billing in the file called CB.txt\n");
    printf("3. Exit\n");
    printf("Choice: ");
    
    int custChoice;
    scanf("%d", &custChoice);
    if (custChoice == 1) {
        printf("\nFeature coming soon: MSISDN search\n");
    } else if (custChoice == 2) {
        printf("\nFeature coming soon: Display CB.txt contents\n");
    }
}

void handleInteroperatorBillingMenu(void) {
    printf("\nInteroperator Settlement Billing Options:\n");
    printf("1. Search for Brand Name/Operator ID identified by Operator MMC/MNC to get the required result on the user screen\n");
    printf("2. Get the all the content of the processed data for Interoperator Settlement Billing in the file called IOSB.txt\n");
    printf("3. Exit\n");
    printf("Choice: ");
    
    int interopChoice;
    scanf("%d", &interopChoice);
    if (interopChoice == 1) {
        printf("\nFeature coming soon: Operator search\n");
    } else if (interopChoice == 2) {
        printf("\nFeature coming soon: Display IOSB.txt contents\n");
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