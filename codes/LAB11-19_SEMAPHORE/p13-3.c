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

#define BUFSIZE 512

union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int main(int argc, char **argv){
    int i, id, pid, semid;
    key_t key;
    union semun arg;
    struct sembuf p_buf;

    id = atoi(argv[1]);
    key = ftok("key", 1);
    semid = semget(key, 3, 0600|IPC_CREAT|IPC_EXCL);

    if(semid == -1){
        semid = semget(key, 3, 0600);
    }
    else{
        arg.val = 0;
        semctl(semid, 0, SETVAL, arg);
    }

    if(id>1){
        p_buf.sem_num = 0;
        p_buf.sem_op = -id;
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);
    }

    pid = getpid();

    for(i=0;i<5;i++){
        sleep(1);
        printf("pid : %d\n", pid);
    }

    if(id<4){
        p_buf.sem_num = 0;
        p_buf.sem_op = id + 1;
        p_buf.sem_flg = 0;
        semop(semid, &p_buf, 1);
    }

    exit(0);
}
