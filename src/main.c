#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tags.h"
#include "accessories.h"
#include "expense.h"

int main() {
    int choice;
    eLoad();
    tagLoad();
    cDateLoad();

    do {
        printf("\n--Personal Expense Tracker--\n");
        printf("1. Tag configuration\n");
        printf("2. Expense configuration\n");
        printf("3. Save & exit\n");
        choice =safeIntCheck("Select an option: ");

        switch (choice) {
            case 1: tagSetup(); break;
            case 2: eSetup(); break;
            case 3: eSave(); tagSave(); cDateSave(); break;
            default: printf("Invalid choice. Try again.");
        }
    } while (choice != 3);

    return 0;
}
