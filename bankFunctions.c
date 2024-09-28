#include "bankFunctions.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define ITERATIONS 10000

void hash_password(const char* password, const unsigned char* salt, unsigned char* hash) {
    PKCS5_PBKDF2_HMAC(password, strlen(password), salt, SALT_LENGTH, ITERATIONS,
                      EVP_sha256(), HASH_LENGTH, hash);
}

struct Account accounts[100];
struct Account currentAccount;
int accountCount = 0;
bool isLoggedIn = false;

// Checks if it's an uppercase letter (ASCII values 65 to 90)
// adds 32 to its ASCII value. This converts it to the corresponding lowercase
void toLowercase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

int digit_count(int number) {
    if (number == 0) return 1;  // Special case for 0

    int count = 0;
    int absolute = abs(number);  // Handle negative numbers

    while (absolute != 0) {
        absolute /= 10; // Remove the last digit
        count++;
    }

    return count;
}

int luhnCheck(long long int number) {
    int sum = 0;
    int isEven = 0;
    while (number > 0) {
        int digit = number % 10;
        if (isEven) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
        isEven = !isEven;
        number /= 10;
    }
    return (sum % 10 == 0);
}

void updateAccount(struct Account* account) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].id == account->id) {
            accounts[i] = *account;
            break;
        }
    }
}

struct Account makeAccount(char accountName[20], char password[20]) {
    struct Account current = {0};
    if (accountCount > 98) {
        clearScreen();
        printf("Account limit reached\n");
        current.id = -1; // -1 indicates an Error
        return current;
    }

    current.id = accountCount;
    strncpy(current.name, accountName, sizeof(current.name) - 1);
    // Ideally hash the password before storing it
    current.name[sizeof(current.name) - 1] = '\0';  // Ensure null-termination

    unsigned char salt[SALT_LENGTH];
    unsigned char hash[HASH_LENGTH];
    RAND_bytes(salt, SALT_LENGTH);
    hash_password(password, salt, hash);

    // Store the salt and hash
    memcpy(current.salt, salt, SALT_LENGTH);
    memcpy(current.password_hash, hash, HASH_LENGTH);

    current.balance = 0;

    accounts[accountCount] = current;
    accountCount++;
    currentAccount = current;
    return current;
}

struct Card setCard() {
    struct Card card = {0};
    card.cardNumber = -1;
    long long int cardNumber;
    int expiryDate = 0;
    char expiryDateStr[5];
    short cvv;

    printf("Enter your card number\n");
    if (scanf("%lli", &cardNumber) == 1) {
        if (digit_count(cardNumber) > 18 || digit_count(cardNumber) < 8) {
            clearScreen();
            printf("Invalid card number\n");
            return card;
        }
        // if (!luhnCheck(cardNumber)) {
        //     printf("Invalid card number\n");
        //     return card;
        // }
    } else {
        clearScreen();
        printf("Input error\n");
        return card;
    }

    printf("Enter your expiry date as MMYY\n");
    if (scanf("%4s", expiryDateStr) == 1) {
        if (strlen(expiryDateStr) != 4) {
            clearScreen();
            printf("Invalid expiry date\n");
            return card;
        }
        int month = (expiryDateStr[0] - '0') * 10 + (expiryDateStr[1] - '0');
        int year = (expiryDateStr[2] - '0') * 10 + (expiryDateStr[3] - '0');
        if (month < 1 || month > 12 || year < 23) {
            clearScreen();
            printf("Invalid expiry date\n");
            return card;
        }
        expiryDate = month * 100 + year;
    } else {
        clearScreen();
        printf("Input error\n");
        return card;
    }

    printf("Enter your CVV\n");
    if (scanf("%hd", &cvv) == 1) {
        if (digit_count(cvv) < 3 || digit_count(cvv) > 4) {
            clearScreen();
            printf("Invalid CVV\n");
            return card;
        }
    } else {
        clearScreen();
        printf("Input error\n");
        return card;
    }
    card.cardNumber = cardNumber;
    card.date = expiryDate;
    card.cvv = cvv;
    return card;
}

