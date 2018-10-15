# QUIZ

## quiz 20181010

```c
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

void cat_m(char **s){
    char buffer[BUFSIZE];
    int fd, nread;

    fd = open(s[1], O_RDONLY);

    nread = read(fd, buffer, BUFSIZE);
    while(nread > 0){
        write(1, buffer, nread);
        nread = read(fd, buffer, BUFSIZE);
    }

    close(fd);
}

void cd_m(char **s){
    chdir(s[1]);
}

void cp_m(char **s){
    char buffer[BUFSIZE];
    int fd1, fd2, nread;

    fd1 = open(s[1], O_RDONLY);

    if (fd1 == -1) {
        return;
    } else {
        fd2 = open(s[2], O_WRONLY | O_CREAT | O_EXCL, 0777);

        if(fd2 == -1){
            fd2 = open(s[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        }

        nread = read(fd1, buffer, BUFSIZE);
        while(nread > 0){
            write(fd2, buffer, nread);
            nread = read(fd1, buffer, BUFSIZE);
        }
    }

    close(fd1);
    close(fd2);
}

void mkdir_m(char **s){
    mkdir(s[1], 0777);
}

void ls_m(char **s){
    DIR *dp;
    struct dirent *d;
    dp = opendir(".");
    while((d = readdir(dp)) != NULL){
        if(d->d_name[0] != '.'){
            printf("%s\n", d->d_name);
        }
    }
}

void vi_m(char **s){
    char buffer[BUFSIZE];
    int fd1, nread;

    fd1 = open(s[1], O_RDONLY);

    if (fd1 == -1) {
        fd1 = open(s[1], O_WRONLY | O_CREAT, 0777);
        while(1) {
            int n = read(0, buffer, BUFSIZE);
            if (strncmp(buffer, "quit", 4) == 0) {
                return;
            }
            write(fd1, buffer, n);
        }
    } else {
        fd1 = open(s[1], O_RDWR);

        nread = read(fd1, buffer, BUFSIZE);
        while(nread > 0){
            write(1, buffer, nread);
            nread = read(fd1, buffer, BUFSIZE);
        }

        while(1) {
            int n = read(0, buffer, BUFSIZE);
            if (strncmp(buffer, "quit", 4) == 0) {
                return;
            }
            write(fd1, buffer, n);
        }
    }
}

void exec_m(char **s){
    execv(s[0], s);
}

int main(int argc, char **argv){

    char in[50], *res[20] = {0};
    char *inst[6] = {"cat", "cd", "cp", "mkdir", "ls", "vi"};
    void (*f[7])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, exec_m};
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
```

## quiz 20181015

- 수업시간에 팀원들이랑 제출한 결과

  ```c
  #include <fcntl.h>
  #include <unistd.h>
  #include <stdio.h>
  #include <string.h>
  #include <dirent.h>
  #include <ftw.h>
  #include <stdlib.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  
  void cat_m(char ** data){ /* 16010977 */
  
      char * filename = data[1];
      char buff[512];
      int fd = open(filename, O_RDONLY);
      int size;
  
      while(size = read(fd, buff, sizeof(buff))){
  
          write(0, buff, size);
  
      }
      close(fd);
  }
  
  void cd_m(char ** data){ /* 16010977 */
      chdir(data[1]);
  }
  
  void cp_m(char ** data){
      char * file_in = data[1];
      char * file_out = data[2];
      char buff[512];
      int in, out;
      int size;
  
      in = open(file_in, O_RDONLY);
      out = open(file_out, O_CREAT | O_WRONLY, 0644);
  
      while(size = read(in, buff, sizeof(buff))){
          write(out, buff, size);
      }
      close(in);
      close(out);
  }
  
  void mkdir_m(char ** data){ /* 16010977 */
      int mode = 0755;
      mkdir(data[1], mode);
  }
  
  void ls_m(char ** data){ /* 16010977 */
      DIR * dir;
      struct dirent *ent;
  
      dir = opendir(".");
  
      while(ent = readdir(dir)){
          printf("%s\n", ent->d_name);
      }
  
      closedir(dir);
  }
  
  void vi_m(char ** data){ /* 16010977  */
      int fd;
      char buff[512];
      int size;
      int flag = 0;
  
  
      fd = open(data[1], O_WRONLY | O_APPEND);
  
      if (fd != -1){
          cat_m(data);
  
      }
      else{
          fd = open(data[1], O_CREAT | O_WRONLY, 0644);
      }
  
      while(size = read(0, buff, sizeof(buff))){
          if (strncmp(buff+size-5, "quit", 4) == 0){
              size -= 5;
              flag = 1;
          }
          write(fd, buff, size);
          if (flag == 1) break;
      }
  
      close(fd);
  
  
  }
  
  void exec_m(char **s){   /* 16011025 */
      execv(s[0],s);
  }
  
  int main(void){
      char in[50], *res[20] = {0};
      char *inst[6]={"cat","cd","cp", "mkdir","ls","vi"};
      void (*f[7])(char **)={cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, exec_m };
      int i;
      pid_t pid;
  
      while(1){
          printf("> ");
          gets(in);
          i = 0;
          res[i] = strtok(in, " ");
          if (strcmp(res[0], "exit") ==0)
              exit(0);
  
          while(res[i]){
              i++;
              res[i] = strtok(NULL, " ");
          }
  
          for(i = 0; i < 6; i++){
              if (!strcmp(res[0], inst[i]))
                  break;
          }
          if (i == 1){
              cd_m(res);
          }
          else{
              pid = fork();
              if (pid == 0){
                  f[i](res);
                  if(i != 6) exit(0);
              }
              else{
                  wait(0);
              }
          }
      }
  }
  ```

