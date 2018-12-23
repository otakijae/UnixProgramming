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

void mkdir_m(char **s){
	mkdir(s[1], 0777);
}

void vi_m(char **s){
        char buffer[BUFSIZE];
        int fd1, nread;

        fd1 = open(s[1], O_RDONLY);

        if (fd1 == -1) {
                fd1 = open(s[1], O_WRONLY | O_CREAT, 0777);
                while(1) {
                        int n = read(0, buffer, BUFSIZE);
                        if (strncmp(buffer, "quit", 4) == 0) {
                                return;
                        }
                        write(fd1, buffer, n);
                }
        } else {
                fd1 = open(s[1], O_RDWR);

                nread = read(fd1, buffer, BUFSIZE);
                while(nread > 0){
                        write(1, buffer, nread);
                        nread = read(fd1, buffer, BUFSIZE);
                }

                while(1) {
                        int n = read(0, buffer, BUFSIZE);
                        if (strncmp(buffer, "quit", 4) == 0) {
                                return;
                        }
                        write(fd1, buffer, n);
                }
        }
}

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

void backup_m(char **s){
	mkdir(s[1], 0777);
	ftw(".", backup, 1);
}

int delete_file(const char *name, const struct stat *status, int type){

    if(type == FTW_F){
        unlink(name);
    }

    return 0;
}

int delete_directory(const char *name, const struct stat *status, int type){

    int k;
    struct dirent *d;
    DIR *dp;

    if(type == FTW_D){
        rmdir(name);
    }

    return 0;
}

void rmall_m(char **s){
	ftw(s[1], delete_file, 1);
	ftw(s[1], delete_directory, 1);
}

int main(int argc, char **argv){

    char in[50], *res[20] = {0};
    char *inst[4] = {"mkdir", "vi", "backup", "rmall"};
    void (*f[5])(char **) = {mkdir_m, vi_m, backup_m, rmall_m};
    int i;

    while(1){
        printf("> ");
        gets(in);
        i = 0;
        res[i] = strtok(in, " ");
        if(strcmp(res[0], "exit")==0)
            exit(0);

        while(res[i]){
            i++;
            res[i] = strtok(NULL, " ");
        }

        for(i=0;i<4;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }
        f[i](res);
    }
}

