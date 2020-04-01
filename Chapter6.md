# Chapter6. 프로세스 생성과 실행 

- Process의 생성. fork 시스템 호출
  
```c
  #include <sys/types.h>
  #include <unistd.h>
  
pid_t fork(void);
  ```
  
- 실행 중이던 process(parent)의 복사본 process(child) 생성
  
- fork() 바로 다음 문장부터 동시에 실행
    아래 그림 실행하면, one two two가 출력이 된다.
    parent와 child가 하는 일을 따로 지정을 안 해줘서 같은 일을 반복적으로 수행한 것임.
  
  ![child생성 후 작업](https://t1.daumcdn.net/cfile/tistory/998909345BB310F30F)
  
- Parent / child 구분
  
  - fork()를 하면 복사를 하는 것이기 때문에 data, stack, pcb(program counter : 몇번째줄까지 실행됐다 정보가 들어있음)를 다 복사한다
      그래서 이걸로는 누가 parent인지 child인지 구분을 할 수 없음
    - pid와 ppid가 다르다 / 나머지는 다 똑같이 복사되지만 이것만 아님
    - fork()의 return 값이 달라서, return 값으로 확인
      - (pid = fork()) > 0 : pid는 parent, child 프로세스의 프로세스id가 반환된다
      - (pid = fork()) == 0 : pid는 child
      - (pid = fork()) < 0 : fork() 실패
    - fork() 실패하는 경우
      - 시스템 전체 process 수 제한
      - 한 process가 생성할 수 있는 process 수 제한
    - 그래서 아래와 같이 parent / child의 역할을 구분해서 지정해줘야함
  
  ```c
    int main(int argc, char **argv) {
            pid_t pid;
            printf("Calling fork\n");
            pid = fork();
  
          if(pid == 0) {
                    printf("Child\n");
            } else if(pid > 0) {
                    printf("Parent\n");
            } else {
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

  int main(int argc, char **argv) {
          int i;
          pid_t pid;

          printf("%ld : %ld\n", getpid(), getppid());

          for(i=0;i<3;i++) {
                  pid = fork();
                  if(pid == 0) {
                          printf("%ld : %ld\n", getpid(), getppid());
                          exit(0); // 제일 중요한 부분
                  }
          }
          for(i=0;i<3;i++) {
                  if(pid > 0) {
                          wait(0);
                  }
          }
          return 0;
  }
  ```

  - child 프로세스가 기능을 다 마무리했으면 exit(0)으로 종료를 시켜줘야함
  - fork()는 parent와 같은 시점의 내용이 복사가 되니까, for문 사용할 때 주의해야함
  - exit(0)이 없으면, 7번 실행이 된다

- Fork : 파일과 자료

  - child 프로세스는 parent 프로세스의 복제. 모든 변수 값이 그대로 복제 된다
  - fork() 후에 변경된 값은 복제되지 않는다
  - file descriptor도 복제된다. 포인터이기 때문에 같은 file table의 내용에 접근하고 있게 된다. Parent process가 open한 file을 child process에게도 open한다. 즉, parent와 child가 file을 공통으로 사용가능함
  - file pointer를 공유하고 있다는 예제
  
  ```c
  int main(int argc, char **argv) {
          int fd;
        pid_t pid;
          char buf[10];
  
        fd = open("data", O_RDONLY);
          read(fd, buf, 10);
          printf("before fork : %ld\n", lseek(fd, (off_t)0, SEEK_CUR));
  
        switch(pid = fork()) {
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
  
- child 생성 전에 파일 읽고, child 생성 후 파일 읽으면 파일을 공유하고 있다는 것을 알 수 있음
  
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

- exec 
  
- 새 프로그램 수행
  
  ```c
#include <unistd.h>
  
  int execl(const char *path, const char *arg0, ..., const char *argn, (char *) 0);
  int execlp(const char *file, const char *arg0, ..., const char *argn, (char *) 0);
  int execv(const char *path, char *const argv[]);
  int execvp(const char *file, char *const argv[]);
```
  
- 함수 인자
  
    - path : 실행파일 전체 경로 / arg0 : 실행파일 이름 / arg1~argn : 실행하고자 하는 프로그램의 인수들
    - avgv : {path, arg0, arg1~argn} 실행파일 인자들 배열로 넘겨줌
    - `(char *)0` 은 NULL문자임
    - execl, execlp는 함수 인자로 실행파일 인자들 나열 / execv, execvp는 함수 인자로 실행파일 인자 배열로 전달
    - execl, execvp는 p가 들어가는데 path라는 의미이고, 환경변수에 의해 설정된 path안의 file을 훑어서 실행하겠다는 의미. 그래서 실행파일 경로 적는 부분에 파일 이름만 적어주면 된다.
    - 참고) 터미널에서 `$ echo $PATH` 하면 환경변수로 설정된 경로가 뭐가 있는지 출력됨. 그래서 `$ ls` 이런 명령 또한 환경변수 경로에 위치해있는 실행파일이기 때문에 파일 이름만 적으면 실행이 된다
  
  - 호출 프로세스의 주기억장치 공간에 새로운 프로그램을 적재

    - 호출 프로그램은 no longer exist
    - 새 프로그램은 처음부터 실행
    - 새 프로그램은 호출 프로세스의 id로 실행
    - 호출하는 프로세스를 새로운 프로세스로 변경시키는데 사용한다
    - exec후, 프로세스의 개수는 그대로 이면 그 프로세스의 PID 번호도 변경되지 않지만, 프로세스의 내용이 변경된다

  - return 값

    - exec()는 성공적으로 마쳐진 경우에는 제어가 반환되지 않음
      새 프로그램으로 바뀌어서 return 받을 사람이 없기 때문
    - 오류 발생시만 –1 return

  - 위 함수들은 fork로 차일드 프로세스를 만든 후, 그 프로세스를 새로운 독립적인 프로세스로 만들어 주는 역할을 한다
    따라서 그것은 우리가 명령프롬프트에 실행파일을 실행하는 것과 비슷함

    ```
    $ gcc -c test.c

    argc : 3
    arg0 : gcc
    arg1 : -c
    arg2 : test.c
    ```


- 기본 예제
  
- example.c
  
  ```c
int main(int argc, char **argv) {
          printf("executing a.out\n");
          execl("./a.out", "a.out", "3", (char *)0);
          printf("execl failed to run a.out");
      		exit(1);
    
      		/*
  				char *const av[] = {"a.out", "3", (char*)0};
          printf("executing a.out\n");
          execl("./a.out", av);
          printf("execv failed to run a.out");
         	exit(1);
         	*/
  }
```
  
- a.c
  
  ```c
int main(int argc, char **argv) {
          int i;
          for(i = 0;i <atoi(argv[1]);i++) {
                  printf("a.out실행\n");
          }
          return 0;
  }
  ```

- exec와 fork를 함께 사용

  ```c
  int main(int argc, char **argv) {
      pid_t pid;
  		switch (pid=fork()) {
          case –1:
            	perror(“fork failed”);
  	        	exit(1);
      	    	break;
          case 0:
              execl(“/bin/ls”, “ls”, “-l”, (char *) 0);
  	        	perror(“exec failed”);
              exit(1);
      	    	break;
          default:
              wait((int*)0);
      				printf(“ls completed\n”);
      				exit(0);
  		}
  }
  ```

- wait 시스템 호출

  ```c
  #include <sys/wait.h>
  #include <sys/types.h>
  
  pid_t wait(int *status);
  ```

  - 하나 이상의 child process 수행 시 아무나 하나가 종료되면 return 된다

  - return 값
    
    종료된 child의 id
    
    살아있는 child process가 없는 경우 -1
    
  - Status : child의 종료 상태가 전달

- wait(0); 를 하고 안 하고 차이
  
  - child를 생성한만큼 wait를 실행해줘야한다. 안 하게 되면, parent를 종료를 했기때문에 child가 종료하는 걸 안 기다림
  - wait를 하면 os로 인해 block이 된다
  - child 중 하나 실행종료되는대로 parent 다시 깨어난다
- child 종료되면 return 된 id를 출력하면 어떤 순서로 종료했는지 알 수 있음
  
  ```c
  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <string.h>
  #include <time.h>
  #include <ftw.h>
#include <stdlib.h>
  
  void do_child(void) {
        int i;
  
          for(i=0;i<5;i++) {
                  printf("child %ld running ... \n", getpid());
                  sleep(1);
          }
          exit(0);
}
  
int main(int argc, char **argv) {
          int i, status, N = 3;
          pid_t pid;

          for(i=0;i<N;i++) {
                  pid = fork();
                  if(pid == 0) {
                          do_child();
                  }
          }
    
  	    	/* wait를 적어줘야 
          for(i=0;i<N;i++){
          	wait(0);
          }
          */
          return 0;
  }
```
  
- 실행결과
  
  ```
  [s13011022@bce LAB10-08]$ ./example.out
  child 37172 running ... 
  child 37174 running ... 
  child 37173 running ... 
  [s13011022@bce LAB10-08]$ child 37172 running ... 
  child 37174 running ... 
  child 37173 running ... 
  child 37172 running ... 
  child 37174 running ... 
  child 37173 running ... 
  child 37172 running ... 
  child 37174 running ... 
  child 37173 running ... 
  child 37172 running ... 
  child 37174 running ... 
  child 37173 running ... 
```
  
- WIFEXITED(status) : status의 하위 8bit가 0인지 검사. 정상종료인지 검사. 1 return하면 정상종료이고, 정상 종료가 안 됐으면 exit 자체가 실행이 안 된 것이기 때문에 값을 알 수 없음

- WEXITSTATUS(status) : status의 상위 비트에 저장된 값을 return. Exit status 반환

- waitpid 시스템 호출
  
- 그냥 wait 시스템 호출을 사용하면, 종료한 순서대로 확인을 하기 때문에 원하는 순서대로 종료를 하고 싶으면 waitpid 시스템 호출 사용
  
  ```c
  #include <sys/types.h>
#include <sys/wait.h>
  
  pid_t waitpid(pid_t pid, int *status, int options);
```
  
  - pid : 기다리고 싶은 child의 id
  
  - status : child의 종료 상태
  
  - options  WNOHANG; (with no hang option)
  
    child가 종료하지 않았으면 0을 return
    
    child 종료했으면 종료한 child id return
    
    이 옵션으로 중간중간에 child 종료했는지 아닌지 확인해주면 된다
  
  ```c
  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
#include <unistd.h>
  #include <dirent.h>
#include <string.h>
  #include <time.h>
  #include <ftw.h>
#include <stdlib.h>
  
  int main(int argc, char **argv) {
          pid_t pid;
          int status, exit_status;

          if((pid=fork()) < 0) {
                  perror("fork failed");
                  exit(1);
          }

          if(pid == 0) {
                  sleep(4);
                  exit(5);
          }

          if((pid = wait(&status)) == -1) {
                  perror("wait failed");
                  exit(2);
          }
  
          if(WIFEXITED(status)) {
                  exit_status = WEXITSTATUS(status);
                printf("Exit status from %d was %d\n", pid, exit_status);
          }
  				exit(0);
  }
  ```
  
- 좀비 상태와 너무 이른 퇴장

  - 부모 프로세스가 wait를 수행하지 않고 있는 상태에서 자식이 퇴장할 때 -> child는 좀비상태가 된다
  - parent가 wait를 안 한 경우 : child 생성후 다른 작업을 하다가 wait를 한 경우가 있고, 아니면 아예 wait를 안 해준 경우가 있다
  - 하나 이상의 자식 프로세스가 수행되고 있는 상태에서 부모가 퇴장할 때 -> child는 init(pid = 1)의 child로 된다

