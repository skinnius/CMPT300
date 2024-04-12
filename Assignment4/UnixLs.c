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

ino_t* iFlag(char* entryDir[], int len) {
    ino_t* inodeArr = malloc(len*sizeof(int));
    struct stat buf;
    DIR* dir = NULL;
    struct dirent *dp = NULL;
    int arrIndex = 0;

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

            inodeArr[arrIndex++] = buf.st_ino;
        }
    }
    if (dir != NULL) {
        closedir(dir);
    }
     
    return inodeArr;

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

int main(int argc, char *argv[]) {
    bool isFlag = true;
    int numFlags = argc - 1;
    char* dirList[numFlags];
    char flag[3];   
    int dirListIndex = 0;
    int flagListIndex = 0;


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

        if (isFlag && flagListIndex < 3) {
            if (iSeen && !inFlag(flag, 'i')) {
                flag[flagListIndex++] = 'i';
            }

            if (lSeen && !inFlag(flag, 'l')){
                flag[flagListIndex++] = 'l';
            }
        }
        else {
            dirList[dirListIndex++] = argv[i];
        }
    }



    // ------------------------- PROCESSING FLAGS ----------------------------------

    // first account for flags
    // for (int i = 0; i < flagListIndex; i++) {
    //     char* flag = flagList[i];
        
        
    // }


    printf("------- flags: -------------\n");
    printf("%s\n", flag);

    printf("--------dir------------\n");
    printList(dirList);





    return 0;
}