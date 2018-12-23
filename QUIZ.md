# QUIZ

## QUIZ 20181010

- 첫 퀴즈 제출 결과

  ```
  ./QUIZ10-10 submitted.c, 13011022.c
  ```

## QUIZ 20181015

- 수업시간에 팀원들이랑 제출한 결과

  ```c
  ./QUIZ10-15 submitted.c, 4.c
  ```

- 내 코드에 적용한 결과

  ```c
  //./QUIZ10-15/13011022.c => 교수님 피드백 받은 결과로 수정 했음
  //원래는 아래와 같이 코드 설계했었음
  
  ...
  
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
  ./QUIZ10-15 submitted.c, 4.c, 13011022.c
  ```

## QUIZ 20181017

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

## 2-2 완성 코드(팀)

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
  
  
  int backup_file(const char * name, const struct stat * st, int type){
  
     char buff[512];
     char output[512] ="./TEMP/";
     int in, out;
     int i, size;
  
     if (type != FTW_F){
        return 0;
     }      
  
     if (strncmp(output, name, 7) == 0) return 0;
  
     strcpy(buff, name);
     strcat(output, name+2);
  
     for(i = 7; output[i]; i++){
        if (output[i] == '/')
           output[i] = '_';
     }
  
     in = open(buff, O_RDONLY);
     out = open(output, O_CREAT | O_WRONLY | O_TRUNC, st->st_mode);
  
     while(size = read(in, buff, sizeof(buff))){
        write(out, buff, size);
     }
  
     close(in);
     close(out);
  
     return 0;
  }
  
  void backup(){
     mkdir("TEMP", 0777);
     ftw(".", backup_file, 1);
  }
  
  void exec_m(char **s){   /* 16011025 */
     execv(s[0],s);
  }
  
  
  int main(void){
     char in[50], *res[20] = {0};
     char *inst[7]={"cat","cd","cp", "mkdir","ls","vi", "backup"};
     void (*f[8])(char **)={cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup, exec_m };
     int i;
     pid_t pid, b_pid;
     int background = 0;
  
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
  
        for(i = 0; i < 7; i++){
           if (!strcmp(res[0], inst[i]))
              break;
        }
        if (i == 1){
           cd_m(res);
        }
        else{
           if (i == 6){
              background++;
              b_pid = fork();
              if (b_pid == 0){
                 backup();
                 exit(0);
  
              }
           }
           else{
              pid = fork();
              if (pid == 0){
                 f[i](res);   
                 exit(0); 
              }
              else{
                 waitpid(pid, 0, 0);
              }
           }
        }
        if (background){
           if(waitpid(b_pid, 0, WNOHANG)){
              background = 0;
           }
        }
     }
  }
  ```

## 2-2 내 코드 다듬고, 중간고사 backup 및 rmall 추가

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
    int nread, fd = open(s[1], O_RDONLY);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
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

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
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
    int nread, fd = open(s[1], O_RDWR|O_CREAT, 0777);

    //있으면 파일 안 만들고, 없으면 파일을 만들어서, 파일에 아무것도 안 적혀있든 적혀있든 다 출력을 해주고,
    //그러면 파일 포인터가 제일 뒤로 가있어서 바로 그 뒤에 이어적을 내용들을 적어주면 된다.ㄴ

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
    }
    while((nread = read(0, buffer, BUFSIZE)) > 0){
        //이렇게 마지막 new line 문자도 체크를 해주면 더 확실한 종료 코드라고 볼 수 있음
        if((strncmp(buffer, "quit", 4) == 0) && (buffer[4] == '\n'))
            break;
        write(fd, buffer, nread);
    }
}

////지정 TEMP 폴더에만 백업 실행
//int backup(const char *name, const struct stat *status, int type){
//    char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE] = "./TEMP/";
//    int fd1, fd2, i, nread;
//
//    if(type == FTW_F){
//        if(strncmp((char*)name, "./TEMP", 6) == 0)
//            return 0;
//
//        fd1 = open(name, O_RDONLY);
//
//        strcpy(file1, name + 2);
//
//        for(i=0;i<strlen(file1);i++){
//            if(file1[i] == '/')
//                file1[i] = '_';
//        }
//
//        strcat(file2, file1);
//
//        fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);
//
//        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
//            write(fd2, buffer, nread);
//        }
//    }
//    return 0;
//}

char b_dir[BUFSIZE] = "./";

//인자로 넘겨준 폴더 생성 후 백업 실행
int backup(const char *name, const struct stat *status, int type){
    char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE];
    int fd1, fd2, i, nread;

    strcpy(file2, b_dir);

    if(type == FTW_F){
        if(strncmp((char*)name, file2, strlen(file2)) == 0)
            return 0;

        fd1 = open(name, O_RDONLY);
        strcpy(file1, name + 2);

        for(i=0;i<strlen(file1);i++){
            if(file1[i] == '/')
                file1[i] = '_';
        }

        strcat(file2, file1);
        fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
        }
    }
    return 0;
}

void backup_m(char **s){
    strcat(b_dir, s[1]);
    strcat(b_dir, "/");
    mkdir(b_dir, 0777);
    ftw(".", backup, 1);
}

void exec_m(char **s){
    execv(s[0], s);
}

int flag;

int delete_all(const char *name, const struct stat *status, int type){

    int cnt = 0;
    DIR *dp;
    struct dirent *d;

    if(type == FTW_F){
        remove(name);
    }
    else if(type == FTW_D){
        dp = opendir(name);
        while(d = readdir(dp))
            cnt++;
        if(cnt <= 2)
            rmdir(name);
        else
            flag = 1;
    }
    return 0;
}

void rmall_m(char **s){
    do{
        flag = 0;
        ftw(s[1], delete_all, 1);
    }while(flag == 1);
}

//int delete_file(const char *name, const struct stat *status, int type){
//
//    if(type == FTW_F){
//        unlink(name);
//    }
//
//    return 0;
//}
//
//int delete_directory(const char *name, const struct stat *status, int type){
//
//    int k;
//    struct dirent *d;
//    DIR *dp;
//
//    if(type == FTW_D){
//        rmdir(name);
//    }
//
//    return 0;
//}
//
//void rmall_m(char **s){
//    ftw(s[1], delete_file, 1);
//    ftw(s[1], delete_directory, 1);
//}

int main(int argc, char **argv){
    char in[50], *res[20] = {0};
    char *inst[8] = {"cat", "cd", "cp", "mkdir", "ls", "vi", "backup", "rmall"};
    void (*f[9])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup_m, rmall_m, exec_m};
    int i, background = 0;
    pid_t pid, b_pid;

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

        for(i=0;i<8;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }

        if(i == 1){
            cd_m(res);
        }
        else{
            if (i == 6){
                background++;
                b_pid = fork();
                if (b_pid == 0){
                    f[i](res);
                    exit(0);

                }
            }
            else{
                pid = fork();
                if (pid == 0){
                    f[i](res);
                    exit(0);
                }
                else{
                    waitpid(pid, 0, 0);
                }
            }
        }
        if (background){
            if(waitpid(b_pid, 0, WNOHANG)){
                background = 0;
            }
        }
    }
}
```

