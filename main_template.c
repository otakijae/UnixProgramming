#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <stdlib.h>

#define BUFSIZE 512

void A(char **){
    printf("AAA\n");
}

void B(char **){
    printf("BBB\n");
}

void C(char **){
    printf("CCC\n");
}

void D(char **){
    printf("DDD\n");
}

void E(char **){
    printf("EEE\n");
}

void F(char **){
    printf("FFF\n");
}

void G(char **argv){
    int i;
    for(i = 0;argv[i]!=NULL;i++){
        printf("%s ", argv[i]);
    }
}

int main(int argc, char **argv){

    char in[50], *res[20] = {0};
    char *inst[6] = {"A", "B", "C", "D", "E", "F"};
    void (*f[7])(char **) = {A,B,C,D,E,F,G};
    int i;

    while(1){
        printf("> ");
        gets(in);
        i = 0;

        //입력된 문자열을 단어로 나눠서 res[] 배열에 저장
        res[i] = strtok(in, " ");
        if(strcmp(res[0], "exit")==0)
            exit(0);

        while(res[i]){
            i++;
            res[i] = strtok(NULL, " ");
        }

        for(i=0;i<6;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }
        f[i](res);
    }
}

/*
//strtok함수는 방금전에 분리한 문자열의 분리지점 바로 다음 주소값을 기억하고 있다.
//첫 번째 인자로 문자열의 주소값이 들어오면 거기서부터 분리를 시도,
//만약 NULL이 들어오면 이전에 자기가 기억한 곳으로부터 분리를 시도한다.

int main(int argc, char **argv){
  char s1[30] = "The Little Prince";
  char *ptr = strtok(s1, " ");
  while(ptr != NULL){
    printf("%s\n", ptr);
    ptr = strtok(NULL, " ");
  }
  return 0;
}
*/
