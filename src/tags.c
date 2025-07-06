#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tags.h"
#include "accessories.h"
#include "expense.h"

int tagIndexFind (char *target, char ref[][MAX_TAG_LENGTH], int tagCount) {
    //return -1 if no tags match
    for (int i=0; i<tagCount; i++) {
        if (strcmp(target,ref[i])==0) {return i;}
    }
        return -1;
}

//SAVING AND LOADING
void tagSave() {
    FILE *fp=fopen ("tags.txt","w");
    if (fp==NULL) {printf("\nTags failed to save"); return;}
    for (int i=0;i<tagCount;i++) {
        fprintf(fp,"%s\n",definedTag[i]);
    }
    fclose(fp);
    printf("\nTags saved succesfully");
}

void tagLoad() {
    FILE *fp=fopen("tags.txt","r");
    if (fp==NULL) {printf("\nTags not found or failed to load");  return;}
    tagCount=0;
    //Scanning tags while calculating tag count
    while (fgets(definedTag[tagCount],MAX_TAG_LENGTH,fp)) {
        definedTag[tagCount][strcspn(definedTag[tagCount], "\n")] = 0;
        tagCount++;

    }
    fclose(fp);
}

void tagShow() {
    printf("\n --Your tags--\n");
    for (int i=0;i<tagCount;i++) {
        printf("%d.%s\n",i+1,definedTag[i]);
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