- 시험 주의 사항
  - Backup 함수 실행할 때 backup한 파일들도 계속해서 복사/붙이기를 할 필요가 없음. 그래서 backup을 한 디렉토리 폴더를 0333 permission을 주면 FTW_DNR 디렉토리로 되어서 복사를 못하게 된다.
  - rmall 함수 실행할 때 시험 중인 디렉토리 안 지우게 설정을 해주려면 0555로 설정을 해주면 된다.

## 20181128 3-1 설계과제

- 시험은 망했지만 다시 풀어봤음.
  - 근데 다시 풀어본 코드에서는 ctrl + c 시그널을 무시를 하긴하는데 child 프로세스를 종료하지 않아서 잘못된 코드였음
  - 백업 기능 실행할 때 exit하게 되면 시그널 보내서 작업 도중에는 시그널 잠시 무시했다가 작업 끝나면 블록 풀어줘서 시그널 액션 처리하게 만든거는 맞는 것 같다. 어차피 제출 코드에는 안 돼있어서 점수는 없다.

- 20181212 수업 때 교수님이 다시 만들어오면 점수를 준다고 해서 다시 해봤음
- 수정사항
  - 기본적으로 parent에서는 ctrl + c 시그널을 무시하게 설정해두면, shell process가 실행하는 cd 및 중간에 종료되면 안 되는 backup 명령 작업은 ctrl + c 시그널을 무시함
  - child 프로세스로 실행되는 나머지 명령들은 ctrl + c default signal action을 취할 수 있도록 설정해주고 함수 실행.
  - Child 프로세스만 들어갈 수 있는 구간에서 설정을 해주기 때문에 parent signal 설정에는 영향이 없다

