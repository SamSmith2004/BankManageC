#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

struct Account accounts[100];
struct Account currentAccount;

int accountCount = 0;
bool isLoggedIn = false;

struct Account makeAccount(char accountName[20]) {
    struct Account current = {0};
    if (accountCount > 98) {
        printf("Account limit reached\n");
        current.id = -1; // -1 indicates an Error
        return current;
    }

    current.id = accountCount;
    strncpy(current.name, accountName, sizeof(current.name) - 1); // Copy at most sizeof(current.name) - 1 characters
    current.name[sizeof(current.name) - 1] = '\0';  // Ensure null-termination
    current.balance = 0.0;

    accounts[accountCount] = current;
    accountCount++;
    currentAccount = current;
    return current;
}

_Bool checkLogin() {
    char input[2];
    if (scanf("%1s", input) == 1) {
        if (input[0] == '1') {
            printf("Enter your name\n");
            char name[20];
            if (scanf("%19s", name) != 1) {
                printf("Input error\n");
                return 0;
            }
            if (strlen(name) == 19) {
                char c = getchar();
                if (c != ' ' && c != '\n' && c != EOF) {
                    // EOF is the end of file
                    printf("Name too long\n");
                    // Clear the input buffer
                    while (getchar() != '\n' && getchar() != EOF);
                    return 0;
                }
            }
            struct Account account = makeAccount(name);
            if (account.id == -1) {
                printf("Failed to create account due to account limit.\n");
            } else {
                printf("Account created successfully with ID: %d\n", account.id);
            }
            return 1;
        } else if (input[0] == '2') {
            printf("Enter your name\n");
            char name[20];
            if (scanf("%19s", name) != 1) {
                printf("Input error\n");
                return 0;
            }
            if (strlen(name) == 19) {
                char c = getchar();
                if (c != ' ' && c != '\n' && c != EOF) {
                    printf("Name too long\n");
                    // Clear the input buffer
                    while (getchar() != '\n' && getchar() != EOF);
                    return 0;
                }
            }
            for (int i = 0; i < accountCount; i++) {
                if (strcmp(accounts[i].name, name) == 0) { // Cannot compare strings with ==
                    return 1;
                }
            }
            printf("Account not found\n");
            return 0;
        }
    } else {
        printf("Invalid input\n");
        return 0;
    }
    return 0;
}

void initState() {
    printf("Create an account: ENTER 1 ----- Sign in: ENTER 2\n");

    _Bool found = checkLogin();
    if (found) {
        char buffer[40];
        snprintf(buffer, sizeof(buffer), "Welcome back %s\n", currentAccount.name);
        printf("%s", buffer);
    } else {
        initState();
    }
}

int main() {
    printf("--------------------\n");
    printf("Welcome to C Bank\n");

    initState();
    isLoggedIn = true;
    loggedIn(currentAccount, isLoggedIn);

    return 0;
}
