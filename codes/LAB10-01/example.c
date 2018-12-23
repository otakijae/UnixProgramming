#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>
#include <stdlib.h>

//파일 포인터 fork후에 파일 포인터는 공유함
int main(int argc, char **argv){

	int fd;
	pid_t pid;
	char buf[10];

	fd = open("data", O_RDONLY);
	read(fd, buf, 10);
	printf("before fork : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));

	switch(pid = fork()){
		case -1:
			perror("fork failed\n");
			exit(1);
			break;
		case 0:
                        printf("child before read : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
			read(fd, buf, 10);
	                printf("child after read : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
			break;
		default:
			wait((int *) 0);
			printf("parent after wait : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
	}

	return 0;
}
