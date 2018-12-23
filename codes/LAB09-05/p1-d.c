#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    char ch = 'Z';
    int i, fd = open("test1", O_WRONLY | O_APPEND);
    for(i=0;i<5;i++){
        write(fd, &ch, 1);
    }
    return 0;
}

