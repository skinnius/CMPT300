#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>

// valid flag checker
bool testSetValidFlagChar(char c, bool* iSeen, bool* lSeen) {
    if (c == 'l') {
        *lSeen = true;
        return true;
    }
    if (c == 'i') {
        *iSeen = true;
        return true;
    }

    return false;
}

// for basic ls command
void printBasicDir(char* entryDir[], int len) {
    DIR* dir = NULL;
    struct dirent *dp = NULL;

    for (int i = 0; i < len; i++) {
        dir = opendir(entryDir[i]);

        if (dir == NULL) {
            printf("an error occured\n");
            continue;
        }

        while ((dp = readdir(dir)) != NULL) {
            if (dp->d_name[0] == '.') { // ensures that hidden files are not displayed
                continue;
            }

            printf("%s\n", dp->d_name);
        }
        printf("\n");
    }

    if (dir != NULL) {
        closedir(dir);
    }
}

void lsPrint(char* entryDir[], int len) {
    struct stat buf;
    DIR* dir = NULL;
    struct dirent *dp = NULL;

    for (int i = 0; i < len; i++) {
        dir = opendir(entryDir[i]);

        if (dir == NULL) {
            printf("an error occured\n");
            continue;
        }

        while ((dp = readdir(dir)) != NULL) {
            if (dp->d_name[0] == '.') {
                continue;
            }

            if (stat(dp->d_name, &buf) == -1) {
                printf("an error occured when retrieving the info of this file\n");
                continue;
            }

            printf("%lu %s\n", buf.st_ino, dp->d_name);
        }
    }
    if (dir != NULL) {
        closedir(dir);
    }

}


// -i, --inode = print the index number of each file

bool inFlag(char* s, char c) {
    if (s == NULL) {
        return false;
    }

    int i = 0;
    while (s[i] != 0) {
        if (c == s[i]) {
            return true;
        }
        i++;
    }
    return false;
}


// printing lists
void printList(char* list[]) {
    int i = 0;
    while (list[i] != 0) {
        printf("%s\n", list[i]);
        i++;
    }
}


// flag processing
int processFlag(char* flag, int numFlags, char* entryDir[], int numDir) {
    for (int i = 0; i < numFlags; i++) {
        if (numDir == 0) {
            char* entry[1] = {"."};
            
            if (flag[i] == 'i') {
                lsPrint(entry, 1);
            }
        }

        if (flag[i] == 'i') {
            // iFlag();
        }        
    }
    return 0;
}





int main(int argc, char *argv[]) {
    bool isFlag = true;
    int numFlags = argc - 1;
    char* dirList[numFlags];
    char flag[3];   
    int dirListIndex = 0;
    int flagIndex = 0;


    memset(dirList, 0, sizeof(dirList));
    memset(flag, 0, sizeof(flag));


    if (numFlags == 0) {                    // basic ls with no options
        char* d[1] = {"."};
        printBasicDir(d, 1);
        return 0;    
    }
     
    for (int i = 1; i < argc; i++) {        // get all arguments from command line
        if (argv[i] == NULL) {
            continue;
        }

        bool iSeen = false;
        bool lSeen = false;
        int j = 0;

        char c = argv[i][j];                // parse through string to determine if it is a valid flag. 
        if (c != '-') {
            isFlag = false;
        }

        c = argv[i][++j];
        while (c != '\0') {
            if (isFlag && !testSetValidFlagChar(c, &iSeen, &lSeen)) {
                isFlag = false;
            }
            c = argv[i][++j];
        }

        if (isFlag && flagIndex < 3) {
            if (iSeen && !inFlag(flag, 'i')) {
                flag[flagIndex++] = 'i';
            }

            if (lSeen && !inFlag(flag, 'l')){
                flag[flagIndex++] = 'l';
            }
        }
        else {
            dirList[dirListIndex++] = argv[i];
        }
    }



    // ------------------------- PROCESSING FLAGS ----------------------------------
    processFlag(flag, flagIndex, dirList, dirListIndex);
    
    // printf("------- flags: -------------\n");
    // printf("%s\n", flag);

    // printf("--------dir------------\n");
    // printList(dirList);





    return 0;
}