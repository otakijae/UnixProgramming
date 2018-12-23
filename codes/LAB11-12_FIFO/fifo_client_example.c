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
    int fd, j, nread;
    char buf[512];
    
    if((fd=open("fifo", O_WRONLY|O_NONBLOCK))<0){
        printf("fifo open failed\n");
        exit(1);
    }
    
    for(j=0;j<3;j++){
        nread = read(0,buf,512);
        write(fd, buf, nread);
    }
    
    exit(0);
}

