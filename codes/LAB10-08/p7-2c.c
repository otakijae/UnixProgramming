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

	char buffer[BUFSIZE];
	int fd,i;

	fd = open("data", O_RDONLY);

	for(i=0;i<10;i++){
		read(fd, buffer, 1);
		write(1, buffer, 1);
		write(1, "\n", 1);
		sleep(1);
	}
	exit(3);
}

