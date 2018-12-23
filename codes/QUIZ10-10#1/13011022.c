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

void cat_m(char **s){
	char buffer[BUFSIZE];
    	int fd, nread;

    	fd = open(s[1], O_RDONLY);

    	nread = read(fd, buffer, BUFSIZE);
    	while(nread > 0){
    		write(1, buffer, nread);
        	nread = read(fd, buffer, BUFSIZE);
    	}
	
	close(fd);
}

void cd_m(char **s){
	chdir(s[1]);
}

void cp_m(char **s){
    char buffer[BUFSIZE];
    int fd1, fd2, nread;

    fd1 = open(s[1], O_RDONLY);

    if (fd1 == -1) {
        return;
    } else {
        fd2 = open(s[2], O_WRONLY | O_CREAT | O_EXCL, 0777);

        if(fd2 == -1){
            fd2 = open(s[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        }

        nread = read(fd1, buffer, BUFSIZE);
        while(nread > 0){
            write(fd2, buffer, nread);
            nread = read(fd1, buffer, BUFSIZE);
        }
    }

    close(fd1);
    close(fd2);
}

void mkdir_m(char **s){
	mkdir(s[1], 0777);
}

void ls_m(char **s){
	DIR *dp;
    	struct dirent *d;
    	dp = opendir(".");
    	while((d = readdir(dp)) != NULL){
        	if(d->d_name[0] != '.'){
            		printf("%s\n", d->d_name);
        	}
    	}
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

void exec_m(char **s){
	execv(s[0], s);
}

int main(int argc, char **argv){

    char in[50], *res[20] = {0};
    char *inst[6] = {"cat", "cd", "cp", "mkdir", "ls", "vi"};
    void (*f[7])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, exec_m};
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

        for(i=0;i<6;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }
        f[i](res);
    }
}

