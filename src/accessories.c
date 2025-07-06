#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tags.h"
#include "accessories.h"
#include "expense.h"
//VARIABLES
int eCount=0;
int tagCount=0;
int cMonth; //default month
int cYear;  //default year
int defaultDateSet = 0; //flag for default-date set up or not

//DEFINITION OF USER DATA
char definedTag[MAX_TAG][MAX_TAG_LENGTH];
Expense definedE[MAX_EXPENSE];

//HELPER FUNCTIONS
void wait() {
    printf("\nPress Enter to continue, just take your time...");
    while (getchar() != '\n');
}
int validDateCheck (int day, int month, int year) {
    // returns 0 if invalid
    //year check
    if (year<1900 || year>2169) return 0;
    int isLeap = ((year%4)==0 && (year%100)!=0) || (year%400 == 0);
    //month check
    if (month<1 || month>12) return 0;
    //day check
    int dayCheck[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (isLeap && month==2) {
        if (day<1 || day>29) return 0;
    }
    else {
        if (day<1 || day>dayCheck[month]) return 0;
    }
    return 1;
}

int safeIntCheck(const char *message) {
    int input;
    int check;

    do {
        printf("\n%s", message);
        check = scanf("%d", &input);
        while (getchar() != '\n');

        if (!check) {
            printf("\nSmart ass.\n");
        }
    } while (!check);

    return input;
}

int dateEncode(int day, int month, int year) {
    //returns YYYYMMDD
    return year*10000 + month*100 + day;
}

int dateComp(Expense a, Expense b) {
    int date1 = dateEncode(a.day, a.month, a.year);
    int date2 = dateEncode(b.day, b.month, b.year);
    return date1 - date2;
}


//DEFAULT-DATES FUNCTIONS
void cDateSave () {
    FILE *fp = fopen("date_def.txt", "w");
    if (fp==NULL) {printf("\nCurrent Month and Year failed to save\n"); return;}
    else {
        fprintf(fp, "%d\n%d\n", cMonth, cYear);
        fclose(fp);
        printf("\nDefault saved successfully\n");
    }
}

void cDateLoad() {
    FILE *fp = fopen("date_def.txt", "r");
    if (fp==NULL) {printf("\nNo default Month and Year found\n"); return;}
    else {
        if (fscanf(fp, "%d", &cMonth) == 1 && fscanf(fp, "%d", &cYear) == 1 && validDateCheck(1, cMonth, cYear)) {
            defaultDateSet = 1;
        } else {
            printf("\nDefault date file found, but contents are invalid\n");
            defaultDateSet = 0;
        }
        fclose(fp);
    }
}

void defDateSetup() {
    printf("--Default date configuration--\n");
    do {
        cMonth = safeIntCheck("Input month default: ");
        cYear = safeIntCheck ("Input Year default: ");
        if (!validDateCheck(1,cMonth,cYear)) printf("\nInvalid default, please set them again\n");
    } while (!validDateCheck(1,cMonth,cYear));
    cDateSave();
    cDateLoad();
}
