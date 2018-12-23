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

void catchint(int);

int main(int argc, char **argv){
    int i, j, num[10], sum = 0;
    static struct sigaction act;
    sigset_t mask;

    act.sa_handler = catchint;
    sigaction(SIGINT, &act, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    for(i=0;i<5;i++){
	sigprocmask(SIG_SETMASK, &mask, NULL);
        scanf("%d", &num[i]);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
        sum += num[i];
        printf("num : %d, sum = %d\n", num[i], sum);
    }
    exit(0);
}

void catchint(int signo){
    printf("DO NOT INTERRUPT ... \n");
}

