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
#define MSGSIZE 6

int main(int argc, char **argv){

	char fifo[2][3] = {"f1", "f2"};
	int i, in, fd[2];
	
	for(i=0;i<2;i++){
		mkfifo(fifo[i], 0600);
	}

	fd[0] = open(fifo[0], O_WRONLY);
	fd[1] = open(fifo[1], O_RDONLY);

	for(;;){
		scanf("%d", &in);
		write(fd[0], &in, sizeof(int));
		if(in == -1)
			exit(0);
		read(fd[1], &in, sizeof(int));
		printf("final A received : %d\n", in);
	}
	
	exit(0);
}
