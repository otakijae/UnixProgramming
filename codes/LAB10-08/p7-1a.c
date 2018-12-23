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

	int i,j;

	for(i=0;i<3;i++){
		for(j=1;argv[j]!=NULL;j++){
                	printf("%s ", argv[j]);
		}
		printf("\n");
	}
	return 0;
}

