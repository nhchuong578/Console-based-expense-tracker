#ifndef ACCESSORIES_H
#define ACCESSORIES_H

#define MAX_TAG 100
#define MAX_TAG_LENGTH 256
#define MAX_NOTE 256
#define MAX_EXPENSE 1000

//Expense struct
typedef struct {
    int year,month,day;
    char tag[MAX_TAG_LENGTH];
    int monni;
    char note[MAX_NOTE];
    } Expense;

//User-defined data
extern char definedTag[MAX_TAG][MAX_TAG_LENGTH];
extern Expense definedE[MAX_EXPENSE];
extern int cMonth;
extern int cYear;

//Global variables
extern int tagCount;
extern int eCount;
extern int defaultDateSet;

//FUNCTIONS
//Helper functions
void wait();
int validDateCheck (int day, int month, int year);
int safeIntCheck(const char *message);
int dateEncode(int day, int month, int year);
int dateComp(const void *a, const void *b);

//Default-dates functions
void cDateSave();
void cDateLoad();
void defDateSetup();

#endif // ACCESSORIES_H

