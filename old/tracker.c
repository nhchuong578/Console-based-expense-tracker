#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tracker.h"

//VARIABLES
int eCount=0;
int tagCount=0;
int cMonth; //default month
int cYear;  //default year
int defaultDateSet = 0;
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

int dateComp(const void *a, const void *b) {
    const Expense *ea = (const Expense *)a;
    const Expense *eb = (const Expense *)b;
    int date1 = dateEncode(ea->day, ea->month, ea->year);
    int date2 = dateEncode(eb->day, eb->month, eb->year);
    return date1 - date2;
}

int tagIndexFind (char *target, char ref[][MAX_TAG_LENGTH], int tagCount) {
    //return -1 if no tags match
    for (int i=0; i<tagCount; i++) {
        if (strcmp(target,ref[i])==0) {return i;}
    }
        return -1;
}

void tagShow() {
    printf("\n --Your tags--\n");
    for (int i=0;i<tagCount;i++) {
        printf("%d.%s\n",i+1,definedTag[i]);
    }
}


//SAVING AND LOADING
void tagSave() {
    FILE *fp=fopen ("tags.txt","w");
    if (fp==NULL) {printf("\nTags failed to save"); return;}
    for (int i=0;i<tagCount;i++) {
        fprintf(fp,"%s\n",definedTag[i]);
    }
    fclose(fp);
    printf("\nTags saved succesfully\n");
}

void tagLoad() {
    FILE *fp=fopen("tags.txt","r");
    if (fp==NULL) {printf("\nTags failed to load");  return;}
    tagCount=0;
    //Scanning tags while calculating tag count
    while (fgets(definedTag[tagCount],MAX_TAG_LENGTH,fp)) {
        definedTag[tagCount][strcspn(definedTag[tagCount], "\n")] = 0;
        tagCount++;

    }
    fclose(fp);
}

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
        printf("\nExpenses failed to load");
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

//TAG FUNCTIONS
void addTag() {
    printf("\nType (done) to exit\n");
    while (tagCount<MAX_TAG) {
        char tagInput[MAX_TAG_LENGTH];
        printf("\nInput tag no.%d: ",tagCount+1);
        fgets(tagInput,MAX_TAG_LENGTH,stdin); //(safer than scanf() for text input)
        tagInput[strcspn(tagInput,"\n")]=0; //remove the \n and replace to \0
        //Tag dupes check
        if (tagIndexFind(tagInput,definedTag,tagCount) !=-1) {
            printf("\nTag %s already exists.\n",tagInput);
            continue;
        }

        if (strcmp(tagInput,"done")==0) break;
        strcpy(definedTag[tagCount],tagInput);//save Tags to the array
        tagCount++;
    }
    tagSave();
    return;
}

void deleteTag() {
    //Show tags
    if (tagCount==0) {printf("\n You have not set any tags (u dummy)\n"); return;}
    printf("\nBe aware, this will delete any associated expenses\n");
    tagShow();

    int delChoice = safeIntCheck("Enter the number of the tag you want to delete (Press 0 to cancel): ");

    //Valid check
    if (!delChoice) {return;}
    else if (delChoice>tagCount) {
        printf("\nGood luck next time\n");
        return;
        }
    //Saved deleted tag to delete the expenses
    char deletedTag[MAX_TAG_LENGTH];
    strcpy(deletedTag,definedTag[delChoice-1]);
    //Tag index delete
    for (int i=delChoice-1;i<tagCount;i++) {
        strcpy(definedTag[i],definedTag[i+1]);
        }
    tagCount--;
    tagSave();
    //Delete related expenses
    for (int i=0;i<eCount;) {
        if (strcmp(definedE[i].tag,deletedTag)==0) {
            for (int j=i;j<eCount-1;j++) {
                    definedE[j] = definedE[j+1];
                    }
            eCount--;
            }
        else {i++;} //Only increase if theres no deletion
        }
            printf("\nAll related expenses deleted");
            eSave();
    }
//EXPENSES FUNTIONS
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

void viewRecentE() {
    int shown=0;
    char targetTag[MAX_TAG_LENGTH];
    qsort(definedE,eCount,sizeof(Expense),dateComp);
    //Tag filter
    int useTagFilter = safeIntCheck("\nDo you want to filter by a tag? (Yes=1 | No=0)");
    if (useTagFilter != 0 && useTagFilter != 1) {
        printf("I guess Ill take that as a no\n");
        useTagFilter=0;
        }
    if (useTagFilter==1) {
        tagShow();
        int tChoice= safeIntCheck("Choose a tag to sort (press 0 to cancel");
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
    int useTagFilter = safeIntCheck("\nDo you want to filter by a tag? (Yes=1 | No=0)");
    if (useTagFilter != 0 && useTagFilter != 1) {
        printf("I guess Ill take that as a no\n");
        useTagFilter=0;
        }
    if (useTagFilter==1) {
        tagShow();
        int tChoice= safeIntCheck("Choose a tag to sort (press 0 to cancel");
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
    printf("\n--Expenses from %02d-%02d-%04d to %02d-%02d-%040d\n",sDay,sMonth,sYear,eDay,eMonth,eYear);
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
    printf("--Adding new expense, press 0 to cancel");
    //Date input
    int day= safeIntCheck("Input day: ");
    int month= safeIntCheck("Input month: ");
    int year= safeIntCheck("Input year: ");
    if (!validDateCheck(day,month,year)) return;
    //Tag input
    tagShow();
    int tChoice = safeIntCheck("Choose a tag (Press 0 to cancel)");
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

//DEFAULT DATE SETUP
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

//MAIN FUNCTIONS
void tagSetup(){
    int choice;
    do {
        printf("\n--Tag management--\n");
        printf("No of tags: %d\n",tagCount);
        printf("1.View tags\n");
        printf("2.Add new tag\n");
        printf("3.Delete a Tag\n");
        printf("4.Back to main menu\n");
        choice = safeIntCheck("Select an option: ");

    switch (choice){
        case 1:
            if (tagCount==0) {printf("\nNo tags to show\n"); wait();break;}
            tagShow();
            wait();
            break;
        case 2:
            addTag();
            wait();
            break;
        case 3:
            deleteTag();
            wait();
            break;
        case 4: break;
        default:
            printf("\nInvalid choice, please try again (its not that hard)\n");
        }
    } while (choice!=4);
}

void eSetup()
{
    int choice;
    do {
        printf("\n--Expense management--\n");
        if (!validDateCheck(1,cMonth,cYear)) {
            printf("Default month and year have not yet set (or are in the wrong format)\n Pls go and set them!");
        }
        else {printf("\nDefault -- Month: %d  Year:%d --\n",cMonth,cYear);}
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
        choice = safeIntCheck("Select an option:");
    switch(choice) {
    case 1:
        defDateSetup();
        break;

    case 2:
        if (eCount==0) {printf("\nNo expenses to view\n"); wait(); break;}
        eShow();
        wait();
        break;

    case 3:
        if (eCount==0) {printf("\nNo expenses to view\n"); wait(); break;}
        viewRecentE();
        wait();
        break;
    case 4:
        if (eCount==0) {printf("\nNo expenses to view\n"); wait(); break;}
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
        printf("\nInvalid choice, skipping");
        break;
    }
    } while (choice!=9);
}














