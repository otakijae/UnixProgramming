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

union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
};

int main(int argc, char **argv){
    int i, semid, shmid;
    key_t semkey, shmkey;
    union semun arg;
    struct sembuf p_buf;
    ushort sembuf[2] = {0};
    int *shmbuf;
    
    semkey = ftok("semkey", 3);
    semid = semget(semkey, 2, 0600|IPC_CREAT|IPC_EXCL);
    
    if(semid == -1){
        semid = semget(semkey, 1, 0600);
    }
    else{
        arg.array = sembuf;
        semctl(semid, 0, SETALL, arg);
    }
    
    shmkey = ftok("shmkey", 3);
    shmid = shmget(shmkey, sizeof(int), 0600|IPC_CREAT);
    shmbuf = (int *)shmat(shmid, 0 ,0);
    
    for(i=0;i<5;i++){
        scanf("%d", (shmbuf+0));
        
        p_buf.sem_num = 0;
        p_buf.sem_op = 1;
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);
        
        p_buf.sem_num = 1;
        p_buf.sem_op = -1;
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);
        
        printf("[input + 8] ==> %d\n", *(shmbuf+0));
    }

    exit(0);
}

