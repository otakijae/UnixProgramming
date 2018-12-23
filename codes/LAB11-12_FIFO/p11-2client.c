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

int main(int argc, char **argv){
    char fifo[6][15] = {"p2_f1", "p2_f2", "p2_f3", "p2_f4", "p2_f5", "p2_f6"};
    int i, in, fd[6];
    int id = atoi(argv[1]);

    fd[id] = open(fifo[id], O_WRONLY);
    fd[id+3] = open(fifo[id+3], O_RDONLY);

    scanf("%d", &in);
    write(fd[id], &in, sizeof(int));

    read(fd[id+3], &in, sizeof(int));

    for(i=0;i<5;i++){
        sleep(1);
        printf("received : %d\n", in);
    }
    exit(0);
}
