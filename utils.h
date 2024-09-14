#include <stdbool.h>
#ifndef UTILS_H
#define UTILS_H

struct Account{
    int id;
    char name[20];
    float balance;
};

void loggedIn(struct Account currentAccount, bool isLoggedIn);

#endif
