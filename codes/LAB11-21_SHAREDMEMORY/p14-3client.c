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
    int i, j, id, shmid;
    struct databuf *buf;
    key_t shmkey;
    
    id = atoi(argv[1]);
    shmkey = ftok("shmkey", 3);
    shmid = shmget(shmkey, 6*sizeof(struct databuf), 0600|IPC_CREAT);
    buf = (struct databuf *)shmat(shmid, 0, 0);
    
    for(i=0;i<5;i++){
        scanf("%d", &((buf+id)->data));
        (buf+id)->flag = 1;
        while((buf+id+3)->flag == 0);
        printf("id : %d, input + 8 = %d\n", id, (buf+id+3)->data);
        (buf+id+3)->flag = 0;
    }

    exit(0);
}

