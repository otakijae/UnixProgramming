# Chapter5. 프로세스 정보

- Process란,

  - 프로그램이 파일 형태로 하드디스크에 저장돼있다가, 실행을 시키면 process가 된다
  - [Program code]
  - [Data : 프로그램 전역 변수] : 처음 컴파일할 때부터 메모리 Data 영역에 따로 저장됨. 프로그램이 끝나야 사라짐
  - [Stack : 프로그램 지역 변수] : 함수에서 호출된 변수들 메모리 Stack 영역에 따로 저장됨. 함수 return 시 사라짐
  - [PCB : Process Control Block]
    - 프로세스 제어 블록
    - Process는 여러가지 요소들에 의해 유일하게 식별될 수 있다
      (식별자, 샅애, 우선순위, 프로그램 카운터, 메모리 포인터, 문맥 데이터, 입출력상태정보, 어카운팅정보)
      이런게 있는데, 이런 요소들이 PCB에 저장이 되고, OS에 의해 생성되고 관리된다
    - PCB이 주요 역할은 process를 interrupt한 후 다시 그 process를 수행재개 할 수 있게 process의 정보를 유지하는 것. 다수의 process를 지원하고, 멀티프로세싱을 제공할 수 있게 해준다
    - [Process Identification]
      - Process Identifier : 프로세스들이 서로 통신할 때, 통신할 상대방을 OS에게 알려주기 위해 프로세스 식별자 사용
      - Parent Process Identifier : 프로세스들이 다른 프로세스를 생성할 때, 식별자를 이용해서 부모/자식 프로세스 구별 가능
      - User Identifier : 작업에 대해 책임지고 있는 사용자를 지시하기 위해 사용자 식별자가 각 프로세스에 할당될 수 있다
    - [Processor State Information]
    - [Process Control Information]

- Process의 계층 구조 : Parent process -> Child proceses
  프로그램을 실행하면, 프로세스는 여러개의 child 프로세스를 만들어서, 여러개의 프로세스들로 실행이 된다

- UNIX 시스템의 모든 process는 [init] 이라는 1번 process의 descendent process이다
  [init] 프로세스가 [shell] process를 만들고, shell이 [child]를 만들어서 프로그램을 실행한다

- 예를 들어서 a.out 프로그램을 실행하면, init 프로세스가 shell 프로세스를 만들고, shell 프로세스가 a.out이라는 child 프로세스를 만들어서 실행한다

- Shell상에서 프로세스 목록 확인

  ```c
  $ ps
  ```

  실행 결과

  ```
    PID TTY          TIME CMD
  16945 pts/7    00:00:00 bash
  20763 pts/7    00:00:00 ps
  ```

- 실행 중인 프로세스 종료 시키기

  PID를 알아야 종료시킬 수 있기 때문에 ps로 확인하고 프로세스를 종료시킨다

  ```
  $ kill -9 [프로세스 번호 : PID]
  ```

- Process Identifier
  
  - 음이 아닌 정수
  - 0 : swapper
- 1 : init
  
  - getpid() : 지금 실행 중인 process ID
- getppid() : 지금 실행 중인 process의 parent ID
  
  ```c
  include<unistd.h>
  
  pid_t getpid(void);
  pid_t getppid(void);
  
  getpid();
  getppid();
  ```

- Process Group
  
  - 특정 프로세스들을 묶어서 하나의 group으로 만들어서, 같은 group에 속한 process들에게 동시에 signal을 보낼 수 있다
    초기에 fork나 exec에 의해 group id 계승(상속) : 딱히 명시 안 해주면, parent 아래 모든 process는 같은 group임
  그래서 group id를 바꿔주면 그 아래 gid도 다 상속을 받으니 바뀌게 된다
  
