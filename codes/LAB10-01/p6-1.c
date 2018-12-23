#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>
#include <stdlib.h>

int main(){
	pid_t pid;
	pid = fork();

	if(pid == 0){
	        printf("%ld : %ld : %ld \n", getpid(), getpgrp(), getsid(getpid()));	
	}

	return 0;
}
