# pipe

- pipe

  - 한 프로세스에서 다른 프로세스로의 단방향 통신 채널
  - 파일이 아니지만, write와 read로 data 통신 가능

- pipe 만들기

  ```c
  #include <unistd.h>
  
  int pipe(int filedes[2]);
  ```

  - filedes[0] : 읽기용
  - filedes[1] : 쓰기용
  - 성공 시 0, 실패 시 -1 return
  - process 당 open file 수, 시스템 내의 file 수 제한

- pipe 특성

  - FIFO 처리
  - lseek은 작동하지 않음. (읽은 데이터는 사라지기 때문에)
  - pipe는 fork()에 의해 상속 가능. 파일포인터를 공유하는 것처럼

- pipe를 이용한 단방향 통신(부모 -> 자식)

  - pipe 생성
  - fork()에 의해 자식 생성 및 pipe 복사
  - 부모는 읽기용, 자식은 쓰기용 pipe를 close

  ```c
    int main(int argc, char **argv){
        char ch[10];
        int pid, p[2];
        
        if(pipe(p) == -1){
            perror("pipe call");
            exit(1);
        }
        
        pid = fork();
        if(pid == 0){
            close(p[1]);
            read(p[0], ch, 10);
            printf("%s\n", ch);
            exit(0);
        }
        
        close(p[0]);
        scanf("%s", ch);
        write(p[1], ch, 10);
        wait(0);
        exit(0);
    }
  ```

- pipe를 이용한 양방향 통신
  - Pipe 2개 생성
  - fork()에 의해 자식 생성 & pipe 2개 복사
  - pipe1 : 부모는 읽기용, 자식은 쓰기용 pipe를 close
  - pipe2 : 부모는 쓰기용, 자식은 읽기용 pipe를 close

  ```c
    int main(int argc, char **argv){
        int i, in, pid, p[2][2];
        
        for(i=0;i<2;i++){
            pipe(p[i]);
        }
        pid = fork();
        if(pid == 0){
            close(p[0][1]);
            close(p[1][0]);
            read(p[0][0], &in, sizeof(int));
            in++;
            write(p[1][1], &in, sizeof(int));
            exit(0);
        }
        close(p[0][0]);
        close(p[1][1]);
        scanf("%d", &in);
        write(p[0][1], &in, sizeof(int));
        read(p[1][0], &in, sizeof(int));
        printf("%d\n", in);
        wait(0);
        exit(0);
    }
  ```

- Blocking read / blocking write
  - read가 blocking 되는 경우 : pipe가 비어 있는 경우
  - write가 blocking 되는 경우 : pipe가 가득 찬 경우

- pipe 닫기
  - 쓰기 전용 pipe 닫기 : 다른 writer가 없는 경우, read를 위해 기다리던 process들에게 0을 return(EOF과 같은 효과)
  - 읽기 전용 pipe 닫기 : 더 이상 reader가 없으면, writer들은 SIGPIPE signal을 받는다. Signal handling이 되지 않으면 process는 종료; signal handling이 되면, signal 처리 후 write는 -1을 return;

- Non-blocking read / non-blocking write

  - 여러 pipe를 차례로 polling 하는 경우

  - P 하나에서 c1,c2,c3 자식 3개로 부터 데이터를 받을 때와 같은 경우, 여러 pipe를 번갈아가면서 읽어올 때 사용

  - 여러 pipe를 번갈아 가면서 읽을 때, 한 파이프에 읽어올 데이터가 없으면 block이 되니까, 다음 pipe로 넘어가기 위해서 non-block 설정을 해준다

  - 읽으려고 하는데 데이터가 없으면 그냥 넘어감.

  - non-blocking 설정 방법 : 한 줄만 적어주면 된다

    ```c
    #include <fcntl.h>
    
    fcntl(filedes, F_SETFL, O_NONBLOCK);
    ```

  - filedes가 쓰기 전용이고, pipe가 차면 blocking 없이 즉시 -1을 return
    (읽으려고 하는데 데이터가 없으면 그냥 넘어감 : -1 return)

  - 읽기 전용인 경우, pipe가 비어 있으면 즉시 -1 return
    (데이터를 쓰려고 하는데 pipe가 가득차서 못쓰면 또 넘어감 : -1 return)

  - 이 경우, errno는 EAGAIN

  - 추가 정리

    ```
    parent가 3명의 child로부터 데이터를 받게 될 때, 근데 문제는 3명의 child가 언제 데이터를 보낼질 알 수 없을 때
    원래는 blocking이 기본 설정이기 때문에 세 파이프를 번갈아가면서 확인하다가,
    첫 번째 파이프에 데이터가 없을 때 block이 되니까 다음 파이프를 확인을 못하기 때문에,
    여러 파이프를 번갈아가면서 확인하고 데이터를 읽어오기 위해서 non-blocking을 설정해준다.
    ```

