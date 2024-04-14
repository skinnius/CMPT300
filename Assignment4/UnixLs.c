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
#include <errno.h>
#include <time.h>


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


void printlflag(struct dirent* dp, struct stat buf) {
    // code taken and adapted from https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
    printf((S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf((buf.st_mode & S_IRUSR) ? "r" : "-");
    printf((buf.st_mode & S_IWUSR) ? "w" : "-");
    printf((buf.st_mode & S_IXUSR) ? "x" : "-");
    printf((buf.st_mode & S_IRGRP) ? "r" : "-");
    printf((buf.st_mode & S_IWGRP) ? "w" : "-");
    printf((buf.st_mode & S_IXGRP) ? "x" : "-");
    printf((buf.st_mode & S_IROTH) ? "r" : "-");
    printf((buf.st_mode & S_IWOTH) ? "w" : "-");
    printf((buf.st_mode & S_IXOTH) ? "x" : "-");

    time_t t = buf.st_mtime;
    char timeString[18];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M", localtime(&t));

    printf("%2lu %1s %1s %11ld %s\n", 
            buf.st_nlink, getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,
            buf.st_size, timeString);
}
    
// , buf.st_mtime
// %02d:%02d

void printliflag(struct dirent* dp, struct stat buf) {
    // do stuff
}

void printnoflag(struct dirent* dp) {
    printf("%s\n", dp->d_name);
}

void printiflag(struct dirent* dp, struct stat buf) {
    printf("%lu %s\n", buf.st_ino, dp->d_name);
}

// for basic ls command
void printBasicDir(char* flag) {
    DIR* dir = NULL;
    struct dirent *dp = NULL;
    struct stat buf;

    dir = opendir(".");
    if (dir == NULL) {
        printf("%s\n", strerror(errno));
        return;
    }

    while ((dp = readdir(dir)) != NULL) {

        char* path = malloc(strlen(dp->d_name) + 3);
        strcpy(path, "./");
        strcat(path, dp->d_name);
        
        if (stat(path, &buf) == -1) {
            printf("%s\n", strerror(errno));
            return;
        }

        free(path);
        if (dp->d_name[0] == '.') { // ensures that hidden files are not displayed
            continue;
        }

        if (strcmp(flag, "i") == 0) {
            printiflag(dp, buf);
        }
        else if (strcmp(flag, "l") == 0) {
            printlflag(dp, buf);
        } 
        else if (strcmp(flag, "il") == 0 || strcmp(flag, "li") == 0){
            printliflag(dp, buf);
        }
        else {
            printnoflag(dp);
        }
    }

    if (dir != NULL) {
        closedir(dir);
    }
}


void lsPrint(int len, char* entryDir[], char* flag) {
    struct stat buf;
    struct dirent *dp = NULL;
    DIR* dir = NULL;

    for (int i = 0; i < len; i++) {
        
        char* fullPath = malloc(strlen(entryDir[i]) + 3);
        strcpy(fullPath, "./"); 
        strcat(fullPath, entryDir[i]);

        dir = opendir(fullPath);

        if (dir == NULL) {
            printf("%s\n", strerror(errno));
            return;
        }
               
        while ((dp = readdir(dir)) != NULL) {
            if (dp->d_name[0] == '.') {
                continue;
            }

            char* path = malloc(strlen(fullPath) + strlen(dp->d_name) + 2);
            strcpy(path, fullPath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if (stat(path, &buf) == -1) {
                printf("%s\n", strerror(errno));
                continue;
            }
            free(path);

            if (strcmp(flag, "i") == 0) {
                printiflag(dp, buf);
            }
            else if (strcmp(flag, "l") == 0) {
                printlflag(dp, buf);
            } 
            else if (strcmp(flag, "il") == 0 || strcmp(flag, "li") == 0){
                printliflag(dp, buf);
            }
            else {
                printnoflag(dp);
            }
        }

        free(fullPath);
        if (dir != NULL) {
            closedir(dir);
        }
    }
}

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
// flag processing
int processFlag(char* flag, char* entryDir[], int numDir) {

    if (numDir == 0) {
        printBasicDir(flag);
    }
    else {
        lsPrint(numDir, entryDir, flag);
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
        printBasicDir("z");
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
    processFlag(flag, dirList, dirListIndex);

    return 0;
}