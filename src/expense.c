#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expense.h"
#include "accessories.h"
#include "tags.h"

//SAVING AND LOADING

void eSave(){ //day, month, year, tag, money
    FILE *fp=fopen("expenses.txt","w");
    if (fp==NULL) {printf("\nExpenses failed to save"); return;}
    for (int i = 0; i < eCount; i++) {
        // Save year, month, day as integers
        fprintf(fp, "%02d-%02d-%04d,%s,%d,%s\n",definedE[i].day,definedE[i].month,definedE[i].year, definedE[i].tag,definedE[i].monni, definedE[i].note);}
    fclose(fp);
    printf("\nExpenses saved successfully");
}


void eLoad(){
    FILE *fp = fopen("expenses.txt", "r");
    if (fp == NULL) {
        printf("\nExpenses not found or failed to load");
        return;
    }

    eCount = 0;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (eCount >= MAX_EXPENSE) {
            printf("\nExceeded expense limit, failed to load\n");
            fclose(fp);
            return;
        }

        int scanres = sscanf(line, "%d-%d-%d,%[^,],%d,%[^\n]", &definedE[eCount].day, &definedE[eCount].month, &definedE[eCount].year,definedE[eCount].tag, &definedE[eCount].monni, definedE[eCount].note);

        if (scanres == 5) {
            definedE[eCount].note[0] = '\0';  // manually clear note
            eCount++;
        } else if (scanres == 6) {
            eCount++;
        } else {
            printf("\nLine parsing failed: %s", line);
        }
    }
    fclose(fp);
    printf("\nExpenses loaded successfully\n");
}

void eShow() {
    qsort(definedE,eCount,sizeof(Expense),dateComp);

    int grandTotal=0;
    char uTag[MAX_EXPENSE][MAX_TAG_LENGTH];
    int uTagCount=0;
    int uTagTotal[MAX_EXPENSE]={0};
    //View tags
    printf("\n--Expense list--\n");
    for (int i=0;i<eCount;i++) {
        printf("%d. %02d-%02d-%04d | %s | %d VND | %s\n",i+1,definedE[i].day,definedE[i].month,definedE[i].year,definedE[i].tag, definedE[i].monni, definedE[i].note);
    //Grand total
    grandTotal +=definedE[i].monni;
    //Individual tags total
    int index = tagIndexFind(definedE[i].tag,uTag,uTagCount);
    if (index==-1) {
        strcpy(uTag[uTagCount],definedE[i].tag);
        uTagTotal[uTagCount]=definedE[i].monni;
        uTagCount++;
    } else {
        uTagTotal[index] += definedE[i].monni;
    }
    }
    printf("\nGrand total: %d VND\n",grandTotal);

    printf("--Tag totals--\n");
    for (int i=0;i<uTagCount;i++) {
        printf("%s: %d VND\n",uTag[i],uTagTotal[i]);
    }
}

//EXPENSE FUNCTIONS

void viewRecentE() {
    int shown=0;
    char targetTag[MAX_TAG_LENGTH];
    qsort(definedE,eCount,sizeof(Expense),dateComp);
    //Tag filter
    int useTagFilter = safeIntCheck("\nDo you want to filter by a tag? (Yes=1 | No=0): ");
    if (useTagFilter != 0 && useTagFilter != 1) {
        printf("I guess Ill take that as a no\n");
        useTagFilter=0;
        }
    if (useTagFilter==1) {
        tagShow();
        int tChoice= safeIntCheck("Choose a tag to sort (press 0 to cancel): ");
        if (tChoice==0 || tChoice>tagCount) return;
        strcpy(targetTag,definedTag[tChoice-1]);
    }
    //Number of recent expenses
    int n=safeIntCheck("Number of recent expenses to view? (Max50): ");
    if (n>eCount) {
        n=eCount;
    }
    else if (n>50) {
        printf("\nI told you the maximum is 50, dont be so stubborn, im setting your input to 50 anyways\n");
        n=50;
    }


    //Printing results
    printf("\n--Recent %d expenses--\n",n);
    for (int i=eCount-n;i<eCount;i++) {
        if (useTagFilter==1 && strcmp(targetTag,definedE[i].tag)!=0) continue;
        printf("%02d-%02d-%04d | %s | %d VND | %s\n",definedE[i].day,definedE[i].month,definedE[i].year,definedE[i].tag, definedE[i].monni, definedE[i].note);
        shown++;
    }
    if (shown==0) printf("No matching expenses found.\n");
}

