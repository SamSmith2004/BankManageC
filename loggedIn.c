#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account account;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void moreOptions() {
    printf("More options\n");
    return;
}

void deposit() {
    float depositAmount;
    char confirm[10];  // Increased buffer size

    printf("How much to Deposit?\n");
    if (scanf("%f", &depositAmount) != 1) {
        printf("Input error\n");
        while (getchar() != '\n');  // Clear input buffer
        return;
    }
    while (getchar() != '\n');  // Clear input buffer after scanf

    if (depositAmount < 0) {
        printf("Invalid amount\n");
        return;
    }

    printf("You are depositing %.2f. Confirm? (y/n)\n", depositAmount);
    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    confirm[strcspn(confirm, "\n")] = 0;  // Remove newline

    if (confirm[0] == 'y') {
        account.balance += depositAmount;
        printf("Your balance is: %.2f\n", account.balance);
        return;
    } else if (confirm[0] == 'n') {
        printf("Cancelling deposit\n");
        return;
    } else {
        printf("Invalid input\n");
        return;
    }
}

void withdraw() {
    float withdrawAmount;
    printf("How much to Withdraw\n");
}
void handleDepositWithdraw() {
    while (1) {
        printf("Deposit or Withdraw? (d/w) or 'q' to return to main menu: ");
        char choice[3];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            printf("Input error B0\n");
            continue;
        }
        choice[strcspn(choice, "\n")] = 0;

        if (choice[0] == 'd') {
            deposit();
        } else if (choice[0] == 'w') {
            withdraw();
        } else if (choice[0] == 'q') {
            return; // Return to main menu
        } else {
            printf("Invalid input. Please try again.\n");
        }
    }
}

bool checkInput() {
    char input[10];
    printf("Enter your choice: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
        return false;
    }
    input[strcspn(input, "\n")] = 0;  // Remove newline

    switch(input[0]) {
        case '1':
            printf("Your balance is: %.2f\n", account.balance);
            break;
        case '2':
            handleDepositWithdraw();
            break;
        case '3':
            moreOptions();
            break;
        default:
            return false;
    }
    return true;
}

void loggedIn(struct Account currentAccount, bool isLoggedIn) {
    account = currentAccount;
    clearScreen();
    while (isLoggedIn) {
        printf("--------------------\n");
        printf("View Balance: ENTER 1 ----- Deposit/Withdraw: ENTER 2 ----- More: ENTER 3\n");

        if (!checkInput()) {
            // Clear input buffer after invalid input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;  // If input was invalid, continue the loop
        }
        // Add logout logic here
    }
}
