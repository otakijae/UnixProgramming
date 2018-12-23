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
#include <signal.h>

#define BUFSIZE 512

int main(int argc, char **argv) {
    char fifo[6][15] = {"f1", "f2", "f3", "f4", "f5", "f6"};
    int i, in, fd[6], n1, nread, counter = 0;
    fd_set set, master;
    
    for(i=0;i<6;i++){
        mkfifo(fifo[i], 0600);
    }
    
    fd[0] = open(fifo[0], O_RDONLY);
    fd[1] = open(fifo[1], O_RDONLY);
    fd[2] = open(fifo[2], O_RDONLY);
    
    fd[3] = open(fifo[3], O_WRONLY);
    fd[4] = open(fifo[4], O_WRONLY);
    fd[5] = open(fifo[5], O_WRONLY);
    
    FD_ZERO(&master);
    
    for (i=0;i<3;i++)
        FD_SET(fd[i], &master);
    
    while (set=master, (n1 = select(fd[2]+1, &set, NULL, NULL, NULL)) > 0) {
        printf("n1 : %d\n", n1);
        for (i=0;i<3;i++) {
            if (FD_ISSET(fd[i], &set)) {
                if((nread = read(fd[i], &in, sizeof(int))) > 0){
                    printf("nread : %d, received : %d\n", nread, in);
                    in = in + 8;
                    write(fd[i+3], &in, sizeof(int));
                }
                else if(nread == 0)
                    counter++;
            }
        }
        if(n1 == 3 && counter == 3){
            exit(0);
        }
        counter = 0;
    }
    
    exit(0);
}

