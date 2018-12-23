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
    int mnum;
};

int main(int argc, char **argv){
    int qid, i, in;
    struct q_entry msg;
    key_t key;
    
    key = ftok("keyfile", 1);
    qid = msgget(key, 0600|IPC_CREAT);

    if(qid == -1){
        perror("msgget");
        exit(0);
    }

    for(i=0;i<3;i++){
        scanf("%d", &in);
        msg.mtype = i+1;
        msg.mnum = in;
        msgsnd(qid, &msg, sizeof(int), 0);
    }
    exit(0);
}

