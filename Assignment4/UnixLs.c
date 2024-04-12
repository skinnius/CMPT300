#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>

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


int main(int argc, char *argv[]) {
    bool dir;
    int numFlags = argc - 1;
    
    if (numFlags == 0) {                    // basic ls
        char* d[1] = {"."};
        printBasicDir(d, 1);
        return 0;    
    }

    char* argList[numFlags];        
    for (int i = 1; i < argc; i++) {        // get all arguments from command line
        argList[i-1] = argv[i];
    }


    return 0;
}