```c
#define BUFSIZE 512

void catchusr(int);
void catchusr2(int);
void catchalarm(int);

void cat_m(char **s){
    char buffer[BUFSIZE];
    int nread, fd = open(s[1], O_RDONLY);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
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

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
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
    int nread, fd = open(s[1], O_RDWR|O_CREAT, 0777);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
    }
    while((nread = read(0, buffer, BUFSIZE)) > 0){
        if((strncmp(buffer, "quit", 4) == 0) && (buffer[4] == '\n'))
            break;
        write(fd, buffer, nread);
    }
}

int backup(const char *name, const struct stat *status, int type){
    char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE] = "./TEMP/";
    int fd1, fd2, i, nread;

    if(type == FTW_F){
        fd1 = open(name, O_RDONLY);
        strcpy(file1, name + 2);

        for(i=0;i<strlen(file1);i++){
            if(file1[i] == '/')
                file1[i] = '_';
        }

        strcat(file2, file1);
        fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
        }
    }
    return 0;
}

void backup_m(char **s){
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);

    while(1){
        //backup 도중에는 user signal2 는 block 됐다가, unblock 후에 signal handle action을 실행하게 sigprocmask 사용
        sigprocmask(SIG_SETMASK, &mask, NULL);
        printf("*****BACK-UP STARTS*****\n");
        mkdir("TEMP", 0333);
        sleep(5);
        ftw(".", backup, 1);
        printf("*****BACK-UP ENDS*****\n");
        sigprocmask(SIG_UNBLOCK, &mask, NULL);

        alarm(10);
        //pause를 안 해주면 10초를 안 쉬고 알람 보내고 바로 다시 backup작업을 하기 때문에 pause로 블락시키고, alarm signal handle할때 pause를 풀어주면 된다.
        pause();
    }
}

void exec_m(char **s){
    execv(s[0], s);
}

//pause를 풀어주기 위한 signal action
void catchusr(int signo){
    return ;
}

//parent에서 exit을 입력받으면 backup child 프로세스로 신호를 보냄
void catchusr2(int signo){
    exit(1);
}

//10초 알람 후에 pause를 풀어주기 위한 signal 보내는 것
void catchalarm(int signo){
    kill(getpid(), SIGUSR1);
}

int main(int argc, char **argv){
    char in[50], *res[20] = {0};
    char *inst[7] = {"cat", "cd", "cp", "mkdir", "ls", "vi", "backup"};
    void (*f[8])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup_m, exec_m};
    int i, status;
    pid_t pid, b_pid;

    static struct sigaction act;

    //기본적으로 parent에서는 ctrl + c 시그널을 무시하게 설정해두면, shell process가 실행하는 cd 및 중간에 종료되면 안 되는 backup 명령 작업은 ctrl + c 시그널을 무시함
    act.sa_handler = SIG_IGN;
    sigaction(SIGINT, &act, NULL);

    act.sa_handler = catchalarm;
    sigaction(SIGALRM, &act, NULL);

    act.sa_handler = catchusr;
    sigaction(SIGUSR1, &act, NULL);

    act.sa_handler = catchusr2;
    sigaction(SIGUSR2, &act, NULL);

    while(1){
        printf("> ");
        gets(in);
        i = 0;
        res[i] = strtok(in, " ");
        if(strcmp(res[0], "exit")==0){
            kill(b_pid, SIGUSR2);
            while(waitpid(b_pid, 0, WNOHANG) == 0){
                printf(".....wait for backup .....\n");
                sleep(1);
            }
            exit(0);
        }

        while(res[i]){
            i++;
            res[i] = strtok(NULL, " ");
        }

        for(i=0;i<7;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }

        if(i == 1){
            cd_m(res);
        }
        else{
            if (i == 6){
                b_pid = fork();
                if (b_pid == 0){
                    f[i](res);
                    exit(0);
                }
                else{
                    waitpid(b_pid, 0, WNOHANG);
                }
            }
            else{
                pid = fork();
                if (pid == 0){
                    //child 프로세스로 실행되는 나머지 명령들은 ctrl + c default signal action을 취할 수 있도록 설정해주고 함수 실행
                    act.sa_handler = SIG_DFL;
                    sigaction(SIGINT, &act, NULL);

                    f[i](res);
                    exit(0);
                }
                else{
                    waitpid(pid, &status, 0);
                }
            }
        }

        //시그널을 받고 종료한 pid인 경우 마지막에 newline이 출력 안 되기 때문에, 출력 이쁘게 하기 위해서 추가해줌
        if(WIFSIGNALED(status))
            printf("\n");

    }
}
```

## 20181203 4-1 설계과제

### 교수님이 의도하지 않은 방법으로 만든거

