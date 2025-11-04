#ifndef MENU_HEADER_H
#define MENU_HEADER_H

#include "../Header/mainHeader.h"

/* Menu Function Declarations */
void displayAuthMenu(void);
void displayCDRMenu(void);
void displaySearchMenu(void);
void handleCustomerBillingMenu(void);
void handleInteroperatorBillingMenu(void);
int handleCDRMenu(void);
void clearInputBuffer(void);
void handleInvalidInput(MenuType menu_type);

#endif /* MENU_HEADER_H */