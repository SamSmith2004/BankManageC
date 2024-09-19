#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

struct Account{
    int id;
    char name[20];
    char password[20];
    float balance;
};

void loggedIn(struct Account* currentAccount, struct Account* lastLoggedInAccount, bool isLoggedIn);

#endif