- 채팅 프로그램 먼저 한 번 코딩해봤음

  - 일단 메세지 큐만 이용하여 채팅에 참여한 사람들에게 안 섞이고 잘 전달하게는 만들었는데,,,
  - 실시간으로 채팅 참여한 인원을 파악해주는 기능을 더 손을 봐줘야함.

  ```c
  #define BUFSIZE 512
  
  int total_user, id_counter;
  
  struct q_entry{
      long mtype;
      int id_counter;
      int total_user;
  };
  
  struct q_data{
      long mtype;
      int id;
      char mtext[BUFSIZE];
  };
  
  void receiver(int id, int qid){
      struct q_entry user_count;
      struct q_data user_message;
  
      while(msgrcv(qid, &user_message, sizeof(struct q_data), 3, 0) > 0){
          msgrcv(qid, &user_message, sizeof(struct q_data), 2, 0);
  
          if(user_message.id != id)
              printf("[received] id : %d, message : %s\n", user_message.id, user_message.mtext);
          else{
              if ((strncmp(user_message.mtext, "talk_quit", 9) == 0) && (user_message.mtext[9] == '\n')) {
                  exit(1);
              }
          }
      }
  
      exit(1);
  }
  
  void sender(int id, int qid){
      int i;
      char buffer[BUFSIZE];
      struct q_entry user_count;
      struct q_data user_message;
  
      while((read(0, buffer, BUFSIZE)) > 0) {
          msgrcv(qid, &user_count, 2*sizeof(int), 1, IPC_NOWAIT);
          user_count.mtype = 1;
          id_counter = user_count.id_counter;
          total_user = user_count.total_user;
          msgsnd(qid, &user_count, 2*sizeof(int), 0);
  
          if(total_user == 1){
              printf("talk_wait ... \n");
          }
  
          if ((strncmp(buffer, "talk_quit", 9) == 0) && (buffer[9] == '\n')) {
              for(i=0;i<total_user;i++){
                  user_message.mtype = 3;
                  user_message.id = id;
                  strcpy(user_message.mtext, buffer);
                  msgsnd(qid, &user_message, sizeof(struct q_data), 0);
              }
              for(i=0;i<total_user;i++){
                  user_message.mtype = 2;
                  user_message.id = id;
                  strcpy(user_message.mtext, buffer);
                  msgsnd(qid, &user_message, sizeof(struct q_data), 0);
              }
              return;
          }
  
          for(i=0;i<total_user;i++){
              user_message.mtype = 3;
              user_message.id = id;
              strcpy(user_message.mtext, buffer);
              msgsnd(qid, &user_message, sizeof(struct q_data), 0);
          }
          for(i=0;i<total_user;i++){
              user_message.mtype = 2;
              user_message.id = id;
              strcpy(user_message.mtext, buffer);
              msgsnd(qid, &user_message, sizeof(struct q_data), 0);
          }
  
          strcpy(buffer, "\0");
      }
  
      return;
  }
  
  int main(int argc, char **argv){
      int i, id, qid;
      char buffer[BUFSIZE];
      key_t key;
      pid_t pid;
      struct q_entry user_count;
      struct q_data user_message;
  
      key = ftok("keyfile", 1);
      qid = msgget(key, 0600|IPC_CREAT);
  
      if(msgrcv(qid, &user_count, 2*sizeof(int), 1, IPC_NOWAIT) < 0){
          id_counter = 1;
          total_user = 1;
  
          user_count.mtype = 1;
          user_count.id_counter = id_counter;
          user_count.total_user = total_user;
          id = id_counter;
          msgsnd(qid, &user_count, 2*sizeof(int), 0);
      }
      else{
          id_counter = user_count.id_counter + 1;
          total_user = user_count.total_user + 1;
  
          user_count.mtype = 1;
          user_count.id_counter = id_counter;
          user_count.total_user = total_user;
          id = id_counter;
          msgsnd(qid, &user_count, 2*sizeof(int), 0);
      }
  
      printf("id = %d\n", id);
  
      pid = fork();
      if(pid == 0){
          receiver(id, qid);
      }
      else{
          waitpid(pid, 0, WNOHANG);
          sender(id, qid);
      }
  
      msgrcv(qid, &user_count, 2*sizeof(int), 1, IPC_NOWAIT);
      id_counter = user_count.id_counter;
      total_user = user_count.total_user;
  
      user_count.mtype = 1;
      user_count.id_counter = id_counter;
      user_count.total_user = total_user - 1;
      msgsnd(qid, &user_count, 2*sizeof(int), 0);
  
      if(user_count.total_user == 0){
          printf("no user exists\n");
          msgctl(qid, IPC_RMID, 0);
      }
  
      exit(1);
  }
  ```

### 교수님 피드백 받고 다시 만든 버전

- 설계 방법
  - mtype = 1인 경우는 채팅참가하고 있는 총인원 관리, id 카운터, message 카운터를 관리
  - mtype = 2인 경우는 메세지 데이터 전달 관리
  - mtype = 1을 제외한 2부터 메세지를 보내면 카운터를 하나씩 올려서 보낸 순서를 지정해주어 읽을 때 메세지 카운터 순서대로 읽게 하여 메세지가 섞이지 않게 설정함
  - 문자열 제대로 전달되게 gets(buffer)로 수정
- 개선해야할 사항
  - talk_wait, talk_start 부분

