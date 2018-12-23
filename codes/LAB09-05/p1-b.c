#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    int i, data = 35;
    int fd = open("test2", O_WRONLY | O_CREAT, 0640);
    for(i=0;i<10;i++){
        write(fd, &data, 4);
    }
    return 0;
}

