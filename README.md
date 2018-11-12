# UnixProgramming

## 20181029

- p8-1.c
  Parent process는 세 개의 child process를 만들고, 1초 간격으로 각 child process에게 SIGUSR1 signal을 보낸 후, child process들이 종료 할 때까지 대기 하였다가, 종료한 child process의 id와 exit status를 출력 후 종료 합니다. 각 child process는 생성 후 parent로부터 signal이 올 때까지 대기 하였다가 (pause() 사용), signal을 받으면 자신의 process id를 세 번 출력 한 후 종료 합니다. 이와 같이 동작하는 프로그램을 아래 코드를 사용하여 작성 하시오.

  ```c
  #define BUFSIZE 512
  
  void catchusr(int);
  
  void do_child(int i){
          int pid;
          static struct sigaction act;
  
          act.sa_handler = catchusr;
          sigaction(SIGUSR1, &act, NULL);
  
          printf("%d-th child is created...\n", i);
  
          pause();
  
          pid = getpid();
  
          for(i=0;i<3;i++){
                  printf("[child process id] : %d ...\n", getpid());
          }
  
          exit(0);
  }
  
  void catchusr(int signo){
          printf(".....catch SIGUSR1 = %d\n", signo);
  }
  
  int main(int argc, char **argv){
  
          int i, k, status;
          pid_t pid[3];
  
          for(i=0;i<3;i++){
                  pid[i] =fork();
                  if(pid[i] == 0){
                          do_child(i);
                  }
          }
  
          for(i=0;i<3;i++){
                  sleep(1);
                  kill(pid[i], SIGUSR1);
          }
  
          for(i=0;i<3;i++){
                  k = wait(&status);
                  printf("child #%d is terminated...%d\n", k, WEXITSTATUS(status));
          }
  
          exit(0);
  }
  ```

- P8-2.c
  Parent process는 다섯 개의 child process들을 만들고, 대기하였다가 child process가 모두 종료 한 후 종료 합니다. 각 child process는 자신의 순서가 될 때까지 대기 하였다가, 1초씩 쉬면서 (sleep (1); 사용) 자신의 process id(getpid(); 사용)를 5회 출력하는 작업을 한 후 종료 합니다. child process의 id 츨력 순서는 생성 순서의 역순이며, 이와 같은 순서 동기화 작업은 signal을 보내서 진행 합니다.

  - 생성 순서의 역순으로 작업 수행해야하니까, 첫 번째로 만들어진 child 이외의 child들은 pause로 기다림

  ```c
  void catchusr(int);
  
  void do_child(int i, int *pid_array){
          int j, pid;
          static struct sigaction act;
  
          act.sa_handler = catchusr;
          sigaction(SIGUSR1, &act, NULL);
  
          printf("%d-th child is created ... pid = %d\n", i, getpid());
  
          //생성 순서의 역순으로 작업 수행해야하니까, 첫 번째로 만들어진 child 이외의 child들은 pause로 기다림
          if(i<4)
                  pause();
  
          pid = getpid();
  
          for(j=0;j<5;j++){
                  printf("[child process id] : %d ...\n", getpid());
                  sleep(1);
          }
  
          if(i>0){
                  kill(pid_array[i-1], SIGUSR1);
          }
  
          exit(0);
  }
  
  void catchusr(int signo){
          printf(".....catch SIGUSR1 = %d\n", signo);
  }
  
  int main(int argc, char **argv){
  
          int i, status;
          pid_t pid[5];
  
          for(i=0;i<5;i++){
                  pid[i] =fork();
                  if(pid[i] == 0){
                          do_child(i, pid);
                  }
          }
  
          for(i=0;i<5;i++){
                  wait(&status);
          }
  
          exit(0);
  }
  ```

