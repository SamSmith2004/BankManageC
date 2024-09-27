#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

struct Card {
    long long int cardNumber;
    int date;
    short cvv;
};

struct Account{
    int id;
    char name[20];
    char password[20];
    long long int balance;
    struct Card card;
};

void clearScreen();
void loggedIn(struct Account* currentAccount, struct Account* lastLoggedInAccount, bool isLoggedIn, struct Account* accounts);

#endif
