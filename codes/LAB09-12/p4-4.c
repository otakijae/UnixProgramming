#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){

    char name1[100], name2[100];
    scanf("%100s %100s", name1, name2);
    fflush(stdin);

    symlink(name1, name2);

    return 0;
}
