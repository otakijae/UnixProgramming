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
#include <signal.h>

#define BUFSIZE 512

int id_counter, total_user, message_counter;

struct q_entry{
    long mtype;
    int id_counter;
    int total_user;
    int message_counter;
};

struct q_data{
    long mtype;
    int id;
    char mtext[BUFSIZE];
};

void receiver(int id, int qid){
    struct q_entry user_count;
    struct q_data user_message;

    while(msgrcv(qid, &user_message, sizeof(struct q_data), message_counter, 0) > 0){
        if(user_message.id != id){
            if(strcmp(user_message.mtext, "talk_start") == 0){
                printf("talk_start\n");
            }
            else{
                printf("%d : %s\n", user_message.id, user_message.mtext);
            }
        }
        else{
            if (strcmp(user_message.mtext, "talk_quit") == 0) {
                exit(1);
            }
        }
        message_counter++;
    }
    exit(1);
}

void sender(int id, int qid){
    int i;
    char buffer[BUFSIZE];
    struct q_entry user_count;
    struct q_data user_message;

    while(gets(buffer)) {
        msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
        user_count.mtype = 1;
        id_counter = user_count.id_counter;
        total_user = user_count.total_user;
        message_counter = user_count.message_counter;

        if ((strcmp(buffer, "talk_quit") == 0)) {
            for(i=0;i<total_user;i++){
                user_message.mtype = message_counter;
                user_message.id = id;
                strcpy(user_message.mtext, buffer);
                msgsnd(qid, &user_message, sizeof(struct q_data), 0);
            }

            user_count.mtype = 1;
            message_counter++;
            user_count.message_counter = message_counter;
            msgsnd(qid, &user_count, 3*sizeof(int), 0);

            return;
        }
        else{
            if(total_user == 1){
                printf("talk_wait ... \n");
            }

            for(i=0;i<total_user;i++){
                user_message.mtype = message_counter;
                user_message.id = id;
                strcpy(user_message.mtext, buffer);
                msgsnd(qid, &user_message, sizeof(struct q_data), 0);
            }

            user_count.mtype = 1;
            message_counter++;
            user_count.message_counter = message_counter;
            msgsnd(qid, &user_count, 3*sizeof(int), 0);
        }
    }
    return;
}

int main(int argc, char **argv){
    int id, qid;
    key_t key;
    pid_t pid;
    struct q_entry user_count;
    struct q_data user_message;

    key = ftok("keyfile", 1);
    qid = msgget(key, 0600|IPC_CREAT);

    if(msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT) < 0){
        id_counter = 1;
        total_user = 1;
        message_counter = 2;

        user_count.mtype = 1;
        user_count.id_counter = id_counter;
        user_count.total_user = total_user;
        user_count.message_counter = message_counter;
        id = id_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }
    else{
        id_counter = user_count.id_counter + 1;
        total_user = user_count.total_user + 1;
        message_counter = user_count.message_counter;

        user_count.mtype = 1;
        user_count.id_counter = id_counter;
        user_count.total_user = total_user;
        user_count.message_counter = message_counter;
        id = id_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }

    printf("id = %d\n", id);

    if(total_user == 1){
        printf("talk_wait ... \n");
    }
    else if(total_user == 2){
        printf("talk_start\n");

        msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
        message_counter = user_count.message_counter;

        user_message.mtype = message_counter;
        user_message.id = id;
        strcpy(user_message.mtext, "talk_start");
        msgsnd(qid, &user_message, sizeof(struct q_data), 0);

        user_count.mtype = 1;
        message_counter++;
        user_count.message_counter = message_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }
    else if(total_user > 2){
        printf("talk_start\n");
    }

    pid = fork();

    if(pid == 0){
        receiver(id, qid);
    }
    else{
        sender(id, qid);
    }

    msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
    id_counter = user_count.id_counter;
    total_user = user_count.total_user;
    message_counter = user_count.message_counter;

    user_count.mtype = 1;
    user_count.id_counter = id_counter;
    user_count.total_user = total_user - 1;
    user_count.message_counter = message_counter;
    msgsnd(qid, &user_count, 3*sizeof(int), 0);

    waitpid(pid, 0, 0);

    if(user_count.total_user == 0){
        msgctl(qid, IPC_RMID, 0);
    }

    exit(1);
}

