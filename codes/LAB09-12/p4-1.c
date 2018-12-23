#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){

    struct stat buffer;
    stat("data", &buffer);

    printf("access permission : %o\nlink : %d\nfile size : %ld\n", buffer.st_mode&0777, buffer.st_nlink, buffer.st_size);

    return 0;
}
