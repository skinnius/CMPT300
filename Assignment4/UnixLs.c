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
bool isValidFlagChar(char c) {
    if (c == 'l' || c == 'i') {
        return true;
    }
    return false;
}

// for basic ls command
void printBasicDir(char* entryDir[], int len) {
    struct stat buf;
    DIR* dir;
    struct dirent *dp;

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
            int status = stat(dp->d_name, &buf);
            if (status == -1) {
                printf("an error occured when retrieving the info of this file\n");
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


// -i, --inode = print the index number of each file

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
    char* flagList[numFlags];   
    int dirListIndex = 0;
    int flagListIndex = 0;


    memset(dirList, 0, sizeof(dirList));
    memset(flagList, 0, sizeof(flagList));


    if (numFlags == 0) {                    // basic ls
        char* d[1] = {"."};
        printBasicDir(d, 1);
        return 0;    
    }
     
    for (int i = 1; i < argc; i++) {        // get all arguments from command line
        if (argv[i] == NULL) {
            continue;
        }

        int j = 0;
        char c = argv[i][j];                // parse through string to determine if it is a valid flag. 
        if (c != '-') {
            isFlag = false;
        }

        c = argv[i][++j];

        while (c != '\0') {
            if (isFlag && !isValidFlagChar(c)) {
                isFlag = false;
            }
            c = argv[i][++j];
        }

        if (isFlag) {
            flagList[flagListIndex] = argv[i]; 
            flagListIndex++;
        }
        else {
            dirList[dirListIndex] = argv[i];
            dirListIndex++;
        }
    }
    printf("------- flags: -------------\n");
    printList(flagList);

    printf("--------dir------------\n");
    printList(dirList);





    return 0;
}