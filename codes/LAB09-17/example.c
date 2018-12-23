#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>

int main(){
/*
	char ch[100] = {0};

	mkdir("T1", 0700);
	chdir("T1");

	mkdir("T2", 0700);
	chdir("T2");

	mkdir("T3", 0700);
	chdir("T3");

	getcwd(ch, 99);
	printf("%s\n", ch);
*/
	
	DIR *dp;
    	struct dirent *d;
    	dp = opendir(".");
    	d = readdir(dp);
    	while(d != NULL){
        	printf("%ld : %s \n", d->d_ino, d->d_name);
        	d = readdir(dp);
   	}
	
        return 0;
}
