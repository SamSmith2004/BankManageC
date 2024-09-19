#ifndef BANK_FUNCTIONS_H
#define BANK_FUNCTIONS_H

#include "utils.h"
#include <stdbool.h>

struct Account makeAccount(char accountName[20]);
_Bool checkLogin(struct Account* lastLoggedInAccount);
void initState(struct Account* lastLoggedInAccount);
void runBankSystem(struct Account* lastLoggedInAccount);
void updateAccount(struct Account* account);

#endif