```c
#define BUFSIZE 512

int id_counter, total_user, message_counter;

struct q_entry{
    long mtype;
    int id_counter;
    int total_user;
    int message_counter;
};

struct q_data{
    long mtype;
    int id;
    char mtext[BUFSIZE];
};

void receiver(int id, int qid){
    struct q_entry user_count;
    struct q_data user_message;

    while(msgrcv(qid, &user_message, sizeof(struct q_data), message_counter, 0) > 0){
        if(user_message.id != id)
            printf("[received] id : %d, message : %s\n", user_message.id, user_message.mtext);
        else{
            if ((strcmp(user_message.mtext, "talk_quit") == 0)) {
                exit(1);
            }
        }
        message_counter++;
    }
    exit(1);
}

void sender(int id, int qid){
    int i;
    char buffer[BUFSIZE];
    struct q_entry user_count;
    struct q_data user_message;

    while(gets(buffer)) {
        msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
        user_count.mtype = 1;
        id_counter = user_count.id_counter;
        total_user = user_count.total_user;
        message_counter = user_count.message_counter;

        if(total_user == 1){
            printf("talk_wait ... \n");
        }

        if ((strcmp(buffer, "talk_quit") == 0)) {
            for(i=0;i<total_user;i++){
                user_message.mtype = message_counter;
                user_message.id = id;
                strcpy(user_message.mtext, buffer);
                msgsnd(qid, &user_message, sizeof(struct q_data), 0);
            }

            user_count.mtype = 1;
            message_counter++;
            user_count.message_counter = message_counter;
            msgsnd(qid, &user_count, 3*sizeof(int), 0);

            return;
        }
        else{
            for(i=0;i<total_user;i++){
                user_message.mtype = message_counter;
                user_message.id = id;
                strcpy(user_message.mtext, buffer);
                msgsnd(qid, &user_message, sizeof(struct q_data), 0);
            }

            user_count.mtype = 1;
            message_counter++;
            user_count.message_counter = message_counter;
            msgsnd(qid, &user_count, 3*sizeof(int), 0);
        }
    }
    return;
}

int main(int argc, char **argv){
    int id, qid;
    key_t key;
    pid_t pid;
    struct q_entry user_count;

    key = ftok("keyfile", 1);
    qid = msgget(key, 0600|IPC_CREAT);

    if(msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT) < 0){
        id_counter = 1;
        total_user = 1;
        message_counter = 2;

        user_count.mtype = 1;
        user_count.id_counter = id_counter;
        user_count.total_user = total_user;
        user_count.message_counter = message_counter;
        id = id_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }
    else{
        id_counter = user_count.id_counter + 1;
        total_user = user_count.total_user + 1;
        message_counter = user_count.message_counter;

        user_count.mtype = 1;
        user_count.id_counter = id_counter;
        user_count.total_user = total_user;
        user_count.message_counter = message_counter;
        id = id_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }

    printf("id = %d\n", id);

    pid = fork();
    if(pid == 0){
        receiver(id, qid);
    }
    else{
        waitpid(pid, 0, WNOHANG);
        sender(id, qid);
    }

    msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
    id_counter = user_count.id_counter;
    total_user = user_count.total_user;
    message_counter = user_count.message_counter;

    user_count.mtype = 1;
    user_count.id_counter = id_counter;
    user_count.total_user = total_user - 1;
    user_count.message_counter = message_counter;
    msgsnd(qid, &user_count, 3*sizeof(int), 0);

    if(user_count.total_user == 0){
        printf("no user exists\n");
        msgctl(qid, IPC_RMID, 0);
    }

    exit(1);
}
```

### 20181210 최종 설계도 기반 코드

- talk_wait, talk_start 부분 수정
  - 2명으로 됐을 때 talk_start를 메세지로 보내서 다른 한 쪽에서도 채팅이 가능한 것을 알 수 있게 설정. 3명 이상일 때는 혼자만 출력.
- 개선사항
  - 근데 talk_start 그대로 입력하게 되면 2명 째 talk_start라고 인식해서 전체 다 id 출력 없이 talk_start만 출력
  - 설계도에 talk_start 메세지 보내는걸 작성 안 했음

