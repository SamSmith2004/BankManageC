#include "utils.h"
#include "bankFunctions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Account account;

bool stayLoggedIn = false;

void transfer() {
    if (account.balance == 0) {
        clearScreen();
        printf("You have no balance to transfer\n");
        return;
    }

    printf("Enter the account name to transfer to: ");
    char choice[20];
    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }

    choice[strcspn(choice, "\n")] = 0;
    if (strcmp(choice, account.name) == 0) {
        clearScreen();
        printf("You cannot transfer to yourself\n");
        return;
    }

    struct Account transferAccount;
    bool accountFound = false;
    for (int i = 0; i < 100; i++) {
        if (strcmp(accounts[i].name, choice) == 0) {
            transferAccount = accounts[i];
            accountFound = true;
            break;
        }
    }

    if (!accountFound) {
        clearScreen();
        printf("Account not found\n");
        return;
    }


    printf("Enter the amount to transfer: ");
    long long int amount;
    if (scanf("%lld", &amount) != 1) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    while (getchar() != '\n' && getchar() != EOF); // clear input buffer

    if (amount > account.balance) {
        clearScreen();
        printf("Insufficient balance\n");
        return;
    }
    if (amount < 0) {
        clearScreen();
        printf("Invalid amount\n");
        return;
    }

    char password[20];
    printf("Enter your password to continue: ");

    if (fgets(password, sizeof(password), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    password[strcspn(password, "\n")] = 0;

    unsigned char hash[HASH_LENGTH];
    hash_password(password, account.salt, hash);
    if (memcmp(hash, account.password_hash, HASH_LENGTH) != 0) {
        clearScreen();
        printf("Incorrect password\n");
        return;
    }

    printf("Are you sure you want to transfer %.2f to %s? (y/n)\n", amount / 100.0, choice);
    char confirm[3];
    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    confirm[strcspn(confirm, "\n")] = 0;

    if (confirm[0] == 'y') {
        account.balance -= amount;
        transferAccount.balance += amount;
        clearScreen();
        printf("Transfer complete! Your balance is %.2f\n", account.balance / 100.0);
        updateAccount(&account);
        updateAccount(&transferAccount);
    } else if (confirm[0] == 'n') {
        clearScreen();
        printf("Cancelling\n");
        return;
    } else {
        clearScreen();
        printf("Invalid input\n");
        return;
    }

    return;
}

void viewDetails() {
    printf("Heres your account detials:\n");
    printf("Account ID: %d\n", account.id);
    printf("Name: %s\n", account.name);
    printf("Balance: %.2f\n", account.balance / 100.0);

    printf("Do you want to see your card details? (y/n): ");
    char showCard[10];
    if (fgets(showCard, sizeof(showCard), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    showCard[strcspn(showCard, "\n")] = 0;

    if (showCard[0] == 'y') {
        if (account.card.cardNumber) {
            printf("Card Number: %lld\n", account.card.cardNumber);
            printf("Expiry Date: %d/%d\n", account.card.date / 100, account.card.date % 100);
            printf("CVV: %d\n", account.card.cvv);
        } else {
            clearScreen();
            printf("Error: You do not have a card. Please contact support at XXXXXXX\n");
        }
        return;
    } else if (showCard[0] == 'n') {
        clearScreen();
        printf("Cancelling\n");
        return;
    } else {
        clearScreen();
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
            clearScreen();
            printf("Input error B0\n");
            continue;
        }
        choice[strcspn(choice, "\n")] = 0;

        if (choice[0] == 'n') {
            clearScreen();
            return;
        } else if (choice[0] == 'y') {
            printf("Logging out...\n");
            stayLoggedIn = false;
            clearScreen();
            return;
        } else {
            clearScreen();
            printf("Invalid Input. Please try again. \n");
        }
    }
}

void deleteAccount() {
    char password[20];
    printf("Enter your password to continue: ");

    if (fgets(password, sizeof(password), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    password[strcspn(password, "\n")] = 0;

    unsigned char hash[HASH_LENGTH];
    hash_password(password, account.salt, hash);
    if (memcmp(hash, account.password_hash, HASH_LENGTH) != 0) {
        clearScreen();
        printf("Incorrect password\n");
        return;
    }

    while (1) {
        printf("Are you sure you want to delete your account, this is permanent? (y/n): ");
        char choice[3];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            clearScreen();
            printf("Input error B0\n");
            continue;
        }
        choice[strcspn(choice, "\n")] = 0;

        if (choice[0] == 'n') {
            clearScreen();
            printf("Cancelling\n");
            return;
        } else if (choice[0] == 'y') {
            printf("deleting...\n");

            for (int i = 0; i < 100; i++) {
                if (accounts[i].id == account.id) {
                    memset(&accounts[i], 0, sizeof(struct Account));
                    break;
                }
            }
            memset(&account, 0, sizeof(struct Account));

            stayLoggedIn = false;
            clearScreen();
            return;
        } else {
            clearScreen();
            printf("Invalid Input. Please try again. \n");
        }
    }
}

void moreOptions() {
    while (1) {
        printf("View details, Remove Account, Logout?(1,2,3) or 'q' to return to main menu: ");
        char choice[3];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            clearScreen();
            printf("Input error B0\n");
            continue;
        }
        choice[strcspn(choice, "\n")] = 0;

        if (choice[0] == '1') {
            viewDetails();
        } else if (choice[0] == '2') {
            deleteAccount();
            if (!stayLoggedIn) break;
        } else if (choice[0] == '3') {
            logout();
            if (!stayLoggedIn) break;
        } else if (choice[0] == 'q') {
            clearScreen();
            return;
        } else {
            clearScreen();
            printf("Invalid input. Please try again.\n");
        }
    }
}

void deposit() {
    long long int depositAmount;
    char confirm[10];

    printf("How much to Deposit?(in cents)\n");
    if (scanf("%lld", &depositAmount) != 1) {
        clearScreen();
        printf("Input error\n");
        while (getchar() != '\n');  // Clear input buffer
        return;
    }
    while (getchar() != '\n');  // Clear input buffer after scanf

    if (depositAmount < 0) {
        clearScreen();
        printf("Invalid amount\n");
        return;
    }

    if (account.balance > LLONG_MAX - depositAmount) { // Check for overflow
        clearScreen();
        printf("Error: Deposit amount too large\n");
        return;
    }

    short cvv;
    printf("Enter your CVV: ");
    if (scanf("%hd", &cvv) != 1) {
        printf("Input error [C1]\n");
        clearScreen();
        while (getchar() != '\n');  // Clear input buffer
        return;
    }
    while (getchar() != '\n'); // Clear input buffer after scanf

    if (cvv != account.card.cvv) {
        clearScreen();
        printf("Invalid CVV\n");
        return;
    }

    printf("You are depositing %.2f Confirm? (y/n)\n", depositAmount / 100.0);

    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    confirm[strcspn(confirm, "\n")] = 0;  // Remove newline

    if (confirm[0] == 'y') {
        account.balance += depositAmount;
        clearScreen();
        printf("Your balance is: %.2f\n", account.balance / 100.0);
        return;
    } else if (confirm[0] == 'n') {
        clearScreen();
        printf("Cancelling deposit\n");
        return;
    } else {
        clearScreen();
        printf("Invalid input\n");
        return;
    }
}

void withdraw() {
    long long int withdrawAmount;
    printf("How much to Withdraw (in cents)\n");

    if (scanf("%lld", &withdrawAmount) != 1) {
        clearScreen();
        printf("Input error\n");
        while (getchar() != '\n');  // Clear input buffer
        return;
    }
    while (getchar() != '\n');  // Clear input buffer after scanf

    if (withdrawAmount < 0) {
        clearScreen();
        printf("Invalid amount\n");
        return;
    }

    if (account.balance < withdrawAmount) {
        clearScreen();
        printf("Insufficient funds\n");
        return;
    }

    char password[20];
    printf("Enter your password to confirm: ");

    if (fgets(password, sizeof(password), stdin) == NULL) {
        clearScreen();
        printf("Input error\n");
        return;
    }
    password[strcspn(password, "\n")] = 0;

    unsigned char hash[HASH_LENGTH];
    hash_password(password, account.salt, hash);
    if (memcmp(hash, account.password_hash, HASH_LENGTH) != 0) {
        clearScreen();
        printf("Incorrect password\n");
        return;
    }

    char confirm[10];
    printf("You are withdrawing %.2f Confirm? (y/n)\n", withdrawAmount / 100.0);
    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        clearScreen();
        printf("Input error [C2]\n");
        return;
    }
    confirm[strcspn(confirm, "\n")] = 0;  // Remove newline

    if (confirm[0] == 'y') {
        account.balance -= withdrawAmount;
        clearScreen();
        printf("Your balance is: %.2f\n", account.balance / 100.0);
        return;
    } else if (confirm[0] == 'n') {
        clearScreen();
        printf("Cancelling withdrawal\n");
        return;
    } else {
        clearScreen();
        printf("Invalid input\n");
        return;
    }
}
void handleDepositWithdraw() {
    while (1) {
        printf("Deposit or Withdraw? (d/w) or 'q' to return to main menu: ");
        char choice[3];
        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            clearScreen();
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
            transfer();
            break;
        case '2':
            handleDepositWithdraw();
            break;
        case '3':
            moreOptions();
            break;
        default:
            printf("Invalid input. Press Enter to try again.\n");
            return false;
    }
    return true;
}

void loggedIn(struct Account* currentAccount, struct Account* lastLoggedInAccount, bool isLoggedIn, struct Account* accounts) {
    stayLoggedIn = isLoggedIn;
    account = *currentAccount;
    clearScreen();
    while (stayLoggedIn) {
        printf("--------------------\n");
        printf("Transfer: ENTER 1 ----- Deposit/Withdraw: ENTER 2 ----- More: ENTER 3\n");

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
