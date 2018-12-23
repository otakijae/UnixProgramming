#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <stdlib.h>

#define BUFSIZE 512

int backup(const char *name, const struct stat *status, int type){

    char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE] = "./TEMP/";
    int fd1, fd2, i, nread;

    if(type == FTW_F){

        if(strncmp((char*)name, "./TEMP", 6) == 0)
            return 0;

        fd1 = open(name, O_RDONLY);

        strcpy(file1, name + 2);

        for(i=0;i<strlen(file1);i++){
            if(file1[i] == '/')
                file1[i] = '_';
        }

        strcat(file2, file1);

        fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);

        nread = read(fd1, buffer, BUFSIZE);
        while(nread > 0){
            write(fd2, buffer, nread);
            nread = read(fd1, buffer, BUFSIZE);
        }
    }

    return 0;
}

int main(int argc, char **argv){

    mkdir("TEMP", 0777);
    ftw(".", backup, 1);

    return 0;
}

