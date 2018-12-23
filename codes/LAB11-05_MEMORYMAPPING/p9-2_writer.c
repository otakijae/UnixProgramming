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
#include <sys/mman.h>

#define BUFSIZE 512

int main(int argc, char **argv){
	
	int i, fd, len = 0;
	char *addr;
	
	fd = open("temp", O_RDWR | O_CREAT, 0600);

	addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	for(i=0;i<3;i++){
		sleep(3);
		len = len + write(1, addr + len, 512);
		printf("[write len] : %d\n", len);
		write(1, "-------\n", 8);
		if(len > BUFSIZE)
			break;
	}
	
	exit(0);
}
