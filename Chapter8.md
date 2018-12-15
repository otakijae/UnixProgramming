# Chapter8. 메모리 매핑

- 메모리 매핑은 파일을 프로세스의 메모리에 매핑하는 것이다.
  즉, 프로세스에 전달할 데이터를 저장한 파일을 직접 프로세스의 가상 주소 공간으로 매핑한다.
  따라서 read, write함수를 사용하지 않고도 프로그램 내부에서 정의한 변수를 사용해 파일에서 데이터를 읽거나 쓸 수 있다. 

  Memory mapping : file의 프로세스의 memory에 mapping

  매핑된 데이터에 변동이 있을 경우, OS가 다른 프로세스에 알려주어 메모리 매핑을 통한 통신을 한다

- mmap함수

  - 파일을 프로세스의 가상 메모리에 매핑할 수 있다.
  - 매핑할 메모리의 위치와 크기를 인자로 받는다.
  - 메모리에 매핑된 데이터는 파일 입출력 함수를 사용하지 않고 직접 읽고 쓸 수 있다는 장점이 있다

  ```c
  #include <sys/mman.h>
  
  void *mmap(void *addr, size_t len, int prot, ing flags, int fildes, off_t off);
  ```
  - fildes가 가리키는 파일에서 off로 지정한 위치부터 len만큼의 데이터를 읽어 addr이 가리키는 메모리 공간에 매핑한다.
    - addr : 매핑할 메모리 주소. 매핑하려는 주소를 직접 지정하거나, NULL이라고 하면 OS가 적당한 공간에 해줌
    - len : 메모리 공간의 크기
    - prot : 보호 모드. 데이터를 읽기만 할지, 쓰기나 실행도 허용할지 등 보호 모드 지정
      - PROT_READ : 읽기 허용
      - PROT_WRITE : 쓰기 허용
      - PROT_EXEC : 실행 허용
      - PROT_NONE : 매핑된 파일에 접근할 수 없음
    - flags : 매핑된 데이터의 처리 방법을 지정하는 함수
      - MAP_SHARED : 변경 내용 공유. 다른 사용자와 데이터의 변경 내용을 공유한다. 이 플래그가 설정되어 있으면 쓰기 동작은 매핑된 메모리의 내용을 변경한다.
      - MAP_PRIVATE : 변경 내용 공유하지 않음. 데이터의 변경 내용을 공유하지 않는다. 이 플래그가 설정되어 있으면 최초의 쓰기 동작에서 매핑된 메모리의 사본을 생성하고 매핑 주소는 사본을 가리킨다.
    - fildes : 파일 기술자
    - off : 파일 오프셋
  - mmap함수는 페이지 단위로 메모리 매핑을 실행한다. 매핑된 영역의 마지막 페이지에 남는 부분은 0으로 채운다.
  - 매핑된 메모리 영역을 벗어난 공간에 접근하려고 하면 SIGBUS 혹은 SIGSEGV 시그널이 발생한다.
  - mmap함수는 수행 성공하면 매핑된 메모리의 시작 주소를 반환한다. 매핑된 영역의 크기는 시작주소 + len이 된다.

- munmap : Memory mapping 해제

  - 해제 방법

    ```c
    #include <sys/mman.h>
    
    int munmap(void *addr, size_t len);
    ```

    - addr : mmap의 반환값

  - 예제

    ```c
    int main(int argc, char **argv){
    	int fd;
        caddr_t addr;
        
        fd = open(open(argv[1], O_RDWR));
        
        addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);
        
        printf("%s", addr);
        
        if(munmap(addr, statbuf.st_size) == -1){
            perror("munmap");
            exit(1);
        }
        
        //이 부분에서 세그멘테이션 결함 오류 남. 이미 해제된 메모리 영역에 접근을 시도했기 때문
        printf("%s", addr);
    
        return 0;
    }
    ```

- mprotect : 보호 모드 변경

  - 변경 방법

    ```c
    #include <sys/mman.h>
    
    int mprotect(void *addr, size_t len, int prot);
    ```

    - 읽기만 하도록 설정했는데 중간에 쓰기를 하고 싶을 때 사용

- Truncate 파일 크기 변경

  - 변경 방법

    ```c
    #include <unistd.h>
    
    //open하지 않은 파일
    int truncate(const char *path, off_t len);
    //open한 파일
    int ftruncate(int fildes, off_t len);
    ```

    - 파일이 없는데 새로 만든 경우나 파일 실제 크기보다 더 큰 크기의 데이터를 쓰려고 할 때, 예를 들어서 파일 크기가 0인데 50만큼 쓰려고 하는 경우, 오류가 남
    - 그래서 truncate함수로 파일 크기를 늘려줘야한다.

