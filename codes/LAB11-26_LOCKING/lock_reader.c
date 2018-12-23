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

int main(int argc, char **argv){
    int fd, i, in;
    struct flock lock;
    
    fd=open("data1", O_RDWR|O_CREAT, 0600);
    
    for(i=0;i<10;i++){
        lock.l_whence = SEEK_CUR;
        lock.l_start = 0;
        lock.l_len = sizeof(int);
        lock.l_type = F_RDLCK;
        fcntl(fd, F_SETLK, &lock);
        
        sleep(1);
        read(fd, &in, sizeof(int));
        printf("read : %d\n", in);
        
        lseek(fd, -4, SEEK_CUR);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
    }
    
    return 0;
}

