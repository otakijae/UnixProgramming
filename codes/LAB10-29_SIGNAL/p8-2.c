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

void do_child(int i, int *pid_array){
	int j, pid;
	static struct sigaction act;

	act.sa_handler = catchusr;
	sigaction(SIGUSR1, &act, NULL);

	printf("%d-th child is created ... pid = %d\n", i, getpid());

	//생성 순서의 역순으로 작업 수행해야하니까, 첫 번째로 만들어진 child 이외의 child들은 pause로 기다림
	if(i<4)
		pause();

	pid = getpid();

	for(j=0;j<5;j++){
		printf("[child process id] : %d ...\n", getpid());
		sleep(1);
	}

	if(i>0){
		kill(pid_array[i-1], SIGUSR1);
	}

	exit(0);
}

void catchusr(int signo){
        printf(".....catch SIGUSR1 = %d\n", signo);
}

int main(int argc, char **argv){

	int i, status;
	pid_t pid[5];

	for(i=0;i<5;i++){
		pid[i] =fork();
		if(pid[i] == 0){
			do_child(i, pid);
		}
	}

	for(i=0;i<5;i++){
		wait(&status);
	}

	exit(0);
}

