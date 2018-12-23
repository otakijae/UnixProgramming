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

int main(void){
    int fd, n;
    char buf[512];
    
    mkfifo("fifo", 0600);
    fd = open("fifo", O_RDONLY); //********************주의할 것
    for(;;){
        n = read(fd, buf, 512);
//        printf("%d ", n);
//        if(n == 0)
//            exit(1);
        write(1, buf, n);
     }
}

