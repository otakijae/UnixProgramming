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

#define BUFSIZE 512

void do_child(int id, int p[3][2]){
    int i, in, pid = getpid();

    for(i=0;i<3;i++){
        close(p[i][1]);
        if(id != i)
            close(p[i][0]);
    }

    while(1){
        read(p[id][0], &in, sizeof(int));
        if(in == -1)
            exit(0);
        else
	    printf("id : %d, pid : %d, input : %d\n", id, pid, in);
    }
}

int main(int argc, char **argv){
    int i, in, pid, p[3][2];

    for(i=0;i<3;i++){
        pipe(p[i]);
    }

    for(i=0;i<3;i++){
        if(fork() == 0){
            do_child(i, p);
        }
    }

    for(i=0;i<3;i++){
        close(p[i][0]);
    }

    for(i=0;i<12;i++){
        scanf("%d", &in);
        write(p[i%3][1], &in, sizeof(int));
    }

    in = -1;

    for(i=0;i<3;i++){
        write(p[i][1], &in, sizeof(int));
    }

    for(i=0;i<3;i++){
        wait(0);
    }

    exit(0);
}
