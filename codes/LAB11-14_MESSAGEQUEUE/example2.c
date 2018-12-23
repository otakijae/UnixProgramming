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

    while(msgrcv(qid, &msg, sizeof(int), -3, IPC_NOWAIT) > 0){
        msg.mtype = 4;
        msg.mnum = msg.mnum + 8;
        printf("%d\n", msg.mnum);
        msgsnd(qid, &msg, sizeof(int), 0);
    }

    for(i=0;i<3;i++){
        msgrcv(qid, &msg, sizeof(int), 4, 0);
        printf("%dth this is it : %d\n", i, msg.mnum);
    }

    //메세지큐 기능 다 사용하고 삭제해주는 시스템 콜 사용
    msgctl(qid, IPC_RMID, 0);

    exit(0);
}