```c
#define BUFSIZE 512

int id_counter, total_user, message_counter;

struct q_entry{
    long mtype;
    int id_counter;
    int total_user;
    int message_counter;
};

struct q_data{
    long mtype;
    int id;
    char mtext[BUFSIZE];
};

void receiver(int id, int qid){
    struct q_entry user_count;
    struct q_data user_message;

    while(msgrcv(qid, &user_message, sizeof(struct q_data), message_counter, 0) > 0){
        if(user_message.id != id){
            if(strcmp(user_message.mtext, "talk_start") == 0){
                printf("talk_start\n");
            }
            else{
                printf("%d : %s\n", user_message.id, user_message.mtext);
            }
        }
        else{
            if (strcmp(user_message.mtext, "talk_quit") == 0) {
                exit(1);
            }
        }
        message_counter++;
    }
    exit(1);
}

void sender(int id, int qid){
    int i;
    char buffer[BUFSIZE];
    struct q_entry user_count;
    struct q_data user_message;

    while(gets(buffer)) {
        msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
        user_count.mtype = 1;
        id_counter = user_count.id_counter;
        total_user = user_count.total_user;
        message_counter = user_count.message_counter;

        if ((strcmp(buffer, "talk_quit") == 0)) {
            for(i=0;i<total_user;i++){
                user_message.mtype = message_counter;
                user_message.id = id;
                strcpy(user_message.mtext, buffer);
                msgsnd(qid, &user_message, sizeof(struct q_data), 0);
            }

            user_count.mtype = 1;
            message_counter++;
            user_count.message_counter = message_counter;
            msgsnd(qid, &user_count, 3*sizeof(int), 0);

            return;
        }
        else{
            if(total_user == 1){
                printf("talk_wait ... \n");
            }

            for(i=0;i<total_user;i++){
                user_message.mtype = message_counter;
                user_message.id = id;
                strcpy(user_message.mtext, buffer);
                msgsnd(qid, &user_message, sizeof(struct q_data), 0);
            }

            user_count.mtype = 1;
            message_counter++;
            user_count.message_counter = message_counter;
            msgsnd(qid, &user_count, 3*sizeof(int), 0);
        }
    }
    return;
}

int main(int argc, char **argv){
    int i, id, qid;
    key_t key;
    pid_t pid;
    struct q_entry user_count;
    struct q_data user_message;

    key = ftok("keyfile", 1);
    qid = msgget(key, 0600|IPC_CREAT);

    //처음 들어오는 경우 메세지 큐에 메세지가 없는 상태이니까, msgrcv가 실패를 하게된다
    if(msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT) < 0){
        id_counter = 1;
        total_user = 1;
        message_counter = 2;

        user_count.mtype = 1;
        user_count.id_counter = id_counter;
        user_count.total_user = total_user;
        user_count.message_counter = message_counter;
        id = id_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }
    else{
        id_counter = user_count.id_counter + 1;
        total_user = user_count.total_user + 1;
        message_counter = user_count.message_counter;

        user_count.mtype = 1;
        user_count.id_counter = id_counter;
        user_count.total_user = total_user;
        user_count.message_counter = message_counter;
        id = id_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }

    printf("id = %d\n", id);

    if(total_user == 1){
        printf("talk_wait ... \n");
    }
    else if(total_user == 2){
        //본인 id의 receiver, sender 실행되고 있지 않아서 자기 자신은 미리 출력해줌
        printf("talk_start\n");

        msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
        user_count.mtype = 1;
        id_counter = user_count.id_counter;
        total_user = user_count.total_user;
        message_counter = user_count.message_counter;

        user_message.mtype = message_counter;
        user_message.id = id;
        strcpy(user_message.mtext, "talk_start");
        msgsnd(qid, &user_message, sizeof(struct q_data), 0);

        user_count.mtype = 1;
        message_counter++;
        user_count.message_counter = message_counter;
        msgsnd(qid, &user_count, 3*sizeof(int), 0);
    }
    else if(total_user > 2){
        printf("talk_start\n");
    }

    pid = fork();

    if(pid == 0){
        receiver(id, qid);
    }
    else{
        //굳이 안 해줘도 된다고 했음
        //waitpid(pid, 0, WNOHANG);
        sender(id, qid);
    }

    msgrcv(qid, &user_count, 3*sizeof(int), 1, IPC_NOWAIT);
    id_counter = user_count.id_counter;
    total_user = user_count.total_user;
    message_counter = user_count.message_counter;

    user_count.mtype = 1;
    user_count.id_counter = id_counter;
    user_count.total_user = total_user - 1;
    user_count.message_counter = message_counter;
    msgsnd(qid, &user_count, 3*sizeof(int), 0);

    //교수님 말씀으로는 굳이 pid(receiver)를 기다릴 필요가 없으니까 WNOHANG으로 반복적으로 확인 안 해줘도 된다고 해서 마지막으로 뺐음
    waitpid(pid, 0, 0);

    if(user_count.total_user == 0){
        msgctl(qid, IPC_RMID, 0);
    }

    exit(1);
}
```

## Producer / Consumer Problem with C

