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

void catchalarm(int);

int main(int argc, char **argv){

	int i, num, sum = 0;
	static struct sigaction act;

	act.sa_handler = catchalarm;
	sigaction(SIGALRM, &act, NULL);

	for(i=0;i<10;i++){
		do{
			alarm(10);
		}while(scanf("%d", &num) < 0);
		alarm(0);
		sum += num;
		printf("sum = %d\n", sum);
	} 
	exit(0);
}

void catchalarm(int signo){
        printf("[Warning]No input for 10 seconds ... %d\n", signo);
}

