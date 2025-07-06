#ifndef TAGS_H
#define TAGS_H
#define MAX_TAG_LENGTH 256
int tagIndexFind (char *target, char ref[][MAX_TAG_LENGTH], int tagCount);
//Saving and loading
void tagSave();
void tagLoad();
void tagShow();
//Tag functions
void addTag();
void deleteTag();
void tagSetup();
#endif // TAGS_H

