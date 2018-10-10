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

