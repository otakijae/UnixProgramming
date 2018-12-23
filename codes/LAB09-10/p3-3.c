#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){

    int i, fd, k, k1, data1[10], data2[10];

    fd=open("data3", O_RDWR|O_CREAT, 0600);

    for (i=0;i<10;i++){
        scanf("%d", &data1[i]);
    }

    for(i=0;i<5;i++){
        write(fd, data1 + i, sizeof(int));
        lseek(fd, 4, SEEK_CUR);
    }

    for(i=9;i>4;i--){
        lseek(fd, -4, SEEK_CUR);
        write(fd, data1 + i, sizeof(int));
        lseek(fd, -8, SEEK_CUR);
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, data2, 10 * sizeof(int));

    for (i=0;i<10;i++) {
        printf("%-5d", data1[i]);
    }

    printf("\n");

    for (i=0;i<10;i++){
        printf("%-5d", data2[i]);
    }

    printf("\n");

    return 0;
}
