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
#include <signal.h>

#define BUFSIZE 512

void catchusr(int);

void do_child(int i){
	int pid;
	static struct sigaction act;

	act.sa_handler = catchusr;
	sigaction(SIGUSR1, &act, NULL);

	printf("%d-th child is created...\n", i);

	pause();

	pid = getpid();

	for(i=0;i<3;i++){
		printf("[child process id] : %d ...\n", getpid());
	}

	exit(0);
}

void catchusr(int signo){
        printf(".....catch SIGUSR1 = %d\n", signo);
}

int main(int argc, char **argv){

	int i, k, status;
	pid_t pid[3];

	for(i=0;i<3;i++){
		pid[i] =fork();
		if(pid[i] == 0){
			do_child(i);
		}
	}

	for(i=0;i<3;i++){
		sleep(1);
		kill(pid[i], SIGUSR1);
	}

	for(i=0;i<3;i++){
		k = wait(&status);
		printf("child #%d is terminated...%d\n", k, WEXITSTATUS(status));
	}

	exit(0);
}