- 예제

  - ```c
    //read, write 사용 시, 파일을 열고 필요에 따라 파일의 오프셋을 이동시키고, read함수를 호출해 데이터를 buf로 읽어와서 작업한다.
    fd = open(...);
    lseek(fd, offset, whence);
    read(fd, buf, len);
    
    //mmap함수를 사용하는 경우, 일단 파일을 여는 것까지는 같음. 이후 열린 파일의 내용을 mmap함수를 사용해 메모리에 매핑하고, 이후의 작업은 address가 가리키는 메모리 영역의 데이터를 대상으로 수행한다. 즉 매번 read함수로 데이터를 읽어올 필요가 없다.
    fd = open(...);
    address = mmap((caddr_t)0, len, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, offset);
    ```

  - off로부터 len 길이 만큼 데이터를 가져옴

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
    
            int i, fd;
            char *addr;
    
            fd = open("data", O_RDWR);
            addr = mmap(NULL, 10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            //addr = mmap(NULL, 10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 4096);
    
            for(i=0;i<5;i++){
                    printf("%c : ", *(addr + i));
                    *(addr + i) = *(addr + i) + 1;
                    printf("%c\n", *(addr + i));
            }
    
            printf("%s\n", addr);
    
            exit(0);
    }
    ```

  - parent에서 수정한 파일 child에서도 확인 가능한지, 통신 가능한지 확인하는 예제

    - 여기서 주의할 것. MAP_SHARED라고 설정을 해줘야 하는데, parent가  MAP_PRIVATE으로 설정을 하게 되면, child는 바뀌지 않은 파일을 출력하게 된다. PRIVATE설정은 메모리 매핑한 것에만 쓰이고 실제 파일에는 영향을 끼치지 않아서 통신을 하지 못하게되니 주의할 것.

    ```c
    #define BUFSIZE 512
    
    int main(int argc, char **argv){
    
            int i, fd;
            char *addr;
            pid_t pid;
    
            fd = open("data2", O_RDWR);
    
            pid = fork();
            if(pid == 0){
                    addr = mmap(NULL, 50, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
                    printf("[child] %s\n", addr);
            }
            else{
                    addr = mmap(NULL, 50, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
                    for(i=0;i<5;i++){
                            *(addr + i) = *(addr + i) + 1;
                    }
                    printf("[parent] %s\n", addr);
                    wait(0);
            }
    
            exit(0);
    }
    ```

  - 파일 크기 변경하여 쓰고 읽어오기. ftruncate사용. 안 써주면 오류남. 파일이 없는데 새로 만든 경우나 파일 실제 크기보다 더 큰 크기의 데이터를 쓰려고 할 때, 예를 들어서 파일 크기가 0인데 50만큼 쓰려고 하는 경우, 오류가 남

    ```c
    #define BUFSIZE 512
    
    int main(int argc, char **argv){
    
            int i, fd;
            int *addr;
            pid_t pid;
    
            fd = open("data3", O_RDWR|O_CREAT, 0600);
    
            pid = fork();
            if(pid == 0){
                    addr = mmap(NULL, 50, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
                    printf("[child] %d\n", *(addr + 3));
            }
            else{
                    addr = mmap(NULL, 50, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
                    ftruncate(fd, sizeof(int) * 5);
                    for(i=0;i<5;i++){
                            *(addr + i) = i;
                    }
                    printf("[parent] %d\n", *(addr + 1));
                    wait(0);
            }
    
            exit(0);
    }
    ```

- msync. 매핑된 메모리 동기화 방법.

  ```c
  #include <sys/mman.h>
  
  int msync(void *addr, size_t len, int flags);
  ```

  - memory 매핑시 OS가 수정사항을 파일에 곧바로 수정하지 않음. 다른 프로세스에게 전달이 즉시 안 되고 비동기적으로 작업이 이뤄진다. 근데 기본 설정은 비동기이지만, 동기적으로 처리하라고 설정가능함

  - 간단하게 데이터를 주고 받을 수 있는 수단임

  - flags 인자

    - MS_ASYNC : 비동기적 쓰기 작업. mysnc함수는 즉시 리턴하고, 함수가 리턴한 후 적절한 시점에 쓰기 작업을 수행한다.
    - MS_SYNC : 동기적 쓰기 작업. 쓰기 작업을 완료할 때까지 mysnc함수가 리턴하지 않는다.

  - 예제. 프로그램 실행 후 바로 cat명령해서 확인해보면 바로 변경돼있는 것을 확인할 수 있음. 비동기작업으로 실행하면 OS가 바쁜 경우 바로 변경이 안 될 수 있음.

    ```c
    int main(int argc, char **argv){
        int fd;
        caddr_t addr;
        
        fd = open(open(argv[1], O_RDWR));
        
        addr = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, (off_t)0);
        
        printf("%s", addr);
        
        printf("-----\n");
        addr[0] = 'D';
        printf("%s", addr);
        
        msync(addr, statbuf.st_size, MS_SYNC);
        
        return 0;
    }
    ```

