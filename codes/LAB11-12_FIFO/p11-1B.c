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

	fd[0] = open(fifo[0], O_RDONLY);
        fd[1] = open(fifo[1], O_WRONLY);

	for(;;){
		read(fd[0], &in, sizeof(int));
		printf("first B received : %d\n", in);
		if(in == -1)
			exit(0);
		in = in + 8;
		write(fd[1], &in, sizeof(int));
	}

	exit(0);
}	