void viewSetE () {
    int shown,total = 0;
    char targetTag[MAX_TAG_LENGTH];
    int sDay,sMonth,sYear,eDay,eMonth,eYear;
    qsort(definedE,eCount,sizeof(Expense),dateComp);
    //Tag filter
    int useTagFilter = safeIntCheck("\nDo you want to filter by a tag? (Yes=1 | No=0): ");
    if (useTagFilter != 0 && useTagFilter != 1) {
        printf("I guess Ill take that as a no\n");
        useTagFilter=0;
        }
    if (useTagFilter==1) {
        tagShow();
        int tChoice= safeIntCheck("Choose a tag to sort (press 0 to cancel: ");
        if (tChoice==0 || tChoice>tagCount) return;
        strcpy(targetTag,definedTag[tChoice-1]);
    }
    //Set start-end date
    do { //start
        printf("\n--Setting start date--\n");
        sDay= safeIntCheck("Enter start day: ");
        sMonth= safeIntCheck("Enter start month: ");
        sYear= safeIntCheck("Enter start year: ");
        if (!validDateCheck(sDay,sMonth,sYear)) {printf("\nInvalid date format, please try again");}
    } while (!validDateCheck(sDay,sMonth,sYear));

    do { //End
        printf("\n--Setting end date--\n");
        eDay= safeIntCheck("Enter end day: ");
        eMonth= safeIntCheck("Enter end month: ");
        eYear= safeIntCheck("Enter end year: ");
        if (!validDateCheck(eDay,eMonth,eYear)) {printf("\nInvalid date format, please try again");}
    } while (!validDateCheck(eDay,eMonth,eYear));

    //Encode date
    int sEncode = dateEncode(sDay,sMonth,sYear);
    int eEncode = dateEncode(eDay,eMonth,eYear);

    //Valid check
    if (eEncode<sEncode) {printf("\nEnd date must be after start date silly "); return;}


    //Printing results
    qsort(definedE,eCount,sizeof(Expense),dateComp);
    printf("\n--Expenses from %02d-%02d-%04d to %02d-%02d-%04d\n",sDay,sMonth,sYear,eDay,eMonth,eYear);
    for (int i=0;i<eCount;i++) {
    int encode= dateEncode(definedE[i].day,definedE[i].month,definedE[i].year);
    if (encode>=sEncode && encode<=eEncode) {
        if (useTagFilter==1 && strcmp(targetTag,definedE[i].tag)!=0) continue;
        printf("%02d-%02d-%04d | %s | %d VND | %s\n",definedE[i].day,definedE[i].month,definedE[i].year,definedE[i].tag, definedE[i].monni, definedE[i].note);
        total+=definedE[i].monni;
        shown++;
        }
    }
    if (shown==0) {
        printf("\nNo matching expenses found\n");
    } else {
    printf("\n Total in range: %d\n",total);
    }
}


void addDefaultE () {
    //Max expense and tag check
    if (eCount>=MAX_EXPENSE) {
        printf("\nExpense list is full, please clear some space");
        return;
    }
    if (tagCount==0) {
        printf("\n No tags available. Please set up tags first\n");
        return;
    }
    //default date check
    if(defaultDateSet == 0){printf("Error: default date has not been set"); return;}
    //Day input
    printf("--Adding new expense, press 0 to cancel");
    int day= safeIntCheck("Input day: ");
    if (day==0) return;
    if (!validDateCheck(day,cMonth,cYear)) {printf("\nInvalid date format");  return;}
    //Tag input
    tagShow();
    int tChoice = safeIntCheck("Choose a tag (Press 0 to cancel): ");
    if (tChoice<=0) return;
    if (tChoice>tagCount) return;
    //Monni input
    int amount=safeIntCheck("Input amount of money(Press 0 to cancel): ");
    if (amount<=0) return;
    //Note input
    printf("\nEnter note (optional): ");
    fgets(definedE[eCount].note, MAX_NOTE, stdin);
    definedE[eCount].note[strcspn(definedE[eCount].note, "\n")] = 0;
    //Adding expense
    definedE[eCount].day=day;
    definedE[eCount].month=cMonth;
    definedE[eCount].year=cYear;
    strcpy(definedE[eCount].tag,definedTag[tChoice-1]);
    definedE[eCount].monni=amount;
    eCount++;
    eSave();
}

void addCustomE() {
    if (eCount>=MAX_EXPENSE) {
        printf("\nExpense list is full, please clear some space");
        return;
    }
    if (tagCount==0) {
        printf("\n No tags available. Please set up tags first\n");
        return;
    }
    printf("--Adding new expense, press 0 to cancel");
    //Date input
    int day= safeIntCheck("Input day: ");
    int month= safeIntCheck("Input month: ");
    int year= safeIntCheck("Input year: ");
    if (!validDateCheck(day,month,year)) {printf("\nInvalid date format");  return;}
    //Tag input
    tagShow();
    int tChoice = safeIntCheck("Choose a tag (Press 0 to cancel): ");
    if (tChoice<=0) return;
    if (tChoice>tagCount) return;
    //Monni input
    int amount=safeIntCheck("Input amount of money(Press 0 to cancel): ");
    if (amount<=0) return;
    //Note input
    printf("\nEnter note (optional): ");
    fgets(definedE[eCount].note, MAX_NOTE, stdin);
    definedE[eCount].note[strcspn(definedE[eCount].note, "\n")] = 0;
    //Adding expense
    definedE[eCount].day=day;
    definedE[eCount].month=month;
    definedE[eCount].year=year;
    strcpy(definedE[eCount].tag,definedTag[tChoice-1]);
    definedE[eCount].monni=amount;
    eCount++;
    eSave();
}


