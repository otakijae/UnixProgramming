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
#include <sys/errno.h>

#define BUFSIZE 512
#define N 5

void debug(struct manage_buffer *manage, struct message_buffer *message);

struct manage_buffer{
    int front;
    int back;

    int total_user;
    int id_buffer[4];
};

struct message_buffer{
    int read_counter;
    int message_counter;

    int sender_id;
    char mtext[BUFSIZE];
};

void receiver(int id, int semid, int manage_shmid, int message_shmid){
    struct sembuf wait[] = {{1,-1,0}, {2,-1,0}};
    struct sembuf signal[] = {{0,1,0}, {2,1,0}};
    struct sembuf p_buf[] = {{1,1,0}, {2,1,0}};
    struct manage_buffer *manage;
    struct message_buffer *message;
    int message_counter = 1;

    manage = (struct manage_buffer *)shmat(manage_shmid, 0, 0);
    message = (struct message_buffer *)shmat(message_shmid, 0, 0);

    while(1){
        semop(semid, wait, 2);

        if(message_counter == message[manage->back].message_counter){
            printf("[receiver] %d : %s\n", message[manage->back].sender_id, message[manage->back].mtext);
            message[manage->back].read_counter--;
            message_counter++;

            if(message[manage->back].read_counter == 0)
                manage->back = (manage->back + 1) % N;

            semop(semid, signal, 2);
        }
        else{
            semop(semid, p_buf, 2);
        }
    }
    return;
}

void sender(int id, int semid, int manage_shmid, int message_shmid){
    int message_counter = 0;
    char buffer[BUFSIZE];
    struct sembuf wait[] = {{0,-1,0}, {2,-1,0}};
    struct sembuf signal[] = {{1,1,0}, {2,1,0}};
    struct manage_buffer *manage;
    struct message_buffer *message;

    manage = (struct manage_buffer *)shmat(manage_shmid, 0, 0);
    message = (struct message_buffer *)shmat(message_shmid, 0, 0);

    while(gets(buffer)){
        semop(semid, wait, 2);

        message[manage->front].sender_id = id;
        strcpy(message[manage->front].mtext, buffer);
        message_counter+=1;
        message[manage->front].message_counter = message_counter;
        message[manage->front].read_counter = manage->total_user;
        manage->front = (manage->front + 1) % N;

        debug(manage, message);

        semop(semid, signal, 2);
    }

    return;
}

int main(int argc, char **argv){
    int i, id, semid, manage_shmid, message_shmid;
    key_t semkey, message_shmkey, manage_shmkey;
    pid_t pid;
    union semun arg;
    ushort buf[3] = {N, 0, 1};
    struct manage_buffer *manage;
    struct message_buffer *message;

    semkey = ftok("semkey", 3);
    manage_shmkey = ftok("manage_shmkey", 3);
    message_shmkey = ftok("message_shmkey", 3);

    semid = semget(semkey, 3, 0600|IPC_CREAT|IPC_EXCL);
    if(semid == -1){
        semid = semget(semkey, 3, 0600);
    }
    else{
        arg.array = buf;
        semctl(semid, 0, SETALL, arg);
    }

    message_shmid = shmget(message_shmkey, N*sizeof(struct message_buffer), 0600|IPC_CREAT);
    message = (struct message_buffer *)shmat(message_shmid, 0, 0);

    manage_shmid = shmget(manage_shmkey, sizeof(struct manage_buffer), 0600|IPC_CREAT);
    manage = (struct manage_buffer *)shmat(manage_shmid, 0, 0);

    id = atoi(argv[1]);
    manage->total_user+=1;

    debug(manage, message);

    printf("id : %d\n", id);
    pid = fork();
    if(pid == 0)
        receiver(id, semid, manage_shmid, message_shmid);
    else
        sender(id, semid, manage_shmid, message_shmid);

    while(waitpid(pid, 0, WNOHANG) == 0){
        sleep(1);
        printf("Wait receiver child ... \n");
    }

    manage->id_buffer[id-1] = 0;
    manage->total_user-=1;

    if(manage->total_user == 0){
        semctl(semid, IPC_RMID, 0);
        shmctl(manage_shmid, IPC_RMID, 0);
        shmctl(message_shmid, IPC_RMID, 0);
    }

    exit(1);
}

void debug(struct manage_buffer *manage, struct message_buffer *message) {
    int i;
    printf("[manage_buffer] front : %d, back : %d, users : %d\n", manage->front, manage->back, manage->total_user);
    printf("[id_buffer] : %d %d %d %d\n", manage->id_buffer[0], manage->id_buffer[1], manage->id_buffer[2],
           manage->id_buffer[3]);

    for (i = 0; i < N; i++) {
        printf("[%d : %s] ", (message + i)->sender_id, (message + i)->mtext);
    }
    printf("\n-----------------------------\n");
}

