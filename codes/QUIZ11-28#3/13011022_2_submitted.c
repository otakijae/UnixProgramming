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
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define BUFSIZE 512

void catchalarm(int);
void catchint(int);

void cat_m(char **s){
    char buffer[BUFSIZE];
    int nread, fd = open(s[1], O_RDONLY);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
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

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
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
    int nread, fd = open(s[1], O_RDWR|O_CREAT, 0777);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
    }
    while((nread = read(0, buffer, BUFSIZE)) > 0){
        if((strncmp(buffer, "quit", 4) == 0) && (buffer[4] == '\n'))
            break;
        write(fd, buffer, nread);
    }
}

char b_dir[BUFSIZE] = "./";

int backup(const char *name, const struct stat *status, int type){
    char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE];
    int fd1, fd2, i, nread;

    strcpy(file2, b_dir);

    sleep(1);

    if(type == FTW_F){
        fd1 = open(name, O_RDONLY);
        strcpy(file1, name + 2);

        for(i=0;i<strlen(file1);i++){
            if(file1[i] == '/')
                file1[i] = '_';
        }

        strcat(file2, file1);
        fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
        }
    }
    return 0;
}

void backup_m(char **s){
    printf("*****BACK-UP STARTS*****\n");
    strcat(b_dir, s[1]);
    strcat(b_dir, "/");
    mkdir(b_dir, 0333);
    ftw(".", backup, 1);
    printf("*****BACK-UP ENDS*****\n");
}

void exec_m(char **s){
    execv(s[0], s);
}

void catchint(int signo){
	printf("\n");
}

void catchalarm(int signo){
    printf("*****BACK-UP STARTS*****\n");
    mkdir(b_dir, 0333);
    ftw(".", backup, 1);
    printf("*****BACK-UP ENDS*****\n");
}

int main(int argc, char **argv){
    char in[50], *res[20] = {0};
    char *inst[7] = {"cat", "cd", "cp", "mkdir", "ls", "vi", "backup"};
    void (*f[8])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup_m, exec_m};
    int i, background = 0;
    pid_t pid, b_pid;

    static struct sigaction act;
    sigset_t mask;

    act.sa_handler = catchint;
    sigaction(SIGINT, &act, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    //sigprocmask(SIG_SETMASK, &mask, NULL);
    //do something
    //sigprocmask(SIG_UNBLOCK, &mask, NULL);
    
    static struct sigaction oact;
    act.sa_handler = catchalarm;
    sigaction(SIGALRM, &oact, NULL);

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

        for(i=0;i<7;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }

	//cd 명령은 shell process가 실행
        if(i == 1){
            cd_m(res);
        }
        else{
            if (i == 6){
                background++;
                b_pid = fork();
                if (b_pid == 0){
		    alarm(10);
                    f[i](res);
                    exit(0);
                }
            }
            else{
                pid = fork();
                if (pid == 0){
                    sigprocmask(SIG_SETMASK, &mask, NULL);
                    f[i](res);
                    sigprocmask(SIG_UNBLOCK, &mask, NULL);
                    exit(0);
                }
                else{
                    waitpid(pid, 0, 0);
                }
            }
        }
        if (background){
            if(waitpid(b_pid, 0, WNOHANG)){
                background = 0;
            }
        }
    }
}
