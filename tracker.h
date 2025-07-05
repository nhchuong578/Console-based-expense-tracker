#ifndef TRACKER_H
#define TRACKER_H

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



//FUNCTIONS
//Helper functions
void wait();
int validDateCheck (int day, int month, int year);
int safeIntCheck(const char *message);
int dateEncode(int day, int month, int year);
int dateComp(const void *a, const void *b);
int tagIndexFind (char *target, char ref[][MAX_TAG_LENGTH], int tagCount);
//Saving and loading
void tagShow();
void tagSave();
void tagLoad();
void eLoad();
void eSave();
void cDateSave();
void cDateLoad();
//Tag functions
void addTag();
void deleteTag();
//Expenses function
void eShow();
void viewRecenttE();
void viewSetE ();
void addDefaultE ();
void addCustomE();
void deleteE();
void bulkDelete();
void defDateSetup();
void tagSetup();
void eSetup();

#endif // TRACKER_H
