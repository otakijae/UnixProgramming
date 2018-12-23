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
	
	execl("./test1", "test1", "abc", "def","ghi", (char*)0);

	exit(1);
	
	return 0;
}
