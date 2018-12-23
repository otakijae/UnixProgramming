#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){

    struct stat symlink_buffer, original_buffer;

    char symlink_file[100], ch[100] = {0}, original_file[100];

    scanf("%100s", symlink_file);
    fflush(stdin);

    readlink(symlink_file, original_file, 99);

    lstat(symlink_file, &symlink_buffer);
    stat(original_file, &original_buffer);

    printf("[symbolic file][%s] access permission : %o, file size : %ld\n", symlink_file, symlink_buffer.st_mode&0777, symlink_buffer.st_size);
    printf("[original file][%s] access permission : %o, file size : %ld\n", original_file, original_buffer.st_mode&0777, original_buffer.st_size);

    return 0;

}
