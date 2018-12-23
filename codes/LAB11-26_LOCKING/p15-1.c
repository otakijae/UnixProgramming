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

void do_child(int fd){
    int in;
    struct flock lock;
    
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 4;
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    
    lseek(fd, 0, SEEK_SET);
    read(fd, &in, sizeof(int));
    sleep(2);
    
    in = in + 10;
    lseek(fd, 0, SEEK_SET);
    write(fd, &in, sizeof(int));
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    
    exit(0);
}

int main(int argc, char **argv){
    int fd, i, num;
    struct flock lock;
    pid_t pid[3];
    
    fd=open("data", O_RDWR|O_CREAT, 0600);
    scanf("%d", &num);
    write(fd, &num, sizeof(int));

    for(i=0;i<3;i++){
        pid[i] = fork();
        if(pid[i] == 0){
            do_child(fd);
        }
        else{
            wait(0);
        }
    }
    
    lseek(fd, 0, SEEK_SET);
    read(fd, &num, sizeof(int));
    printf("%d\n", num);
    
    return 0;
}