```c
#define BUFFER_SIZE 5
#define EMPTY_ID 0
#define FULL_ID 1
#define MUTEX_ID 2
#define NSEM_SIZE 3

#define SHM_KEY 9
#define SEM_KEY "."

static struct sembuf downEmpty = { EMPTY_ID, -1, 0 };
static struct sembuf upEmpty = { EMPTY_ID, 1, 0 };
static struct sembuf upFull = { FULL_ID, 1, 0 };
static struct sembuf downFull = { FULL_ID, -1, 0 };
static struct sembuf downMutex = { MUTEX_ID, -1, 0 };
static struct sembuf upMutex = { MUTEX_ID, 1, 0 };

int *create_shared_mem_buffer();
int create_semaphore_set();
int get_buffer_size(int *sbuff);
void clear_buffer(int *sbuf);

// producer
void insert_item(int item, int semid, int *shared_buffer) {
    int index = get_buffer_size(shared_buffer);
    shared_buffer[index] = item;
}

int produce_item() {
    return 0xFF; // nothing dynamic just write a static integer a slot
}

int main(int argc, const char *argv[]) {
    int *shared_buffer = create_shared_mem_buffer();
    int semid = create_semaphore_set();

    clear_buffer(shared_buffer); // prepare buffer for jobs

    int item = 0;

    while (1) {
        item = produce_item();
        semop(semid, &downEmpty, 1);
        semop(semid, &downMutex, 1);
        insert_item(item, semid, shared_buffer);
        debug_buffer(shared_buffer);
        semop(semid, &upMutex, 1);
        semop(semid, &upFull, 1);
    }

    return EXIT_SUCCESS;
}


// consumer
void consume_item(int item) {
    // do something with item
}

int remove_item(int semid, int *shared_buffer) {
    int index = get_buffer_size(shared_buffer) - 1;
    int item = shared_buffer[index];
    shared_buffer[index] = 0x00;
    return item;
}

int main(int argc, const char *argv[])
{
    int *shared_buffer = create_shared_mem_buffer();
    int semid = create_semaphore_set();

    int item = 0;

    while(1) {
        semop(semid, &downFull, 1);
        semop(semid, &downMutex, 1);
        item = remove_item(semid, shared_buffer);
        debug_buffer(shared_buffer);
        semop(semid, &upMutex, 1);
        semop(semid, &upEmpty, 1);
        consume_item(item);
    }

    return EXIT_SUCCESS;
}

/**
 * returns current size of shared buffer
 */
int get_buffer_size(int *sbuff) {
    int i = 0;
    int counter = 0;
    for (i = 0; i < BUFFER_SIZE; ++i) {
        if (sbuff[i] == 0xFF) {
            counter++;
        }
    }
    return counter;
}

void debug_buffer(int *sbuff) {
    int i = 0;
    for (i = 0; i < BUFFER_SIZE; ++i) {
        if (sbuff[i] == 0xFF) printf("1");
    }
    printf("\n");
}

/**
 * returns a pointer to a shared memory buffer that the
 * producer can write to.
 */
int *create_shared_mem_buffer() {
    int *shmaddr = 0; /* buffer address */
    key_t key = SHM_KEY; /* use key to access a shared memory segment */

    int shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | SHM_R | SHM_W); /* give create, read and write access */
    if (errno > 0) {
        perror("failed to create shared memory segment");
        exit (EXIT_FAILURE);
    }

    shmaddr = (int*)shmat(shmid, NULL, 0);
    if (errno > 0) {
        perror ("failed to attach to shared memory segment");
        exit (EXIT_FAILURE);
    }

    // clean out garbage memory in shared memory
    return shmaddr;
}

/**
 * only used in the producer to clean out garbage memory when
 * constructing initial buffer.
 */
void clear_buffer(int *sbuff) {
    int i = 0;
    for (i = 0; i < BUFFER_SIZE; ++i) sbuff[i] = 0x00;
}

/**
 * create FULL and EMPTY semaphores
 */
int create_semaphore_set() {
    key_t key = ftok(SEM_KEY, 'E');

    int semid = semget(key, NSEM_SIZE, 0600 | IPC_CREAT);
    if (errno > 0) {
        perror("failed to create semaphore array");
        exit (EXIT_FAILURE);
    }

    semctl(semid, FULL_ID, SETVAL, 0);
    if (errno > 0) {
        perror("failed to set FULL semaphore");
        exit (EXIT_FAILURE);
    }

    semctl(semid, EMPTY_ID, SETVAL, BUFFER_SIZE);
    if (errno > 0) {
        perror("failed to set EMPTY sempahore");
        exit (EXIT_FAILURE);
    }

    semctl(semid, MUTEX_ID, SETVAL, 1);
    if (errno > 0) {
        perror("failed to create mutex");
    }

    return semid;
}
```

- producer / consumer problem implemented with circular buffer in C.

