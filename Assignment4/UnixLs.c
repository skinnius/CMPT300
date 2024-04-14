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


void printlflag(struct dirent* dp, char* path, struct stat buf) {
    time_t t = buf.st_mtime;
    char timeString[18];
    char* linkBuffer = malloc(buf.st_size + 1);

    // code taken and adapted from https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
    printf((S_ISLNK(buf.st_mode)) ? "l" : (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf((buf.st_mode & S_IRUSR) ? "r" : "-");
    printf((buf.st_mode & S_IWUSR) ? "w" : "-");
    printf((buf.st_mode & S_IXUSR) ? "x" : "-");
    printf((buf.st_mode & S_IRGRP) ? "r" : "-");
    printf((buf.st_mode & S_IWGRP) ? "w" : "-");
    printf((buf.st_mode & S_IXGRP) ? "x" : "-");
    printf((buf.st_mode & S_IROTH) ? "r" : "-");
    printf((buf.st_mode & S_IWOTH) ? "w" : "-");
    printf((buf.st_mode & S_IXOTH) ? "x" : "-");

    strftime(timeString, sizeof(timeString), "%b %d %Y %H:%M", localtime(&t));

    if (S_ISLNK(buf.st_mode)) {
        ssize_t len = readlink(path, linkBuffer, buf.st_size + 1);
        if (len == -1) {
            printf("%s\n", strerror(errno));
            free(linkBuffer);
            return;
        }
        linkBuffer[len] = '\0';
        printf("%11lu %1s %15s %11ld %s %s -> %s\n", 
            buf.st_nlink, getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,
            buf.st_size, timeString, dp->d_name, linkBuffer);
    }
    else {
        printf("%11lu %1s %15s %11ld %s %s\n", 
                buf.st_nlink, getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,
                buf.st_size, timeString, dp->d_name);
    }

    free(linkBuffer);

}
    

void printliflag(struct dirent* dp, char* path, struct stat buf) {
    time_t t = buf.st_mtime;
    char timeString[18];
    char* linkBuffer = malloc(buf.st_size + 1);

    // code taken and adapted from https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
    printf((S_ISLNK(buf.st_mode)) ? "l" : (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf((buf.st_mode & S_IRUSR) ? "r" : "-");
    printf((buf.st_mode & S_IWUSR) ? "w" : "-");
    printf((buf.st_mode & S_IXUSR) ? "x" : "-");
    printf((buf.st_mode & S_IRGRP) ? "r" : "-");
    printf((buf.st_mode & S_IWGRP) ? "w" : "-");
    printf((buf.st_mode & S_IXGRP) ? "x" : "-");
    printf((buf.st_mode & S_IROTH) ? "r" : "-");
    printf((buf.st_mode & S_IWOTH) ? "w" : "-");
    printf((buf.st_mode & S_IXOTH) ? "x" : "-");

    strftime(timeString, sizeof(timeString), "%b %d %Y %H:%M", localtime(&t));

    if (S_ISLNK(buf.st_mode)) {
        ssize_t len = readlink(path, linkBuffer, buf.st_size + 1);
        if (len == -1) {
            printf("%s\n", strerror(errno));
            free(linkBuffer);
            return;
        }
        linkBuffer[len] = '\0';
        printf("%11lu %11lu %1s %15s %11ld %s %s -> %s\n", 
            buf.st_ino, buf.st_nlink, getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,
            buf.st_size, timeString, dp->d_name, linkBuffer);
    }
    else {
    printf("%11lu %11lu %1s %15s %11ld %s %s\n", 
            buf.st_ino, buf.st_nlink, getpwuid(buf.st_uid)->pw_name, getgrgid(buf.st_gid)->gr_name,
            buf.st_size, timeString, dp->d_name);
    }

    free(linkBuffer);
}

void printnoflag(struct dirent* dp) {
    printf("%s\n", dp->d_name);
}

void printiflag(struct dirent* dp, struct stat buf) {
    printf("%lu %s\n", buf.st_ino, dp->d_name);
}


void lsPrint(int len, char* entryDir[], char* flag) {
    struct stat buf;
    struct stat entryCheck;
    struct dirent *dp = NULL;
    DIR* dir = NULL;

    for (int i = 0; i < len; i++) {
        if (len > 1) {
            if (i > 0) {
                printf("\n");
            }
            printf("%s:\n", entryDir[i]);
        }

        if (lstat(entryDir[i], &entryCheck) == -1) {
            printf("%s\n", strerror(errno));
            continue;
        }

        else if (!S_ISDIR(entryCheck.st_mode)) {
            printf("%s \n", entryDir[i]);
            continue;
        }

        dir = opendir(entryDir[i]);
        if (dir == NULL) {
            printf("%s\n", strerror(errno));
            return;
        }
               
        while ((dp = readdir(dir)) != NULL) {
            if (dp->d_name[0] == '.') {
                continue;
            }

            char* fullPath = malloc(strlen(entryDir[i]) + strlen(dp->d_name) + 2);
            strcpy(fullPath, entryDir[i]);
            strcat(fullPath, "/"); 
            strcat(fullPath, dp->d_name);

            if (lstat(fullPath, &buf) == -1) {
                printf("%s\n", strerror(errno));
                continue;
            }

            if (strcmp(flag, "i") == 0) {
                printiflag(dp, buf);
            }
            else if (strcmp(flag, "l") == 0) {
                printlflag(dp, fullPath, buf);
            } 
            else if (strcmp(flag, "il") == 0 || strcmp(flag, "li") == 0){
                printliflag(dp, fullPath, buf);
            }
            else {
                printnoflag(dp);
            }

            free(fullPath);
        }

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
        char* entry[1] = {"."};
        lsPrint(1, entry, flag);
        return 0;  
    }
    else {
        lsPrint(numDir, entryDir, flag);
    }
    return 0;
}



int main(int argc, char *argv[]) {
    bool isFlag = true;
    int numArgs = argc - 1;
    char* dirList[numArgs];
    char flag[3];   
    int dirListIndex = 0;
    int flagIndex = 0;


    memset(dirList, 0, sizeof(dirList));
    memset(flag, 0, sizeof(flag));


    if (numArgs == 0) {                    // basic ls with no options
        char* entry[1] = {"."};
        lsPrint(1, entry, "z");
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