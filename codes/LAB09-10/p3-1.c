
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(void){
    int i, fd, data1[10], data2[10];
    
    for (i=0;i<10;i++){
        scanf("%d", &data1[i]);
    }
   
    fd = open("data1", O_RDWR | O_CREAT, 0600);

    // 배열 data1[]에 저장 된 정수를 “data1" 파일에 저장한다.
    write(fd, data1, 10 * sizeof(int));

    //쓰고나면 파일 포인터가 제일 마지막에 가있기 때문에 lseek사용해서 포인터 위치 변경해주면 됨
    lseek(fd, 0, SEEK_SET);

    // “data1" 파일에 저장된 정수를 읽어 data2[] 배열에 저장한다.
    read(fd, data2, 10 * sizeof(int));
   
    for (i=0;i<10;i++){
        printf("%-5d", data1[i]);
    }
 
    printf("\n");
    
    for (i=0;i<10;i++){
        printf("%-5d", data2[i]);
    }
    
    printf("\n");
    
    return 0;
}