- 내 코드에 적용한 결과

  ```c
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
  
  void cat_m(char **s){
      char buffer[BUFSIZE];
      int fd, nread;
  
      fd = open(s[1], O_RDONLY);
  
      nread = read(fd, buffer, BUFSIZE);
      while(nread > 0){
          write(1, buffer, nread);
          nread = read(fd, buffer, BUFSIZE);
      }
  
      close(fd);
  }
  
  void cd_m(char **s){
      chdir(s[1]);
  }
  
  void cp_m(char **s){
      char buffer[BUFSIZE];
      int fd1, fd2, nread;
  
      fd1 = open(s[1], O_RDONLY);
  
      if (fd1 == -1) {
          return;
      } else {
          fd2 = open(s[2], O_WRONLY | O_CREAT | O_EXCL, 0777);
  
          if(fd2 == -1){
              fd2 = open(s[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
          }
  
          nread = read(fd1, buffer, BUFSIZE);
          while(nread > 0){
              write(fd2, buffer, nread);
              nread = read(fd1, buffer, BUFSIZE);
          }
      }
  
      close(fd1);
      close(fd2);
  }
  
  void mkdir_m(char **s){
      mkdir(s[1], 0777);
  }
  
  void ls_m(char **s){
      DIR *dp;
      struct dirent *d;
      dp = opendir(".");
      while((d = readdir(dp)) != NULL){
          if(d->d_name[0] != '.'){
              printf("%s\n", d->d_name);
          }
      }
  }
  
  void vi_m(char **s){
      char buffer[BUFSIZE];
      int fd1, nread;
  
      fd1 = open(s[1], O_RDONLY);
  
      if (fd1 == -1) {
          fd1 = open(s[1], O_WRONLY | O_CREAT, 0777);
          while(1) {
              int n = read(0, buffer, BUFSIZE);
              if (strncmp(buffer, "quit", 4) == 0) {
                  return;
              }
              write(fd1, buffer, n);
          }
      } else {
          fd1 = open(s[1], O_RDWR);
  
          nread = read(fd1, buffer, BUFSIZE);
          while(nread > 0){
              write(1, buffer, nread);
              nread = read(fd1, buffer, BUFSIZE);
          }
  
          while(1) {
              int n = read(0, buffer, BUFSIZE);
              if (strncmp(buffer, "quit", 4) == 0) {
                  return;
              }
              write(fd1, buffer, n);
          }
      }
  }
  
  void exec_m(char **s){
      execv(s[0], s);
  }
  
  int main(int argc, char **argv){
  
      char in[50], *res[20] = {0};
      char *inst[6] = {"cat", "cd", "cp", "mkdir", "ls", "vi"};
      void (*f[7])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, exec_m};
      int i;
      pid_t pid;
  
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
  
          if(i == 1){
              cd_m(res);
          }
          else{
              pid = fork();
              if(pid == 0){
                  f[i](res);
                  if(i != 6){
                      exit(0);
                  }
              }
              else{
                  wait(0);
              }
          }
      }
  }
  ```

  - cd는 child process로 생성하고 parent로 돌아오면 위치가 원상복귀 되니까 fork()하지 않는다
  - cd 제외한 나머지 명령들은 child process로 실행한다.
  - 근데 i == 6 일 때에는 exit(0)을 실행 안 하게 설계했는데, 왜 그런지 물어봐야함

- 다음

