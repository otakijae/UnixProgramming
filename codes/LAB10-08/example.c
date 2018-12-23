#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <stdlib.h>

//child 프로세스 exit결과 출력
int main(int argc, char **argv){
	
	pid_t pid;
	int status, exit_status;

	if((pid=fork()) < 0){
		perror("fork failed");
		exit(1);
	}

	if(pid == 0){
		sleep(4);
		exit(5);
	}

	if((pid = wait(&status)) == -1){
		perror("wait failed");
		exit(2);
	}

	if(WIFEXITED(status)){
		exit_status = WEXITSTATUS(status);
		printf("Exit status from %d was %d\n", pid, exit_status);
	}
	
	exit(0);
}