- P8-3.c
  10개의 정수를 입력으로 받아 그 합을 출력하는 프로그램을 작성 합니다. 단, 10초간 사용자로부터 의 입력이 없으면, 경고 메시지를 출력하고 다시 입력을 기다립니다.

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
  
  void catchalarm(int);
  
  int main(int argc, char **argv){
  
          int i, num, sum = 0;
          static struct sigaction act;
  
          act.sa_handler = catchalarm;
          sigaction(SIGALRM, &act, NULL);
  
          for(i=0;i<10;i++){
                  do{
                          alarm(10);
                  }while(scanf("%d", &num) < 0);
                  alarm(0);
                  sum += num;
                  printf("sum = %d\n", sum);
          }
          exit(0);
  }
  
  void catchalarm(int signo){
          printf("[Warning]No input for 10 seconds ... %d\n", signo);
  }
  ```

## 20181031

- 원격 서버의 디렉토리 로컬에 저장. scp 명령

  - -r 옵션 주면 디렉토리 복사 가능
  - -r 옵션 안 주면 파일만 복사 가능

  ```
  $ scp -P 1074 -r s13011022@203.250.148.93:/home/account/class/tspark/s13011022 ./unix
  
  [참고]
  http://faq.hostway.co.kr/?mid=Linux_ETC&page=8&document_srl=1426
  https://skylit.tistory.com/90
  ```


## 20181105

- p9-1
  메모리 매핑을 이용한 두 개의 프로그램을 작성 합니다.

  - (a) reader 프로그램은 “temp" 파일을 메모리 매핑 한 후, scanf()로 10개의 정수를 읽어서 매핑된 파일에 저장하는 작업을 10회 실행 합니다.

    ```c
    #define BUFSIZE 512
    
    int main(int argc, char **argv){
            int i, fd;
            int *addr;
    
            fd = open("temp", O_RDWR | O_CREAT, 0600);
            addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        
            ftruncate(fd, sizeof(int)*10);
    
            for(i=0;i<10;i++){
                    scanf("%d", addr+i);
            }
            exit(0);
    }
    ```

  - (b) writer 프로그램은 “temp" 파일을 메모리 매핑 한 후, 매핑된 파일에 있는 정수를 출력하는 작업을 10회 실행합니다 (printf() 사용). 단, 출력 프로그램은 5초간 sleep() 한 후 5회의 출력 작업을 연속 진 행 하고 다시 5초간 sleep() 한 후 5회의 출력 작업을 진행 합니다.

    ```c
    #define BUFSIZE 512
    
    int main(int argc, char **argv){
            int i, j, fd;
            int *addr;
    
            fd = open("temp", O_RDWR | O_CREAT, 0600);
            addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
        	//굳이 sleep해주는건 동기화 때문에
            sleep(5);
    
            for(i=0;i<10;i++){
                    sleep(5);
                    for(j=0;j<5;j++){
                            printf("%d ", *(addr+i));
                    }
                    printf("\n");
            }
            exit(0);
    }
    ```

  - (c) 두 프로그램이 모두 종료 한 후 ”temp" 파일의 크기를 확인합니다.

    ```c
    40으로 동일
    ```

- P9-2
  메모리 매핑을 이용한 두 개의 프로그램을 작성 합니다.

  - (a) reader 프로그램은 “temp" 파일을 메모리 매핑 한 후, 외부 입력을 읽어서 (read() 시스템 콜 사용) 매핑된 메모리에 저장하는 작업을 3회 실행 합니다.

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
    #include <sys/mman.h>
    
    #define BUFSIZE 512
    
    int main(int argc, char **argv){
    
            int i, fd, len = 0;
            char *addr;
    
            fd = open("temp", O_RDWR | O_CREAT, 0600);
    
            addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
            ftruncate(fd, BUFSIZE);
            for(i=0;i<3;i++){
                    len = len + read(0, addr + len, BUFSIZE);
                    printf("[read] : %d\n", len);
                    if(len > BUFSIZE)
                            break;
            }
    
            exit(0);
    }
    ```

  - (b) writer 프로그램은 “temp" 파일을 메모리 매핑 한 후, 매핑된 메모리의 내용을 출력하는 작업을 3회 실행합니다 (write() 시스템 콜 사용). 단. 3초간 sleep() 하면서 출력 작업을 진행합니다.

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
    #include <sys/mman.h>
    
    #define BUFSIZE 512
    
    int main(int argc, char **argv){
    
            int i, fd, len = 0;
            char *addr;
    
            fd = open("temp", O_RDWR | O_CREAT, 0600);
    
            addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            printf("[test]%s\n", addr);
    
            for(i=0;i<3;i++){
                    sleep(3);
                    len = len + write(1, addr + len, BUFSIZE);
                    write(1, "-------\n", 8);
                    if(len > BUFSIZE)
                            break;
            }
    
            exit(0);
    }
    ```

- p9-3.c
  Parent process는 세 개의 child process들을 만들고, 모든 child process가 종료 한 후 종료합니다. 각 child process는 자신의 순서가 될 때까지 대기 하였다가, 1초씩 쉬면서 자신의 process id를 5 회 출력 한 후 종료합니다. child process의 id 츨력 순서는 생성 순서의 역순이며, 이와 같은 순서 동 기화 작업은 매핑된 파일을 이용하여 진행 합니다.

  - 일일이 메모리 맵핑하지 말고, 메모리 매핑한 배열을 child 함수 인자로 넘겨줘서 실행하면 더 간단하게 코드 작성 가능

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
  #include <sys/mman.h>
  
  #define BUFSIZE 512
  
  void do_child(int i, int *addr){
          int fd, child, j;
  
          child = *(addr + 0);
  
          while(i != child){
                  sleep(2);
                  printf(".........................pid : %d waiting\n", i);
  
                  child = *(addr + 0);
          }
  
          for(j=0;j<5;j++){
                  sleep(1);
                  printf("%dth child pid = %d\n", i, getpid());
          }
          *(addr + 0) = child - 1;
  
          exit(0);
  }
  
  int main(int argc, char **argv){
  
          int i, status, fd;
          pid_t pid[3];
          int *addr;
  
          fd = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0600);
          addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
          ftruncate(fd, sizeof(int));
          *(addr + 0) = 2;
  
          for(i=0;i<3;i++){
                  pid[i] =fork();
                  if(pid[i] == 0){
                          do_child(i, addr);
                  }
          }
  
          for(i=0;i<3;i++){
                  wait(&status);
          }
  
          exit(0);
  }
  ```

  - 교수님 답안

    - 일일이 메모리 맵핑하지 말고, 메모리 매핑한 배열을 child 함수 인자로 넘겨줘서 실행하면 더 간단하게 코드 작성 가능

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
    #include <sys/mman.h>
    
    #define BUFSIZE 512
    
    void do_child(int i, int *turn){
            int j, pid;
    
            if(i<2){
                    do{
                    }while(i!=*turn);
            }
    
            pid = getpid();
    
            for(j=0;j<5;j++){
                    printf("child %d ... \n", pid);
                    sleep(1);
            }
    
            if(i>0)
                    *turn = i-1;
    
            exit(0);
    }
    
    int main(int argc, char **argv){
            int i, status, fd;
            pid_t pid[3];
            int *addr;
    
            fd = open("temp", O_RDWR | O_CREAT | O_TRUNC, 0600);
            addr = mmap(NULL, BUFSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            ftruncate(fd, sizeof(int));
            *addr = 2;
    
            for(i=0;i<3;i++){
                    pid[i] =fork();
                    if(pid[i] == 0){
                            do_child(i, addr);
                    }
            }
    
            for(i=0;i<3;i++){
                    wait(&status);
            }
    
            exit(0);
    }
    ```

## 20181107

- p10-1.c
  parent process는 세 개의 child process를 만들고, parent process에서 child process로의 단방향 통신이 가능 한 pipe를 만듭니다. parent process는 외부 입력으로 정수를 12개 입력 받아, 세 child process에게 순서대로 보냅니다. child process는 자신이 받은 정수를 자신의 프로세스 id와 함께 출력 합니다. parent process는 모든 정수를 전달 한 후 정수 -1을 전달하고, -1을 전달 받으면 child process는 종료 합니다. 모든 child process의 종료를 확인 한 후 parent process는 종료 합니다.

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
  #include <sys/mman.h>
  
  #define BUFSIZE 512
  
  void do_child(int id, int p[3][2]){
      int i, in, pid = getpid();
  
      for(i=0;i<3;i++){
          close(p[i][1]);
          if(id != i)
              close(p[i][0]);
      }
  
      while(1){
          read(p[id][0], &in, sizeof(int));
          if(in == -1)
              exit(0);
          else
              printf("id : %d, pid : %d, input : %d\n", id, pid, in);
      }
  }
  
  int main(int argc, char **argv){
      int i, in, pid, p[3][2];
  
      for(i=0;i<3;i++){
          pipe(p[i]);
      }
  
      for(i=0;i<3;i++){
          if(fork() == 0){
              do_child(i, p);
          }
      }
  
      for(i=0;i<3;i++){
          close(p[i][0]);
      }
  
      for(i=0;i<12;i++){
          scanf("%d", &in);
          write(p[i%3][1], &in, sizeof(int));
      }
  
      in = -1;
  
      for(i=0;i<3;i++){
          write(p[i][1], &in, sizeof(int));
      }
  
      for(i=0;i<3;i++){
          wait(0);
      }
  
      exit(0);
  }
  ```

- p10-2.c
  parent process는 세 개의 child process를 만들고, 모든 child process가 종료 한 후 종료 합니다. 각 child process는 자신의 순서가 될 때까지 대기 하였다가, 1초씩 쉬면서 (sleep (1); 사용) 자신의 process id(getpid(); 사용)를 5회 출력하는 작업을 한 후 종료 합니다. child process의 id 츨력 순서는 생성 순서의 역순이며, 이와 같은 순서 동기화 작업은 pipe를 이용하여 진행 합니다.

  ```c
  
  ```


## 20181112

- p11-1
  fifo를 이용하여 통신하는 두 개의 프로그램을 작성합니다. 프로그램 A는 외부 입력으로 정수를 입력 받아 프로그램 B에 전달합니다. 프로그램 B는 전달 받은 정수에 +8을 한 뒤 프로그램 A에 돌려줍니다. 프로그램 A는 돌려받은 정수 값을 출력 합니다. -1이 입력되면 두 프로그램은 종료합니다.

  p11-1A.c

  ```c
  
  ```

  p11-1B.c

  ```c
  
  ```

- p11-2
  server process는 세 개의 client process들과 데이터를 주고받기 위한 fifo를 만듭니다. 각 client는 미리 정해진 이름의 FIFO로 접속하여, 표준 입력으로 입력된 정수를 server process에게 전송합니다. server process는 client process로부터 전송된 정수 값에 +8을 한 후, 해당 client에게 다시 보냅니다. client process는 돌려받은 정수 값을 표준 출력으로 출력합니다. client process는 정수 데이터의 입/출력 작업을 5회 반복 한 후 종료 합니다. client process로 부터의 입력을 blocking으로 기다리기 위해 select 문장을 사용합니다.

  ```c
  
  ```

- p11-3
  네 개의 프로세스가 동기화를 하며 자신의 프로세스 id를 5회 출력하는 프로그램을 작성합니다. 이 프로그램은 main() 함수의 arguments로 동기화에 참여하는 전체 프로세스 중 자신의 출력 순서를 입력받습니다. 프로그램이 시작되면, 순서대로 자신의 프로세스 id를 출력합니다. 동기화 작업은 fifo를 사 용하여 수행합니다.

  ```c
  
  ```
