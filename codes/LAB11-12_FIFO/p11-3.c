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

#define BUFSIZE 512

int main(int argc, char **argv){

    char buf, f[3][3] = {"f1", "f2", "f3"};
    int i, k ,in ,fd[2];

    for(i=0;i<3;i++){
        mkfifo(f[i], 0600);
    }

    k = atoi(argv[1]);

    if(k > 0)
        fd[0] = open(f[k-1], O_RDONLY);
    if(k < 3)
        fd[1] = open(f[k], O_WRONLY);
    
    if(k > 0)
        read(fd[0], &buf, 1);

    for(i=0;i<5;i++){
        printf("pid : %d\n", getpid());
        sleep(1);
    }

    if(k < 3)
        write(fd[1], "a", 1);

    exit(0);
}
