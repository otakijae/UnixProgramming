#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<time.h>

#define BUFSIZE 512

int main(){

    struct stat buffer;
    char file[BUFSIZE], time[BUFSIZE];
    int n;

    DIR *dp;
    struct dirent *d;
    dp = opendir(".");
    while((d = readdir(dp)) != NULL){

        if(d->d_name[0] != '.'){

            stat(d->d_name, &buffer);

            if(S_ISREG(buffer.st_mode)){
                printf("F");
            } else{
                printf("D");
            }
            
            strcpy(time, ctime(&buffer.st_mtime));
            n = strlen(time);
            time[n-1] = '\0';

            printf("%o ", buffer.st_mode&0777);
            printf("%d ", buffer.st_nlink);
            printf("%d ", buffer.st_uid);
            printf("%d ", buffer.st_gid);
            printf("%ld ", buffer.st_size);
            printf("%s ", time);
            printf("%s ", d->d_name);
            printf("%s\n", (S_ISLNK(buffer.st_mode)) ? "symbolic link" : "");
        }
    }
    return 0;
}
