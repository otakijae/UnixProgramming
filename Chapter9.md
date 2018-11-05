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