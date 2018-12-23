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
#include <sys/time.h>

#define BUFSIZE 512

int main(int argc, char **argv) {
    char fifo[6][15] = {"p2_f1", "p2_f2", "p2_f3", "p2_f4", "p2_f5", "p2_f6"};
    int i, in, fd[6], select_check, nread, nread_count[3] = {0};
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

    for (i=0;i<3;i++){
        FD_SET(fd[i], &master);
    }

    while (set=master, (select_check = select(fd[2]+1, &set, NULL, NULL, NULL)) > 0) {
        for (i=0;i<3;i++) {
            if (FD_ISSET(fd[i], &set)) {
                if((nread = read(fd[i], &in, sizeof(int))) > 0){
                    printf("received : %d\n", in);
                    in = in + 8;
                    write(fd[i+3], &in, sizeof(int));
                }
                else if(select_check == 3 && nread == 0)
                    return 0;
            }
        }
    }

    exit(0);
}
