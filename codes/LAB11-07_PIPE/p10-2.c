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

void do_child(int id, int pipe[2][2]){
	char buffer = 'a';
	int i, pid = getpid();

	if(id < 2){
		read(pipe[id][0], &buffer, 1);
	}

	for(i=0;i<5;i++){
		sleep(1);
		printf("id : %d, pid : %d\n", id, pid);
	}

	if(id > 0){
		write(pipe[id-1][1], &buffer, 1);
	}
	
	exit(0);
}

int main(int argc, char **argv){

	int i, status, fd, p[2][2];
        pid_t pid[3];

	for(i=0;i<2;i++){
		pipe(p[i]);
	}
	
	for(i=0;i<3;i++){
                pid[i] =fork();
                if(pid[i] == 0){
                        do_child(i, p);
                }
        }

        for(i=0;i<3;i++){
                wait(&status);
        }

	exit(0);
}
