# Chapter6. 프로세스 생성과 실행 

- Process의 생성
  fork 시스템 호출

  ```c
  #include <sys/types.h>
  #include <unistd.h>

  pid_t fork(void);
  ```

  - 실행 중이던 process(parent)의 복사본 process(child) 생성

  - fork() 바로 다음 문장부터 동시에 실행
    아래 그림 실행하면, one two two가 출력이 된다.
    parent와 child가 하는 일을 따로 지정을 안 해줘서 같은 일을 반복적으로 수행한 것임.

    ![](/Users/jaehyukshin/Desktop/스크린샷 2018-10-01 오후 6.53.49.png)

  - Parent / child 구분

    - fork()를 하면 복사를 하는 것이기 때문에 data, stack, pcb(program counter : 몇번째줄까지 실행됐다 정보가 들어있음)를 다 복사한다
      그래서 이걸로는 누가 parent인지 child인지 구분을 할 수 없음
    - pid와 ppid가 다르다 / 나머지는 다 똑같이 복사되지만 이것만 아님
    - fork()의 return 값이 달라서, return 값으로 확인
      (pid = fork()) > 0 : pid는 parent, child 프로세스의 프로세스id가 반환된다
      (pid = fork()) == 0 : pid는 child
      (pid = fork()) < 0 : fork() 실패
    - fork() 실패하는 경우
      - 시스템 전체 process 수 제한
      - 한 process가 생성할 수 있는 process 수 제한
    - 그래서 아래와 같이 parent / child의 역할을 구분해서 지정해줘야함

    ```c
    int main(int argc, char **argv){
            pid_t pid;
            printf("Calling fork\n");
            pid = fork();

            if(pid == 0){
                    printf("Child\n");
            }
            else if(pid > 0){
                    printf("Parent\n");
            }
            else{
                    printf("Fork returned error code\n");
            }

            return 0;       
    }
    ```

- fork시 주의할 점

  ```c
  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <string.h>
  #include <ftw.h>
  #include <stdlib.h>

  int main(int argc, char **argv){
          int i;
          pid_t pid;

          printf("%ld : %ld\n", getpid(), getppid());

          for(i=0;i<3;i++){
                  pid = fork();
                  if(pid == 0){
                          printf("%ld : %ld\n", getpid(), getppid());
                          exit(0); // 제일 중요한 부분
                  }
          }
          for(i=0;i<3;i++){
                  if(pid > 0){
                          wait(0);
                  }
          }
          return 0;
  }
  ```

  child 프로세스가 기능을 다 마무리했으면 exit(0)으로 종료를 시켜줘야함
  fork()는 parent와 같은 시점의 내용이 복사가 되니까, for문 사용할 때 주의해야함
  exit(0)이 없으면, 7번 실행이 된다

- Fork : 파일과 자료

  - child 프로세스는 parent 프로세스의 복제
    모든 변수 값이 그대로 복제 된다
  - fork() 후에 변경된 값은 복제되지 않는다
  - file descriptor도 복제된다
    포인터이기 때문에 같은 file table의 내용에 접근하고 있게 된다
    Parent process가 open한 file을 child process에게도 open한다
    즉, parent와 child가 file을 공통으로 사용가능함
  - file pointer를 공유하고 있다는 예제

  ```c
  int main(int argc, char **argv){

          int fd;
          pid_t pid;
          char buf[10];

          fd = open("data", O_RDONLY);
          read(fd, buf, 10);
          printf("before fork : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));

          switch(pid = fork()){
                  case -1:
                          perror("fork failed\n");
                          exit(1);
                          break;
                  case 0:
                          printf("child before read : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
                          read(fd, buf, 10);
                          printf("child after read : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
                          break;
                  default:
                          wait((int *) 0);
                          printf("parent after wait : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
          }

          return 0;
  }
  ```

  child 생성 전에 파일 읽고, child 생성 후 파일 읽으면 파일을 공유하고 있다는 것을 알 수 있음

- Exit 시스템 호출

  ```c
  #include <stdlib.h>

  void exit(int status);
  ```

  - Exit : process 정지 => open 된 파일 닫기 => clean-up-action

  - status의 값은 프로세스 종료 후, `$ echo $?` 명령을 통해 알아낼 수 있음
    이걸로 방금 종료한 프로세스 exit 값을 출력할 수 있다
    exit 값은 OS가 받아서 parent process에 전달한다. child 프로세스가 어떤 상황에서 종료됐는지 확인하기위해

  - Clean-up-action 지정

    - 지정된 역순으로 실행

      ```c
      #include <stdlib.h>

      int atexit(void (*func)(void))
      ```

- asdf

- ​