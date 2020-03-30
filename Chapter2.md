# Chapter2. 파일 입출력

## 20180903

- UNIX file 접근 primitives

  - File : byte들의 linear sequence
  - (하드웨어, 시스템 측) 저수준 파일 입출력 : open, close, read, write, dup, dup2, fcntl, lseek, fsync
  - (애플리케이션 측) 고수준 파일 입출력 : fopen, fclose, fread, fwrite, fputs, fgets, fprintf, fscanf, freopen, fseek
  - ==> 이렇게 지금까지 배운 것들은 쉽게 사용할 수 있게 누군가 만들어놓은 함수/기능
  - ==> Shell 상의 명령 또한 누군가가 system call로 구현해놓은 기능들을 사용하는 것임
  - 수업 때는 우리가 직접 system call를 활용하여 기능들을 구현할 것임

- file descriptor

  - 현재 open된 파일을 구분할 목적으로 UNIX가 붙여놓은 번호

  - OS는 파일 open 시, 가령 file table p1에서 배열로 파일에 대한 정보를 저장해둔다.
    
    [FILE *fp; 파일 포인터] 어떤 파일을 파일 포인터로 가리키고 있는지와 같은 파일에 대한 모든 정보들 저장
    
  - 그러면서 파일에 대한 정보가 배열에 저장이 되니까 배열의 index로 파일을 구분함
    
    - 0 : 표준 입력
    - 1 : 표준 출력
    - 2 : 표준 오류 출력
    - 3 : 여기서부터 파일 입출력
    - ...
    
  - file table이 배열이니까 한 프로세스가 동시에 open할 수 있는 file의 갯수가 제한이 있음. 그래서 close라는 system call을 사용해서 닫아줘야함

- open 시스템 호출

  - 기존의 file을 open하거나, 새롭게 file을 생성하여 open하는 system call

  - 기존의 file이 있으면 filename과 oflag만 적어주면 되고, 기존에 file이 없으면 만들기위해 mode까지 적어주면 된다.

    - filename : 파일 이름

    - oflag : file을 access하는 방식

      ```
      파일을 access하는 방식 굳이 나눠놓은 이유는, permission과 관련
      가령, 파일 open할 때 나에게 읽기 권한이 없는데 읽으려고 하면 파일이 열리지 않는다.
      권한과 맞아들어가야 파일을 열 수 있기 때문에 구분돼있다.
      ```
      - O_RDONLY (읽기만), O_WRONLY (쓰기만), O_RDWR (읽기, 쓰기 다)
      - O_CREAT (파일 생성)
      - O_EXCL (exclusive), O_TRUNC (지우고 다시), O_APPEND (이어서)

    - mode : file을 새로 생성할 때만 사용. 0600, 0664, 0644 등등

    - return 값
      - 실행 성공 시 file descriptor (음이 아닌 정수) 반환
      - 실행 실패 시 -1 반환

  - 사용법

    ```c
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<fcntl.h>

    int open(const char *filename, int oflag, [mode_t mode]);
    ```

    ```c
    fd1 = open("test1.c", O_RDWR);
    fd2 = open("test2.c", O_RDWR | O_CREAT, 0600);
    ```

    - test1.c 파일을 읽기/쓰기 전용으로 열겠다
    - test2.c 파일을 읽기/쓰기 전용으로 여는데, 기존에 파일이 없으면 0600권한(나만 읽기/쓰기 가능하게)으로 만들겠다
    - 실행 중 기존에 파일이 있으면 O_CREAT 옵션과 권한이 무시됨

    ```c
    fd = open("test.c", O_RDWR | O_CREAT | O_EXCL, 0600);
    ```

    - O_EXCL : 기존에 파일이 있으면 안 열고, 없어야만 파일을 만들어서 열겠다. Exclusively 파일이 없어야만 실행
    - 기존에 파일이 있으면 파일을 안 열면서 실행 실패 -1을 return. fd = -1 이렇게 된다
    - [주의] O_EXCL 옵션은 O_CREAT랑 같이 사용해야만함

    ```c
    fd = open("test.c", O_RDWR | O_CREAT | O_TRUNC, 0600);
    ```

    - O_TRUNC : 기존에 파일이 있으면 파일 지우고 다시 만든다. 없으면 파일 생성
    - [주의] O_TRUNC 옵션은 O_CREAT랑 같이 사용해야만함

    ```c
    fd = open("test.c", O_RDWR | O_CREAT | O_APPEND, 0600);
    ```

    - O_APPEND : 기존에 파일이 있으면 덮어쓰지 않고, 이어서 작성한다. 파일 포인터를 제일 마지막으로 보낸다. 없으면 파일 생성
    - 기존에 파일을 열면 파일포인터가 제일 앞에 위치해있어서 파일에 write하면 맨 앞에서부터 덮어쓰게 됨
    - 근데 덮어쓰지 않고, 이어서 작성하려면 O_APPEND 옵션을 붙여주면 된다.
    - 하지만 read할 때는, O_APPEND 옵션을 붙이면 파일 포인터가 맨 뒤로 가서 꼬이게 된다.

