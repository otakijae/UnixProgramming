#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    int data[10] = {0}, i, fd = open("test2", O_RDONLY);

    read(fd, data, 5 * sizeof(int));

    for(i=0;i<10;i++){
        printf("%-5d", data[i]);
    }

    return 0;
}

