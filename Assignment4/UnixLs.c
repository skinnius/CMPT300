#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>


void printList(int n, char* list[]) {
    for (int i = 0; i < n - 1; i++) {
        printf("%s\n", list[i]);
    }
}

void printDir() {
    struct stat buf;
    DIR* dir;
    struct dirent *dp;

    dir = opendir(".");    
    while ((dp = readdir(dir)) != NULL) {
        int status = stat(dp->d_name, &buf);

        if (status == -1) {
            printf("an error occured\n");
            continue;
        }

        printf("%s\n", dp->d_name);
    }


}



int main(int argc, char *argv[]) {

    char* argList[argc - 1];        
    for (int i = 1; i < argc; i++) {        // get all arguments from command line
        argList[i-1] = argv[i];
    }

    printDir();

    return 0;
}