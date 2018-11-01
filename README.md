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

- 