- creat 시스템 호출

  - file을 생성하여 open하거나, 기존 file을 open하는 system call

  - 사용법

    ```c
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    
    int creat(const char *filename, mode_t mode);
    ```

    ```c
    open("file", O_WRONLY | O_CREAT | O_TRUNC, mode);
    ```

    - file을 쓰기 가능한 상태로 open함
    - file이 이미 존재하면, 두 번째 권한 인자는 무시함. 기존 파일 존재하면 open하면서 0 반환

- close 시스템 호출

  - open된 file을 close할 때 사용

  - 프로세스 종료 시 open된 file들을 자동으로 close해주지만, 동시에 open할 수 있는 file수가 제한되기 때문에(파일 테이블 배열에 올라가니까) close를 사용한다

    - file des : open된 파일의 file descriptor;

    - return 값 : 성공 시 0; 실패 시 -1;

    ```c
    #include <unistd.h>
  
    int close(int filedes);
    ```

    ```c
    close(fd)
    ```

- read 시스템 호출

  - open된 file로부터 지정한 byte수 만큼의 data를 읽어 지정된 저장장소에 저장하기 위한 명령

  - 사용법

    ```c
    #include <unistd.h>
    
    ssize_t read(int filedes, void *buffer, size_t nbytes);
    ```

    - filedes : open된 file의 file descriptor

    - *buffer : 읽은 data를 저장할 곳의 주소; data type은 상관 없음

    - nbytes : 읽을 byte 수, data type에 상관 없이 지정된 byte 수 만큼 읽음

      - 읽어올 데이터 타입에 맞게 byte 지정해주기

      - 가령, int를 읽어야하는데 4byte 말고 5byte라고 하면 정수를 제대로 읽어오지 않고 하나 더 읽어와서 꼬임

    - return값
      - 성공시 실제읽힌 byte 수 / 실패시 -1
      - return 값 < nbytes이면, 파일에 남은 data가 nbytes보다 적은 경우
      - 더 이상 읽을 내용이 없으면 return 값은 0
      - 혹시나 -1이 반환이 되는 경우 read에 실패한건데, 없는 파일을 열거나 open 잘못했을 때 -1을 return 하니까 어디서 -1을 return 하는지 잘 확인해보기
      - 이걸로 파일에서 더 읽을게 있는지 없는지 파악하면 됨

    ```c
    #include <unistd.h>
    
    char buffer[100];
    fd = open("test", O_RDWR | O_CREAT, 0600);
    n = read(fd, buffer, 4 * sizeof(int));
    ```

    - 16byte만큼 읽어서 buffer에 저장

- write 시스템 호출

  - 지정된 저장장소에서 지정한 byte수 만큼의 data를 읽어 open된 file에 쓰는 명령

  - File pointer or read-write pointer : 쓰여질 다음 byte의 위치를 나타냄

  - 사용법

    ```c
    #include <unistd.h>
    
    ssize_t write(int filedes, const void *buffer, size_t nbytes);
    ```

    - filedes : write를 할 file의 descriptor

    - *buffer : write 할 내용이 들어 있는 저장 장소의 주소

    - nbytes : write 할 byte의 수

    - return값
      - 보통은 return값 = n
      - return 값 < n이면, 쓰는 도중 media가 full
      - 만약 쓰기 전에 꽉 차면 -1 return

  - 기존 file을 open하고 바로 write하면, 포인터가 맨 처음을 가리키고 있어서 처음 문자부터 덮어씌움
    그래서 ```fd = open("data", O_WRONLY | O_APPEND);``` 이렇게 파일 포인터를 파일 맨 끝으로 이동시켜줘야함

## 20180910

### 저번 시간 질문

- 왜 rwx 인자로 정할 때 맨 앞에 0으로 해야하는지, 4자리 중에서 맨 앞에 0은 뭐인지
  - 8진수 notation임. 앞에 0700, 0600 처럼 0 붙으면 8진수 표기
  - 0x는 16진수 표기