void deleteE() {
    if (eCount == 0) {
        printf("\n No expenses to delete.\n");
        return;
    }

    eShow();
    int index=safeIntCheck("\nEnter an expense to delete: ");

    if (index < 1 || index > eCount) {
        printf("\nInvalid expense number.\n");
        return;
    }

    for (int i = index - 1; i < eCount - 1; i++) {
        definedE[i] = definedE[i + 1];
    }
    eCount--;
    printf("Expense deleted successfully.\n");
    eSave();
}

void bulkDelete() {
    if (eCount == 0) {
        printf("\nNo expenses to delete.\n");
        return;
    }
    int sDay, sMonth, sYear, eDay, eMonth, eYear;
    //Start date
    do {
        printf("\n--Set Start Date--\n");
        sDay = safeIntCheck("Day: ");
        sMonth = safeIntCheck("Month: ");
        sYear = safeIntCheck("Year: ");
    } while (!validDateCheck(sDay, sMonth, sYear));
    //End date
    do {
        printf("\n--Set End Date--\n");
        eDay = safeIntCheck("Day: ");
        eMonth = safeIntCheck("Month: ");
        eYear = safeIntCheck("Year: ");
    } while (!validDateCheck(eDay, eMonth, eYear));
    //Date encode
    int sEncode = dateEncode(sDay, sMonth, sYear);
    int eEncode = dateEncode(eDay, eMonth, eYear);
    //Valid check
    if (eEncode < sEncode) {
        printf("\nEnd date must be after start date silly.\n");
        return;
    }

    int deleted = 0;
    for (int i = 0; i < eCount;) {
        int encode = dateEncode(definedE[i].day, definedE[i].month, definedE[i].year);
        if (encode >= sEncode && encode <= eEncode) {
            // Shift left
            for (int j = i; j < eCount - 1; j++) {
                definedE[j] = definedE[j + 1];
            }
            eCount--;
            deleted++;
        } else {
            i++;  // Only increment if not deleted
        }
    }

    if (deleted == 0) {
        printf("\nNo expenses matched the given date range.\n");
    } else {
        printf("\nDeleted %d expenses in range %02d-%02d-%04d to %02d-%02d-%04d\n", deleted, sDay, sMonth, sYear, eDay, eMonth, eYear);
        eSave();
    }
}

//MAIN FUNCTIONS
void eSetup() {
    int choice;
    do {
        printf("\n--Expense management--\n");
        if (!validDateCheck(1, cMonth, cYear)) {
            printf("Default month and year have not yet set!\nPls go and set them first!");
        }
        else {printf("\nDefault -- Month: %d  Year: %d --\n",cMonth,cYear);}
        printf("\nCurrent number of saved expenses: %d/100\n",eCount);
        printf("1.Set month and year defaults\n");
        printf("2.View all expenses\n");
        printf("3.View recent expenses\n");
        printf("4.View expenses between set date\n");
        printf("5.Add new expense (default)\n");
        printf("6.Add new expense (custom)\n");
        printf("7.Delete expenses (single)\n");
        printf("8.Bulk delete\n");
        printf("9.Back to main menu\n");
        choice = safeIntCheck("Select an option: ");
        switch(choice) {
        case 1:
            defDateSetup();
            break;
        case 2:
            if (eCount==0) {
                printf("\nNo expenses to view\n");
                wait();
                break;
            }
            eShow();
            wait();
            break;
        case 3:
            if (eCount==0) {
                printf("\nNo expenses to view\n");
                wait();
                break;
            }
            viewRecentE();
            wait();
            break;
        case 4:
            if (eCount==0) {
                printf("\nNo expenses to view\n");
                wait();
                break;
            }
            viewSetE();
            wait();
            break;
        case 5:
            addDefaultE();
            wait();
            break;
        case 6:
            addCustomE();
            wait();
            break;
        case 7:
            deleteE();
            wait();
            break;
        case 8:
            bulkDelete();
            wait();
            break;
        case 9:
            break;
        default:
            printf("\nInvalid choice, skipping...");
            break;
        }
    } while (choice!=9);
}