- pipe를 이용한 client-server

  - 서버(parent)가 여러개의 클라이언트(child)로부터 데이터를 받는 경우
    - Client는 하나의 pipe로 request를 write
    - Server는 여러 개의 pipe로부터 request를 read
  - no request from any client => server는 blocking
  - a request from any child => read the request
  - more than one request => read them in the order

- select 시스템 호출

  - 위의 예제 이어서, 3개 파이프를 non-blocking 설정해두고 while문을 돌리면 어느순간 파이프 3개가 다 빈 경우가 생기는데, 데이터가 있나 없나 계속 확인하면서 busy waiting을 하게 된다. busy waiting을 하게되면 비효율적이기 때문에 3개의 파이프가 모두 비어있다면 block을 시키고 아닌 경우에만 non-block으로 계속 확인하고 싶음.
    즉, block/non-block 둘 다 왔다갔다 하면서 융통성 있게 쓰고싶다. 이럴 때 select라는 시스템 콜 사용

  - 지정된 file descriptor 집합 중 어느 것이 읽기/쓰기가 가능한지 표시
    (데이터가 왔으면 어느 파이프에 왔는지 알려줌)

  - 읽기, 쓰기가 가능한 file descriptor가 없으면 blocking
    (안 왔으면 select 시스템 콜 block해줌)

  - 영구적 blocking을 막기 위해 time out 사용 가능
    (영원히 block되는걸 막기 위해서 일정 시간 데이터가 안 오면 깨어나게 설정)

  - 사용법

    ```c
    #include <sys/time.h>
    
    int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);
    ```

    - nfds : server가 관심이 있는 file descriptor의 최대 번호(32비트의 fd_set 중 최대 n번까지의 비트만 관심이 있고 확인을 하겠다는 것을 명시. 그래서 1로 설정한 마지막 filedes 번호 + 1 값을 사용)
    - readfds : 읽기 가능한 file descriptor(읽기할 때 어디에서 데이터 왔는지)
    - writefds : 쓰기 가능한 file descriptor(쓰기할 때 어느 파이프를 통해 쓸 수 있는지, 어느 파이프가 비어있는지)
    - errorfds : 오류 발생한 file descriptor를 bit pattern으로 표현(오류 메시지를 쓰는 경우 어느 파이프를 통해 쓸 수 있는지)
    - timeout : timeout 값 설정

  - fd_set 관련 매크로 함수. 파이프는 파일과 같이 파일 테이블에 순차적으로 저장된다. fd_set에 표시

    - void FD_ZERO(fd_set *fdset);
      - fdset 초기화
    - void FD_SET(int fd, fd_set *fdset);
      - fdset의 fd bit를 1로 설정
    - int FD_ISSET(int fd, fd_set *fset);
      - fdset의 fd bit가 1인지 검사. 어디로 데이터가 들어왔는지 체크.
    - void FD_CLR(int fd, fd_set *fset);
      - fdset의 fd bit를 0으로 설정. 읽기용 파이프 4, 7, 9로 쓰다가 9번 파이프에서 읽을 데이터가 없어서 닫아버리면 9번 파이프를 체크할 필요가 없기 때문에 특정 file des만 0으로 설정하고 싶을 때 사용

  - timeval의 구조

    ```c
    struct timeval{
        long tv_set;
        long tv_usec;
    }
    ```

    - timeout이 발생하면
      - NULL이면 해당 event가 발생 시까지 blocking
      - 0이면, non-blocking
      - 0이 아닌 값이면, read/write가 없는 경우 정해진 시간 후에 return

  - Select return 값

    - -1 : 오류 발생 시
    - 0 :  timeout 발생 시
    - 0 보다 큰 정수 : 읽기/쓰기 가능한 file descriptor의 수

  - 주의 사항 : return 시 mask를 지우고 재설정해줘야함

  - select 사용 예제

    - master주소를 바로 안 주고 set에 복사해서 준 이유 : 계속 확인하고 싶은 4, 7, 9를 1로 세팅해서 OS에게 전달을 해준다. 근데 7번만 데이터가 있고 4, 9는 없으면 4, 9만 0으로 만들어서 다시 나에게 전달을 한다. 이런 경우처럼 set값들이 변경되는 경우 다시 원상복구하기 위해서 매번 select를 할 때마다 set을 master로 reset해줘야한다
    - 데이터 없으면 block했다가 데이터가 들어오면 block깨어나서 / 데이터가 어디서 왔는지 확인함. fd_set 배열 중 하나 1인게 있으면 데이터를 읽어주면 된다. 데이터가 1인 경우 읽어줄 때 read가 0보다 큰지 확인하는 작업이 더 추가 되는데 이유는, (또, 모든 child가 작업 완료 후 종료하면 데이터가 안 와서 blocking이 돼서 영원히 block이 되지 않을까 생각을 할 수 있음. 근데 그러지 않은 이유는)
      - ==> (중요) child가 종료하게 되면 파이프가 닫혀서 read를 하는 경우 return을 안 하는 것이 아니라, 0을 return한다. child가 종료하면 그 쪽 파이프는 항상 0을 return하여 데이터가 온 것처럼 표시된다. 그래서 이 예제에서 모든 child가 종료하게 되면 모든 3개의 파이프가 다 0을 return 하여 데이터가 왔다고 표시가 되고, 그러면서 select의 return 값이 3이 된다. child가 종료했다는 0을 세 파이프가 동시에 반환을 하고 있는 것이다. 그래서 일부러 0보다 큰 경우에만 출력하라고 제한을 한 것임

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
    #define MSGSIZE 6
    
    char *msg1 = "hello";
    char *msg2 = "bye!!";
    
    void parent(int[][2]);
    int child(int[]);
    
    int main() {
        int pip[3][2];
        int i;
    
        for (i=0; i<3; i++){
            pipe(pip[i]);
            if(fork()==0)
                child(pip[i]);
        }
        parent(pip);
        for (i=0; i<3; i++) {
            wait(0);
        }
        exit(0);
    }
    
    void parent(int p[3][2]){
        char buf[MSGSIZE], ch;
        fd_set set, master;
        int i, j, k;
    
        //쓰기용 다 닫음
        for (i=0; i<3; i++)
            close(p[i][1]);
    
        //fd_set master 전체 0으로 설정
        FD_ZERO(&master);
    
        //p[0][0], p[1][0], p[2][0] 이 세 개만 사용하여 데이터를 읽겠다
        for (i=0; i<3; i++)
            FD_SET(p[i][0], &master);
    
        int test;
    
        while (set=master, (select(p[2][0]+1, &set, NULL, NULL, NULL)) > 0) {
            for (i=0; i<3; i++) {
                if (FD_ISSET(p[i][0], &set)) {
                    if ((test = read(p[i][0], buf, MSGSIZE)) >0)
                        printf(".....%d\n", test);
                        printf("MSG from %d=%s\n", i, buf);
                }
            }
            //waitpid -1 인자, 아무나 종료한 child가 있는지 확인하는 것임
            //종료할 child가 없는데 왜 wait을 하는거지? 하면서 -1을 return함
            //결국 아래 코드는 모든 child가 종료했는지 확인하는 코드
            if (waitpid(-1, NULL, WNOHANG) == -1)
                return;
        }
    }
    
    int child(int p[2]){
        int count;
        close(p[0]);
        for (count=0; count<2; count++){
            write(p[1], msg1, MSGSIZE);
        }
        sleep(getpid()%4);
        write(p[1], msg2, MSGSIZE);
        exit(0);
    }
    ```

- 