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
	printf("%d\n", argc);

	i = 0;

	while(argv[i] != NULL){
		printf("%s\n", argv[i]);
		i++;
	}

	return 0;
}
