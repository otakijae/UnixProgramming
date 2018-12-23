#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFSIZE 512

int main() {
    char file1[100], file2[100], buffer[BUFSIZE];
    int index, user_input, fd1, fd2, nread;

    scanf("%100s %100s", file1, file2);
    fflush(stdin);

    fd1 = open(file1, O_RDONLY);

    if (fd1 == -1) {
        printf("file opening failed\n");
        return 0;
    } else {
        fd2 = open(file2, O_WRONLY | O_CREAT | O_EXCL, 0600);

        if(fd2 == -1){
            printf("Warning : Existing file can be modified. Are you sure to overwrite the file?\n");
            scanf("%d", &user_input);

            if(user_input == 1){
                fd2 = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            } else{
                printf("terminating");
                return 0;
            }
        }

        nread = read(fd1, buffer, BUFSIZE);
        while(nread > 0){
            write(fd2, buffer, nread);
            nread = read(fd1, buffer, BUFSIZE);
        }
    }

    return 0;
}

