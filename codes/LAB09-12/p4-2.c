#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){

    struct stat buffer;

    if(access("data1", F_OK) == 0){
        if(access("data1", R_OK | W_OK) == 0){
            printf("파일 존재, 읽기 쓰기 모두 가능\n");
        } else{
            printf("파일 존재, 읽기 쓰기 불가\n");
        }
    } else{
        printf("파일 존재하지 않음\n");
    }

    return 0;
}
