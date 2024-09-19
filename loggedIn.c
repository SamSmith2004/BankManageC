#include "utils.h"
#include "bankFunctions.h"

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

bool stayLoggedIn = false;

void viewDetails() {
    printf("Heres your account detials:\n");
    printf("Account ID: %d\n", account.id);
    printf("Name: %s\n", account.name);
    printf("Balance: %.2f\n", account.balance);

    printf("Do you want to see your password? (y/n): ");
    char showPassword[10];
    if (fgets(showPassword, sizeof(showPassword), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    showPassword[strcspn(showPassword, "\n")] = 0;

    if (showPassword[0] == 'y') {
        if (account.password[0] == '\0') return;
        printf("Your password is: %s\n", account.password);
        return;
    } else if (showPassword[0] == 'n') {
        printf("Cancelling\n");
        return;
    } else {
        printf("Invalid input\n");
        return;
    }
    return;
}

void logout() {
    while (1) {
        printf("Are you sure you want to Logout? (y/n): ");
        char choice[3];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            printf("Input error B0\n");
            continue;
        }
        choice[strcspn(choice, "\n")] = 0;

        if (choice[0] == 'n') {
            return;
        } else if (choice[0] == 'y') {
            printf("Logging out...\n");
            stayLoggedIn = false;
            clearScreen();
            return;
        } else {
            printf("Invalid Input. Please try again. \n");
        }
    }
}

void moreOptions() {
    while (1) {
        printf("View details or Logout? (v/l) or 'q' to return to main menu: ");
        char choice[3];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            printf("Input error B0\n");
            continue;
        }
        choice[strcspn(choice, "\n")] = 0;

        if (choice[0] == 'v') {
            viewDetails();
        } else if (choice[0] == 'l') {
            logout();
            if (!stayLoggedIn) break;
        } else if (choice[0] == 'q') {
            return;
        } else {
            printf("Invalid input. Please try again.\n");
        }
    }
}

void deposit() {
    float depositAmount;
    char confirm[10];

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
    char confirm[10];
    printf("How much to Withdraw\n");

    if (scanf("%f", &withdrawAmount) != 1) {
        printf("Input error\n");
        while (getchar() != '\n');  // Clear input buffer
        return;
    }
    while (getchar() != '\n');  // Clear input buffer after scanf

    if (withdrawAmount < 0) {
        printf("Invalid amount\n");
        return;
    }

    if (account.balance - withdrawAmount < 0) {
        printf("Insufficient funds\n");
        return;
    }

    printf("You are withdrawing %.2f. Confirm? (y/n)\n", withdrawAmount);
    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        printf("Input error\n");
        return;
    }
    confirm[strcspn(confirm, "\n")] = 0;  // Remove newline

    if (confirm[0] == 'y') {
        account.balance -= withdrawAmount;
        printf("Your balance is: %.2f\n", account.balance);
        return;
    } else if (confirm[0] == 'n') {
        printf("Cancelling withdrawal\n");
        return;
    } else {
        printf("Invalid input\n");
        return;
    }
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
            clearScreen();
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

void loggedIn(struct Account* currentAccount, struct Account* lastLoggedInAccount, bool isLoggedIn) {
    stayLoggedIn = isLoggedIn;
    account = *currentAccount;
    clearScreen();
    while (stayLoggedIn) {
        printf("--------------------\n");
        printf("View Balance: ENTER 1 ----- Deposit/Withdraw: ENTER 2 ----- More: ENTER 3\n");

        if (!checkInput()) {
            // Clear input buffer after invalid input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;  // If input was invalid, continue the loop
        }
    }
    *lastLoggedInAccount = account;
    *currentAccount = account;

    updateAccount(&account);

    runBankSystem(lastLoggedInAccount);
}
