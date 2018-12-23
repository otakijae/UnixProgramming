#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

#define BUFSIZE 512

int main(){

    struct stat buffer;
    char file[BUFSIZE], dir[BUFSIZE], time[BUFSIZE];

    int n = read(0, file, BUFSIZE);

    file[n-1] = '\0';

    // file 존재 여부 파악
    if(stat(file, &buffer) < 0){
        perror("stat");
        return -1;
    }

    // symlink 파일 여부 파악
    if(readlink(file, dir, BUFSIZE) >= 0){
        lstat(file, &buffer);
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
    printf("%s ", file);

    printf("%s\n", (S_ISLNK(buffer.st_mode)) ? "symbolic link" : "");

    return 0;
}