_Bool checkLogin(struct Account* lastLoggedInAccount) {
    char input[2];
    if (scanf("%1s", input) == 1) {
        if (input[0] == '1') {
            printf("Enter your name\n");
            char name[20];
            if (scanf("%19s", name) != 1) {
                clearScreen();
                printf("Input error\n");
                return 0;
            }
            if (strlen(name) == 19) {
                char c = getchar();
                if (c != ' ' && c != '\n' && c != EOF) {
                    // EOF is the end of file
                    clearScreen();
                    printf("Name too long\n");
                    // Clear the input buffer
                    while (getchar() != '\n' && getchar() != EOF);
                    return 0;
                }
            }

            for (int i = 0; i < 100 ;i++) {
                toLowercase(name);
                if (strcmp(accounts[i].name, name) == 0) {
                    clearScreen();
                    printf("Account already exists\n");
                    return 0;
                }
            }

            char password[20];
            printf("Enter your password\n");
            if (scanf("%19s", password) != 1) {
                clearScreen();
                printf("Input error\n");
                return 0;
            }

            if (strlen(password) == 19) {
                char c = getchar();
                if (c != ' ' && c != '\n' && c != EOF) { // EOF is the end of file
                    clearScreen();
                    printf("Name too long\n");
                    while (getchar() != '\n' && getchar() != EOF); // Clear the input buffer
                    return 0;
                }
            }

            struct Card card = setCard();
            if (card.cardNumber == -1) {
                clearScreen();
                printf("Failed to create account due to card error. 01\n");
                return 0;
            }

            struct Account account = makeAccount(name, password);
            account.card = card;
            currentAccount = account;
            accounts[account.id] = account;

            if (account.id == -1) {
                clearScreen();
                printf("Failed to create account due to account limit.\n");
            } else {
                clearScreen();
                printf("Account created successfully with ID: %d\n", account.id);
            }

            if (account.card.cardNumber == 0 || account.card.cvv == 0 || account.card.date == 0) {
                clearScreen();
                printf("Failed to create account due to card error.02\n");
                return 0;
            }

            return 1;
        } else if (input[0] == '2') {
            printf("Enter your name\n");
            char name[20];
            if (scanf("%19s", name) != 1) {
                clearScreen();
                printf("Input error\n");
                return 0;
            }
            if (strlen(name) == 19) {
                char c = getchar();
                if (c != ' ' && c != '\n' && c != EOF) {
                    clearScreen();
                    printf("Name too long\n");
                    // Clear the input buffer
                    while (getchar() != '\n' && getchar() != EOF);
                    return 0;
                }
            }

            if (lastLoggedInAccount->id != 0 && strcmp(lastLoggedInAccount->name, name) == 0) {
                // If the last logged in account is the same as the current account

                char password[20];
                printf("Enter your password\n");
                if (scanf("%19s", password) != 1) {
                    clearScreen();
                    printf("Input error\n");
                    return 0;
                }

                if (strlen(password) == 19) {
                    char c = getchar();
                    if (c != ' ' && c != '\n' && c != EOF) {
                        clearScreen();
                        printf("Password too long\n");
                        while (getchar() != '\n' && getchar() != EOF);
                        return 0;
                    }
                }

                unsigned char hash[HASH_LENGTH];
                hash_password(password, lastLoggedInAccount->salt, hash);
                if (memcmp(hash, lastLoggedInAccount->password_hash, HASH_LENGTH) == 0) {
                    clearScreen();
                    printf("Logged in successfully\n");
                    currentAccount = *lastLoggedInAccount;
                    return 1;
                } else {
                    clearScreen();
                    printf("Incorrect password\n");
                    return 0;
                }
            }

            for (int i = 0; i < accountCount; i++) {
                if (strcmp(accounts[i].name, name) == 0) { // Cannot compare strings with ==
                    char password[20];
                    printf("Enter your password\n");
                    if (scanf("%19s", password) != 1) {
                        clearScreen();
                        printf("Input error\n");
                        return 0;
                    }

                    unsigned char hash[HASH_LENGTH];
                    hash_password(password, accounts[i].salt, hash);

                    if (memcmp(hash, accounts[i].password_hash, HASH_LENGTH) == 0) {
                        clearScreen();
                        printf("Logged in successfully\n");
                        currentAccount = accounts[i];
                        return 1;
                    } else {
                        printf("Incorrect password\n");
                        return 0;
                    }
                    currentAccount = accounts[i];
                    return 1;
                }
            }
            clearScreen();
            printf("Account not found\n");
            return 0;
        }
    } else {
        clearScreen();
        printf("Invalid input\n");
        return 0;
    }
    return 0;
}

void initState(struct Account* lastLoggedInAccount) {
    printf("Create an account: ENTER 1 ----- Sign in: ENTER 2\n");

    _Bool found = checkLogin(lastLoggedInAccount);
    if (found) {
        char buffer[40];
        clearScreen();
        snprintf(buffer, sizeof(buffer), "Welcome back %s\n", currentAccount.name);
        printf("%s", buffer);
    } else {
        initState(lastLoggedInAccount);
    }
}

void runBankSystem(struct Account* lastLoggedInAccount) {
    printf("--------------------\n");
    printf("Welcome to C Bank\n");

    initState(lastLoggedInAccount);
    isLoggedIn = true;

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // clear input buffer
    loggedIn(&currentAccount, lastLoggedInAccount, isLoggedIn, accounts);
}
