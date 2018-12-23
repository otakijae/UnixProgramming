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
        int i, len = 0, fd, semid;
        char *addr;
        key_t key;
        union semun arg;
        struct sembuf p_buf;

        key = ftok("key", 3);
        semid = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);

        if(semid == -1){
                semid = semget(key, 1, 0600);
        }
        else{
                arg.val = 0;
                semctl(semid, 0, SETVAL, arg);
        }

        fd = open("temp", O_RDWR | O_CREAT, 0600);

        addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

        for(i=0;i<3;i++){
                p_buf.sem_num = 0;
                p_buf.sem_op = -1;
                p_buf.sem_flg = 0;
                semop(semid, &p_buf, 1);

                printf("semval : %d\n", semctl(semid, 0, GETVAL, arg));

                len = len + write(1, addr + len, strlen(addr+len));
                write(1, "-------\n", 8);
        }

        exit(0);
}
