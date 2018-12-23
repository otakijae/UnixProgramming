#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>

#define BUFSIZE 512

void change_directory(char *name){
    chdir(name);
}

void list(){
    DIR *dp;
    struct dirent *d;
    dp = opendir(".");
    while((d = readdir(dp)) != NULL){
        if(d->d_name[0] != '.'){
            printf("%ld : %s \n", d->d_ino, d->d_name);
        }
    }
}

int main(){

    char name[BUFSIZE];

    while(1) {
        getcwd(name, BUFSIZE);
        printf("[ %s ]$ ", name);

        scanf("%s", name);
        if (strcmp(name, "cd") == 0) {
            scanf("%s", name);
            change_directory(name);
        }
        else if (strcmp(name, "ls") == 0)
            list();
        else
            break;
    }
    return 0;
}
