#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#define SALT_LENGTH 16
#define HASH_LENGTH 32

struct Card {
    long long int cardNumber;
    int date;
    short cvv;
};

struct Account{
    int id;
    char name[20];
    unsigned char salt[SALT_LENGTH];
    unsigned char password_hash[HASH_LENGTH];
    long long int balance;
    struct Card card;
};

void clearScreen();
void loggedIn(struct Account* currentAccount, struct Account* lastLoggedInAccount, bool isLoggedIn, struct Account* accounts);

#endif
