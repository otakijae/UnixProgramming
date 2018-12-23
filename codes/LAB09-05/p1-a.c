#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    char ch = 'X';
    int i, fd = open("test1", O_WRONLY | O_CREAT, 0600);
    for(i=0;i<10;i++){
        write(fd, &ch, 1);
    }
    return 0;
}

