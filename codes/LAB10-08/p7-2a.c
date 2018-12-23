#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <stdlib.h>

int main(int argc, char **argv){

	int i,j;

	for(i=0;i<5;i++){
		for(j=1;argv[j]!=NULL;j++){
			printf("%s ", argv[j]);
		}
		printf("\n");
		sleep(1);
	}
	exit(1);
}

