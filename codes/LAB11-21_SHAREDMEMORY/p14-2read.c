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
    key_t key;
    int shmid, i, n;
    struct databuf *buf;

    key = ftok("shmfile", 1);
    shmid = shmget(key, 10* sizeof(struct databuf), IPC_CREAT|0600);

    buf = (struct databuf *)shmat(shmid, 0, 0);

    for(i=0;i<10;i++){
        while((buf+i)->flag == 0);
        printf("%d\n", (buf+i)->data);
    }

    shmdt(buf);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}

