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

struct q_entry{
    long mtype;
    int data;
};

int main(int argc, char **argv){
    int i, j, id, qid;
    key_t key;
    struct q_entry msg;
    
    key = ftok("key", 3);
    qid = msgget(key, IPC_CREAT|0600);
    
    id = atoi(argv[1]);
    
    for(i=4;i<7;i++){

        if(id > 1)
            msgrcv(qid, &msg, sizeof(int), id, 0);
        
        if(id == 1 && i > 4)
            msgrcv(qid, &msg, sizeof(int), i, 0);
        
        for(j=0;j<3;j++){
            sleep(1);
            printf("id = %d pid = %d\n", id, getpid());
        }
        
        if(id < 3){
            msg.mtype = id + 1;
            msg.data = id;
            msgsnd(qid, &msg, sizeof(int), 0);
        }
        else if(id == 3 && i < 6){
                msg.mtype = i + 1;
                msg.data = id;
                msgsnd(qid, &msg, sizeof(int), 0);
        }
    }
    
    if(id == 3)
        msgctl(qid, IPC_RMID, 0);
    
    exit(0);
}

