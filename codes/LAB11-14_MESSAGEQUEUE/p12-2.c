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

struct q_entry{
    long mtype;
    int data;
};

int main(int argc, char **argv){

    int i, id, qid;
    key_t key;
    struct q_entry msg;

    key = ftok("key", 3);
    qid = msgget(key, IPC_CREAT|0600);

    id = atoi(argv[1]);

    if(id > 1)
        msgrcv(qid, &msg, sizeof(int), id, 0);

    for(i=0;i<5;i++){
        sleep(1);
        printf("id = %d pid = %d\n", id, getpid());
    }

    if(id < 4){
        msg.mtype = id + 1;
        msg.data = id;
        msgsnd(qid, &msg, sizeof(int), 0);
    }

    exit(0);
}

