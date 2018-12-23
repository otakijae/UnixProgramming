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

	int i;

	for(i=0;i<5;i++){
		printf("pid = %d gid = %d sid = %d\n", getpid(), getpgid(0), getsid(getpid()));
		sleep(1);
	}
	exit(2);
}

