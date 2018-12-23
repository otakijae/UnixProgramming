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

struct databuf{
    int flag;
    int data;
};

int main(int argc, char **argv){
    int i, j, shmid;
    struct databuf *buf;
    key_t shmkey;
    
    shmkey = ftok("shmkey", 3);
    shmid = shmget(shmkey, 6*sizeof(struct databuf), 0600|IPC_CREAT);
    buf = (struct databuf *)shmat(shmid, 0 ,0);
    
    for(i=0;i<15;i++){
        for(j=0;;j=(j+1)%3)
            if((buf+j)->flag == 1)
                break;
        (buf+j+3)->data = (buf+j)->data + 8;
        (buf+j)->flag = 0;
        (buf+j+3)->flag = 1;
    }
    
    shmctl(shmid, IPC_RMID, 0);

    exit(0);
}

