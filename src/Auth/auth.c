#include "../../Header/authHeader.h"

// External function declarations from Login.c and SignUp.c
extern int loginUser();
extern int signupUser();

void displayMenu(int isLoggedIn) {
    printf("\n=== Authentication Menu ===\n");
    if (!isLoggedIn) {
        printf("1. Signup\n");
        printf("2. Login\n");
        printf("3. Exit\n");
    } else {
        printf("1. Logout\n");
        printf("2. Exit\n");
    }
    printf("Enter your choice: ");
}

int main() {
    int choice;
    int isLoggedIn = 0;
    int running = 1;

    while (running) {
        displayMenu(isLoggedIn);
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        if (!isLoggedIn) {
            switch (choice) {
                case 1: // Signup
                    {
                        int result = signupUser();
                        if (result == SIGNUP_SUCCESS) {
                            printf("Would you like to login now? (1 for yes, 0 for no): ");
                            int loginNow;
                            scanf("%d", &loginNow);
                            if (loginNow) {
                                isLoggedIn = loginUser();
                            }
                        }
                    }
                    break;
                
                case 2: // Login
                    isLoggedIn = loginUser();
                    break;
                
                case 3: // Exit
                    printf("Thank you for using the system. Goodbye!\n");
                    running = 0;
                    break;
                
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } else {
            switch (choice) {
                case 1: // Logout
                    printf("Logged out successfully!\n");
                    isLoggedIn = 0;
                    break;
                
                case 2: // Exit
                    printf("Thank you for using the system. Goodbye!\n");
                    running = 0;
                    break;
                
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        }
    }

    return 0;
}