```c
/*
 * The Bounded Buffer (aka Producer/Consumer) problem.
 * Demonstrates the UNIX / LINUX fork, shared memory, and semaphores.
 *
 * shmget - gets and returns a shared memory segment -- called only once.
 * shmat - attaches to the shared memory segment created by shmget; called by
 *	   all processes wishing to use the shared memory.
 * semop - performs operations on the shared memory: uses a sembuf structure:
 *	   unsigned short	sem_num;	semaphore number
 *	   short		sem_op;		semaphore operation
 *	   short		sem_flg;	operation flags
 *
 *	   A binary semaphore (i.e., available or not available) is used to
 *	   enforce mutual exclusion.  It is initialized to 1 (sem_op = 1).
 *	   A counting semaphore (used here to guarantee either spaces or
 *	   elements in the buffer) is initialized to the number of available
 *	   resources (the number of slots in the buffer in this example).
 *	   The semaphore is acquired by setting sem_op = -1 (if the current
 *	   semaphore value is >0, the count is decremented and the process
 *	   proceeds; if the semaphore value is 0, the process suspends).
 *	   The semaphore is released by setting sem_op = 1, which increments
 	   the semaphore value (this also resumes any suspended processes).
 */

/* The circular buffer is managed by two values:
 * a front ponter that points to the buffer location where the next
 * element will be inserted (by a producer); and
 * a back pointer from whence the next element will be removed (by a consumer).
 * These values are stored in a array of two integers in shared memory,
 * which is accessed by the symbolic constants defined below.
 * The current circular buffer has a capacity of 10.
 */

enum { FRONT, BACK, BUF_SIZE = 10 };

void producer(int);
void consumer(int);
void debug();

/* The id's are inherited by the children after the fork. */

/* shared memory ids */
int	var_id;		/* shared memory holding counter, front, and back */
int	buf_id;		/* buffer shared memroy segment */

/* semaphore id.
 * The semaphore id below will represent 3 separate semaphores in an array:
 * 0: counting semaphore, which is the number of free spaces in the buffer.
 * 1: counting semaphore, which is the number of elements in the buffer.
 * 2: binary semaphore, which enforces mutual exclusion to critical sections.
 */

int	sem_id;		/* semaphore id */

int main(int argc, char* argv[]) {
    int	i;
    int*	vars;			/* array of shared mem front & back  */
    struct	sembuf	init_sm[] = {		/* initialize the semaphores */
            { 0, BUF_SIZE, IPC_NOWAIT },	/* init # spaces = BUF_SIZE  */
            { 1, 0, IPC_NOWAIT },		/* init # elements = 0	     */
            { 2, 1, IPC_NOWAIT }		/* init mutual exclusion = 1 */
    };

    /* initialize the buffer control variables: front and back	     */
    var_id = shmget(IPC_PRIVATE, 2 * sizeof(int), S_IRUSR | S_IWUSR);
    vars = (int *)shmat(var_id, 0, 0);
    vars[FRONT] = vars[BACK] = 0;

    /* initialize the buffer */
    buf_id = shmget(IPC_PRIVATE, BUF_SIZE * sizeof(int), S_IRUSR | S_IWUSR);

    /* initialize the semaphores */
    sem_id = semget(IPC_PRIVATE, 3, S_IRUSR | S_IWUSR);
    semop(sem_id, init_sm, 3);

    /* Spawn all concurrent processes.
     * fork returns the child's pid to the parent and 0 to the child.
     * the following loop starts five new child processes and assigns
     * them the roles of producer or consumer.
     */
    for (i = 0; i < 5; i++) {
        int	pid = fork();

        if (pid == 0 && i < 3)
            producer(i);
        else if (pid == 0)
            consumer(i);
        /*else pid == 0, which implies the parent process --
        it loops around to start the next child.*/
    }

    /* 5 processes were started - wait for five processes to finish */
    for (i = 0; i < 5; i++) {
        wait(NULL);
        debug();
    }

    return 0;
}


/*
 * Producer processes "produce" a pseudo-random number and simulates the
 * time needed to produce something by sleeping for that number of seconds.
 * After awakening, they attempt to acquire a semaphore lock, waiting if it's
 * not available.  After placing the pseudo-random number in the shared memory,
 * they releases the lock.
 */
void producer(int id) {
    int	i;
    char	obuf[512];
    int*	vars;
    int*	buffer;
    struct	sembuf	acquire[] = {{ 0, -1, 0 }, { 2, -1, SEM_UNDO }};
    struct	sembuf	release[] = {{ 1, 1, 0 }, { 2, 1, SEM_UNDO }};

    vars = (int *)shmat(var_id, 0, 0);	/* attach to buffer vars */
    buffer = (int *)shmat(buf_id, 0, 0);	/* attach to the buffer */

    fprintf(stderr, "Producer starting\n");
    for (i = 0; i < 10; i++) {
        int	prod = rand() % 8;
        sleep(prod);

        /* critical section -- see section 6.2 */
        semop(sem_id, acquire, 2);	/* buffer not full	     */

        buffer[vars[FRONT]] = prod;
        vars[FRONT] = (vars[FRONT] + 1) % BUF_SIZE;

        semop(sem_id, release, 2);	/* buffer has 1 more element */
        /* end critical section */

        sprintf(obuf, "producer %d %d\n", id, prod);
        write(1, obuf, strlen(obuf));
    }
    fprintf(stderr, "producer %d exiting\n", id);
    exit(0);
}


/*
 * Consumer processes simulates the time needed to consume something by
 * sleeping for a pseudo-random amount of time.
 * After awakening, they attempt to acquire a semaphore lock, waiting if it's
 * not available.  After removing a pseudo-random number from the buffer in the
 * shared memory, they releases the lock.
 */
void consumer(int id) {
    int	i;
    char	obuf[512];
    int*	vars;
    int*	buffer;
    struct	sembuf	acquire[] = {{ 1, -1, 0 }, { 2, -1, SEM_UNDO }};
    struct	sembuf	release[] = {{ 0, 1, 0 }, { 2, 1, SEM_UNDO }};

    vars = (int *)shmat(var_id, 0, 0);	/* attach to buffer vars */
    buffer = (int *)shmat(buf_id, 0, 0);	/* attach to the buffer */

    fprintf(stderr, "Consumer starting\n");
    for (i = 0; i < 15; i++) {
        int	consume;
        sleep(rand() % 4);

        /* critical section -- see section 6.2 */
        debug();
        semop(sem_id, acquire, 2);	/* buffer not empty 	   */

        consume = buffer[vars[BACK]];
        vars[BACK] = (vars[BACK] + 1) % BUF_SIZE;

        semop(sem_id, release, 2);	/* buffer has 1 more space */
        /* end critical section */

        sprintf(obuf, "consumer %d %d\n", id, consume);
        write(1, obuf, strlen(obuf));
    }
    fprintf(stderr, "consumer %d exiting\n", id);
    exit(0);
}

/*
 * prints the semaphore values of the three programs semaphores.
 */
void debug()
{
    unsigned short	semvals[3];
    union semun	arg;
    arg.array = semvals;
    semctl(sem_id, 0, GETALL, arg);
    printf("\t%d\t%d\t%d\n", semvals[0], semvals[1], semvals[2]);
}
```