- 35 write 하면 아스키코드 문자 #으로 변경되는 이유
  - cat 명령을 사용하면 정수4byte를 한 byte씩 아스키코드로 바꿔서 문자로 보여준다
  - 그래서 '35'를 아스키코드 문자로 변형한 '#'으로 출력한 것임
  - 확인을 위해 데이터크기를 확인해보면 정수 4byte * 10 == 40으로 돼있는 것을 알 수 있음
  - 파일 크기로 확인하는게 부족하면 정수를 write한 파일을 read()해서 제대로 숫자가 적혀있는지 확인을 하면 됨

### 저번 시간 복습 및 주의할 점

1. 아래와 같이, 문자를 반복해서 입력받고 파일에 write를 하면 이어서 작성을 하기 때문에
   각각의 입력이 구분되지 않는다

   ```c
   int main(){
       int i, fd;
       char ch[100];

       fd = open("data1", O_WRONLY | O_CREAT, 0600);
       
       for(i=0;i<5;i++) {
   				scanf("%s", ch);
   				write(fd, ch, strlen(ch));
   		}
   }
   ```


1. 정수를 입력을 받고 파일에 write한 후 cat 명령으로 파일을 확인하면 아스키코드 문자로 변경되어 출력
   cat 명령을 사용하면 정수 4byte를 한 byte씩 아스키코드로 바꿔서 문자로 보여준다
   5개의 정수를 입력받아서 파일에 write하면 파일은 5 * 4byte == 20 byte가 된다.
   이렇게 파일 크기를 통해서 정수가 제대로 쓰였는지 확인해도 되고, write한 파일을 읽어와서 프로그램 내부에서 출력을 해봐도 된다
   cf. 32 이하 정수 or 127 이상 정수를 입력하면 문자로 표현이 안 되기 때문에 출력이 안 될 수 있으나, read & printf 하면 잘 출력됨

   ```c
   int main() {
       int i, fd, in[5];

       fd = open("data2", O_WRONLY | O_CREAT, 0600);
       
       for(i=0;i<5;i++) {
           scanf("%d", in + i);
           write(fd, in + i, sizeof(int));
       }
   }
   ```

   ```c
   int main() {
       int i, fd, in[5];

       fd = open("data2", O_RDONLY);

       for(i=0;i<5;i++) {
           read(fd, in + i, sizeof(int));
           printf("%d", *(in + i));
       }
   }
   ```

2. read 한 번에 5개 하기 / write 한 번에 5개 하기

   ```c
   int main() {
    int data[10] = {0}, i, fd = open("data2", O_RDONLY);
     
    read(fd, data, 5 * sizeof(int));
   
    for(i=0;i<10;i++) {
           printf("%-5d", *(data+i));
       }
   
    return 0;
   }
   
int main() {
       int data[10] = {35,36,37,38,39}, i, fd = open("data2", O_WRONLY);

       write(fd, data, 5 * sizeof(int));

       for(i=0;i<10;i++) {
        printf("%-5d", *(data+i));
       }
   
       return 0;
}
   ```
   
3. 문자열 read시 주의할 점. 문자열 맨 마지막 배열에는 '\0'가 들어가니까 read할 때 고려해줘야함
   문자열 배열 크기가 100인데 문자 읽어올 때 배열 크기 5만큼만 읽어왔으면 마지막 5번째 배열 인덱스에 '\0'을 넣을 것을 명시해줌

   - 뭔지 알겠지만 아직 살짝 감이 없음. 나중에 생각하면서 코딩하기

   ```c
   int main() {
       char ch[100];
       int fd, i, n;

       fd = open("data1", O_RDONLY);
       n = read(fd, ch, 99);
       ch[n] = '\0';
       printf("%s", ch);

       return 0;
   }
   ```

### 강의 내용

- read/write 의 효율성

  - read/write하는 수와 프로세스 실행 수는 비례증가

    - 입력 10번을 받은 것을 write 10번하는 것 (프로세스 10번 실행 중단)
    - 입력 10번을 받은 것을 write 1번하는 것 (프로세스 1번 실행 중단)

    데이터 수가 적으면 큰 차이 없지만, 데이터 수가 엄청 많아지면 실행시간이 많이 차이가 난다.

  - I/O 작업 시 OS 내부적으로 context switching이 일어나기 때문에  system call의 수가 적을수록 효율적인 프로그램이다 (write는 system call의 일부임)
    
- File을 copy하는 프로그램의 실행 시간을 어떻게 줄이느냐 관건
  
