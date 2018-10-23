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
  - i == 6 일 때에는 exit(0)을 실행 안 하게 설계
    - exec 함수군은 인자로 받은 다른 프로그램을 자신을 호출한 프로세스의 메모리에 덮어쓴다
      따라서 프로세스가 수행 중이던 기존 프로그램은 중지되어 없어지고 새로 덮어쓴 프로그램이 실행된다
      exec 함수군을 호출한 프로세스 자체가 바뀌므로, exec 함수를 호출해 성공하면 리턴값이 없다
      Exec 함수군은 fork함수와 연결해 fork로 생성한 자식 프로세스가 새로운 프로그램을 실행하도록 할 때 유용하다
    - fork 함수를 실행하면 생성한 자식 프로세서와 부모 프로세스는 같은 코드를 수행했다
      case로 부모 프로세스와 자식 프로세스가 수행할 코드를 분리했기 때문에 따로 실행한거지 기본적으로 같은 프로그램을 실행한다
    - 그러나 자식 프로세스에서 exec함수군을 호출하면 자식 프로세스는 부모 프로세스로부터 복사한 프로그램과는 다른 명령이나 프로그램을 실행할 수 있다
      쉘은 fork함수로 자식 프로세스를 만들고, 사용자가 입력한 명령이나 파일을 exec함수군을 사용해 자식 프로세스에서 실행한다
      이렇게 부모 프로세스와 자식 프로세스가 각기 다른 작업을 수행해야 할 때 fork와 exec함수를 함께 사용해야 한다
    - exec함수군들은 다른 프로세를 실행시킬 때 현재 프로세스 메모리 공간에 덮어 써버린다
      그래서 exec 함수가 성공하면 함수 이후에 작성한 코드들은 실행이 안 된다
      그러나 exec 함수가 실행에 실패하면 실행하려고 하는 프로세스가 메모리에 로딩이 안 되어 exec 함수 다음 코드들이 실행된다
      exec함수를 호출하고 원래의 프로세스가 사라져 버리는 걸 원치 않으니 fork를 한 후에 exec함수를 호출하면 된다

- 교수님 피드백 이후
  exec 함수군이 제대로 실행되지 않은 경우를 고려하여, 모든 경우에 exit(0)을 해주는게 안전하다고 함

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
  	        exit(0);
  	    }
  	    else{
  	        wait(0);
  	    }
  	}
      }
  }
  ```

- backup함수 도움 받아서 완성
  기존에 만든 cp를 활용을 못함. 그래서 다시 만듬

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
  
  int backup(const char *name, const struct stat *status, int type){
  
      char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE] = "./TEMP/";
      int fd1, fd2, i, nread;
  
      if(type == FTW_F){
  
          if(strncmp((char*)name, "./TEMP", 6) == 0)
              return 0;
  
          fd1 = open(name, O_RDONLY);
  
          strcpy(file1, name + 2);
  
          for(i=0;i<strlen(file1);i++){
              if(file1[i] == '/')
                  file1[i] = '_';
          }
  
          strcat(file2, file1);
  
          fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);
  
          nread = read(fd1, buffer, BUFSIZE);
          while(nread > 0){
              write(fd2, buffer, nread);
              nread = read(fd1, buffer, BUFSIZE);
          }
      }
  
      return 0;
  }
  ```

  - 시험범위 2-6장, 설계과제 1, 2-1, 2-2 backup 코딩까지
    월요일 필기, 수요일 실기
