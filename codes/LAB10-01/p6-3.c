#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>
#include <stdlib.h>

int main(int argc, char **argv){

	int i, num;

	pid_t pid;

	num = atoi(argv[1]);

	for(i=0;i<num;i++){
		pid = fork();
		if(pid == 0){
			printf("%ld : %ld : %ld\n", getpid(), getpgrp(), getsid(getpid()));
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