- read도 마찬가지
    
  - 가령 파일이 하드디스크에 있으면, OS가 하드디스크에 있는 파일의 정수를 하나씩 포인터로 가져오는 것이 아님
    - 파일의 일부분 한 블록으로 가져와서 거기에서 정수를 가져오는 것임
    - 이 한 블록이 512(disk blocking factor) 크기이고, 따라서 BUFSIZE가 512의 배수일 때 효율적임
    - 즉 프로그램의 배열 크기를 512단위로 해두면 좋음
  
- lseek와 임의 접근

  - open된 file내의 특정 위치로 파일 포인터를 이동시키는 system call
    
  - 지금까지 binary파일의 경우, lseek()사용해서 파일 포인터를 한 번에 150번째 문자 위치로 이동 가능
    
  - But, text 파일에서는 안 된다.
    
    ```c
  #include <sys/types.h>
    #include <unistd.h>
    
    off_t lseek(int filedes, off_t offset, int whence);
  ```
    
  - 사용법

    - filedes : file descriptor
    - offset : whence에서 offset만큼 떨어진 위치로 이동. +/-로 방향설정 가능
    - whence : SEEK_SET(파일 시작 지점), SEEK_CUR(현재 파일포인터 위치), SEEK_END(파일 맨 뒷 지점)
    - return값 : 이동된 위치(시작점부터의 위치) / 이동실패시 -1

  - 현재 파일 포인터 기준에서 -4만큼 이동

    ```c
    lseek(fd, -4, SEEK_CUR);
    ```

  - 1 2 3 4 5
    
    - 5개의 정수를 write했으면 20byte이고 20번째 배열 인덱스에 파일 포인터가 이동해있음
    - 0부터 19번째까지 데이터가 들어가있고, 다음에 데이터를 쓸 위치 20번째로 파일 포인터가 이동해있음
  - 즉, 파일 포인터는 다음에 데이터를 쓰고 읽을 위치에 가 있음
    
  - 위에 처럼 파일 포인터가 20에 위치해있을 때,
    
    - lseek(fd, -4, SEEK_SET);
    - lseek(fd, +4, SEEK_END);
    - 배열의 처음부분보다 전으로 포인터를 이동시키는 것은 불가능하지만  ```-1``` 출력
    - 배열의 마지막부분보다 다음으로 포인터를 이동시키는 것은 가능함 ```24``` 출력

- file 제거

  - 사용법

    ```c
    #include <unist.h>
    int unlink(const char *filename);

    #include <stdio.h>
    int remove(const char *filename);
    ```

  - include 파일이 다르니까 주의

  - file descriptor가 아니라, file 이름을 쓴다
    
    - 파일을 사용하는데 삭제하지는 않음
    - 파일 포인터가 있어서 열린 상태에서는 파일 이 삭제가 안 되나, 닫으면 이름이 삭제가 되어서 포인터가 사라져서 열 수가 없다
    - 엄밀히 말하면, 하드디스크의 파일을 지우는게 아니라 파일 이름 택을 없애는 것임
  - 성공 시 0, 실패 시 -1 return
    
  - unlink는 파일만 삭제
    
  - remove는 파일과 빈 디렉토리도 삭제

- 표준 입력, 표준 출력, 표준 오류

  - 표준 입력(키보드) : fd = 0;
  - 표준 출력(터미널 화면) : fd = 1;
  - 표준 오류(터미널 화면) : fd = 2;

- 표준 입출력의 변경

  - redirection. 입력을 받아야하는 값들을 infile에 미리 저장을 해두어서 일일이 입력을 하는 작업을 안 한다
    
  ```c
    #include<stdio.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<fcntl.h>
    #include<unistd.h>
    #include<string.h>
    
  int main() {
        char ch1[15], ch2[15] = "error message";
    
      int n = read(0, ch1, 15);
    
      write(1, ch1, n);
        write(2, ch2, 15);
    
      return 0;
    }
    ```
    
  ```
    $ ./a.out < infile
    $ ./a.out > outfile
    $ ./a.out < infile > outfile
    ```
    
  - pipe
    
    - 2개의 실행 파일을 동시에 실행
    - a.out1의 표준 출력이 a.out2의 표준 입력으로 들어가게 만들어줘야함
    - a.out1 ==> a.out2
    - write() ==> read()
    - printf() ==> scanf()
  - 파일 하나씩 실행하면 화면에 모두 출력되지만, pipe로 실행하면 최종 값만 화면에 출력된다
    
    ```
  $ ./a.out1 | ./a.out2
    ```

- 오류 메시지 출력

  - perror("error …");

    ```
    error ... : No such file or directory
    ```
