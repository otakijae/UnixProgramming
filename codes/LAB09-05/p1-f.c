#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    char ch = 'K';
    int i, fd = open("test3", O_WRONLY | O_CREAT | O_EXCL, 0644);

    if(fd < 0){
        printf("File opening failed");
    }
    else{
        for(i=0;i<10;i++){
            write(fd, &ch, 1);
        }
    }
    return 0;
}
