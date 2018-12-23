#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    char ch = 'K';
    int i, fd = open("test1", O_WRONLY | O_TRUNC);
    for(i=0;i<3;i++){
        write(fd, &ch, 1);
    }
    return 0;
}