- Group leader : group leader는 pid와 pgid를 같게 해줘야함. 따라서, 자신의 pid가 group id이면 group leader이다.

  ```c
  #include <sys/types.h>
  #include <unistd.h>
  
  //나의 group id 알아보기
  pid_t getpgrp(void);
  
  //다른 child의 id를 인자로 넘겨주면, 그 process의 gid 반환
  //인자가 0이면, 호출 프로세스 자신의 group id를 반환
  pid_t getpgid(pid_t pid);
  getpgrp();
  getpgid(0);
  ```

  - process group 변경 / setpgid(id, gid);

    지금부터 새로운 group leader가 되겠다는 명령

    ```c
    #include <sys/types.h>
    #include <unistd.h>
    
    //인자(내 id, 내 group id)
    int setpgid(pid_t pid, pid_t pgid);
    ```

    ```c
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <string.h>
    #include <ftw.h>
    
    int main(int argc, char **argv) {
      pid_t pid;
    
      printf("%ld : %ld\n", getpid(), getppid());
      printf("%ld : %ld\n", getpgrp(), getpgid(0));
    
      pid = fork();
    
      //child인 경우에 0 반환
      if(pid == 0) {
        printf("%ld : %ld\n", getpid(), getppid());
        printf("%ld : %ld\n", getpgrp(), getpgid(0));
    
        //group id 변경
        setpgid(getpid(), getpid());
    
        printf("%ld : %ld\n", getpid(), getppid());
        printf("%ld : %ld\n", getpgrp(), getpgid(0));
      }
      wait(0);
      return 0;
    }
    ```

  - 실행결과

    - ./example.out 실행하면, shell 프로세스가 example 프로세스를 실행. example을 group의 리더로 실행시킴
    
    ```
    29816 : 16945 //example 프로세스 id : shell 프로세스 id
    29816 : 29816 //example 프로세스 gid
      
    29817 : 29816 //child 프로세스 id : parent인 example 프로세스 id
    29816 : 29816 //child 프로세스 gid. example 프로세스의 child이니까 parent의 gid와 같음
      
    29817 : 29816 //child 프로세스 id : parent인 example 프로세스 id
    29817 : 29817 //child 프로세스를 group leader로 만들어서 pid와 pgid 같아짐
    ```
    
- Session

  - 한 session은 한 단말기를 사용하는 foreground process group과 background process group의 집합체이다

  - 각 process group은 하나의 session에 속한다
    A [*process group*] is a collection of related processes which can all be signalled at once.

    A *session* is a collection of process groups, which are either attached to a single terminal device (known as the *controlling terminal*) or not attached to any terminal.

    Sessions are used for job control: one of the process groups in the session is the foreground process group, and can be sent signals by terminal control characters. You can think of a session with a controlling terminal as corresponding to a "login" on that terminal. (Daemons normally disassociate themselves from any controlling terminal by creating a new session without one.)

    e.g. if you run `some_app` from the shell, the shell creates a new process group for it, and makes that the foreground process group of the session. (`some_app` might create some child processes; by default they will be part of the same process group.) If you then press `^Z`, `some_app`'s process group is signalled to stop it; and the shell's process group is switched to be the foreground process group again. Then e.g.`bg %1` would start `some_app`'s process group again, but keep it running in the background.

  - getsid 시스템 호출 : session id를 획득 / 어떤 session에 속해있는지 확인

    ```c
    #include <sys/types.h>
    #include <unistd.h>

    pid_t getsid(pid_t pid);
    ```

  - setsid() 시스템 호출

    ```c
    #include <sys/types.h>
    #include <unistd.h>

    pid_t setsid(void);
    ```

    - 제어 단말기를 갖지 않는 새로운 session과 group 생성
    
      호출 프로세스의 id가 session과 group의 id가 된다

      만약, 호출 process가 현재 group의 leader이면 -1을 return한다
    
    - session을 종료해도 계속 실행 되게 만들 때 session을 변경한다
      
      이 session과 관계없이 실행하겠다라는 명령이니, shell 프로그램 종료돼도 종료를 못시키게 돼서 사용하지 말 것

- Main함수 인자 사용. 이제는 이렇게 인자를 전달받아서 프로그램을 만들 것

  ```c
  //argc 문자열 몇 개를 넘겨받았는지 알려줌
  //argv 포인터 배열로, 어떤 값을 넘겨받았는지 알 수 있음
  int main(int argc, char **argv) {
    int i = 0;
    printf("%d\n", argc);
  
    while(argv[i] != NULL) {
      printf("%s\n", argv[i]);
      i++;
    }
    return 0;
  }
  ```

- 모든 입출력은 OS가 담당함. program이 I/O 작업은 OS에게 부탁을 하는 것임. 그러면 입력받은 값들을 OS는 main함수 인자로 넘겨주는 것임

  ```
  //ls 현재위치 프로그램 이름
  //-l 프로그램 실행을 위한 인자
  $ ls -l
  
  //chmod 현재위치 프로그램 이름
  //0777 프로그램 실행을 위한 인자
  //a.c 프로그램 실행을 위한 인자
  $ chmod 0777 a.c
  ```

- getpid() / getppid() / getpgrp() / getpgid(0) / setpgid(pid, gid) / getsid(pid) / setsid()