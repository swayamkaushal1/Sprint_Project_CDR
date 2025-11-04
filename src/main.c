#include "../Header/mainHeader.h"
#include "../Header/menuHeader.h"
#include "../Header/utilityHeader.h"

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