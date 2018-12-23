#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>
#include <stdlib.h>

#define BUFSIZE 512

int main(int argc, char **argv){

	int i, num, fd;
	char buffer[BUFSIZE] = {0};
	pid_t pid;

	fd = open(argv[1], O_RDONLY);
	num = atoi(argv[2]);

	for(i=0;i<num;i++){
		pid = fork();
		if(pid == 0){
			read(fd, buffer, 2*sizeof(char));
			printf("%s by %ld\n", buffer, getpid());
			exit(0);	
		}
	}
	
	for(i=0;i<num;i++){
		if(pid > 0){
			wait(0);
		}
	}

	return 0;
}
