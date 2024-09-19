#include "bankFunctions.h"
struct Account lastLoggedInAccount = {0};

int main() {
    struct Account lastLoggedInAccount = {0};
    runBankSystem(&lastLoggedInAccount);
    return 0;
}
