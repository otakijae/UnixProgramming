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
    key_t semkey, shmkey;
    int semid, shmid, i, n;
    int *buf;
    union semun arg;
    struct sembuf p_buf;
    
    semkey = ftok("semkey", 3);
    semid = semget(semkey, 1, 0600|IPC_CREAT|IPC_EXCL);
    
    if(semid == -1){
        semid = semget(semkey, 1, 0600);
    }
    else{
        arg.val = 0;
        semctl(semid, 0, SETVAL, arg);
    }

    shmkey = ftok("shmfile", 1);
    shmid = shmget(shmkey, 10*sizeof(int), IPC_CREAT|0600);

    buf = (int *)shmat(shmid, 0, 0);

    for(i=0;i<10;i++){
        p_buf.sem_num = 0;
        p_buf.sem_op = -1;
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);
        
        printf("%d\n", *(buf+i));
    }

    semctl(semid, IPC_RMID, 0);
    shmdt(buf);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}
