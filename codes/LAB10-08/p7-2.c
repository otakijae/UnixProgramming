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

	int i, status;
	pid_t pid;

	for(i=0;i<3;i++){
		pid = fork();
		if(pid == 0 && i == 0){
			execl("./p7-2a.out", "p7-2a.out", "1", "abcdef", (char*)0);
		}
		else if(pid == 0 && i == 1){
			execl("./p7-2b.out", "p7-2b.out", "2", (char*)0);
		}
		else if(pid == 0){
			 execl("./p7-2c.out", "p7-2c.out", "3", (char*)0);
		}
	}

	for(i=0;i<3;i++){
		wait(&status);
		if(WIFEXITED(status))
			printf("......%d\n", WEXITSTATUS(status));
	}

	exit(0);
}

