# UnixProgramming

## 20180903

- UNIX 프로그래밍 : UNIX의 system call(OS가 가지고 있는 function을 호출하는 것)을 사용하는 프로그래밍. 예를 들어 운영체제에 구현이 돼있는 SemaphoreWait 함수를 프로세스에서 호출해서 사용하는 것. 

- XShell 사용 방법

  - 새로만들기에서 이름 아무거나

  - Host name : 203.250.148.93

  - User name : s13011022

  - Port : 1074

  - Password : 비밀번호

    ```
    //패스워드 바꾸기
    $ passwd
    Current UNIX password : 지금 비밀번호
    New UNIX password : 새로운 비밀번호
    Retype new UNIX password : 새로운 비밀번호 재입력
    ```

- Mac Terminal에서 ssh로 시스템에 로그인하기

  - $ ssh [ID]@[HOST]
  - $ ssh root@127.0.0.1
  - SSH 접속의 기본포트는 22번인데, 강의에서는 1074 포트 사용한다고 해서 포드번호를 바꿀 때에는 -p [포트번호] 옵션을 붙혀주면 된다
  - 최초 SSH 연결 시 RSA 키를 생성하고 연결을 계속하려는지 묻는 메시지가 나옴. yes를 입력해야 패스워드 입력 요청 메시지가 나타남.

  ```
  $ ssh -p 1074 s13011022@203.250.148.93
  $ s13011022@203.250.148.93's password : 비밀번호 입력
  ```

- vi 에디터

  - 최초의 UNIX용 화면 편집기
  - 최초의 유닉스용 화면 편집기
  - 작고, 빠르고, 모든 유닉스/리눅스 시스템이 기본적으로갖추고 있음
  - 몇가지기본적인기능만을갖추고있으나유닉스의다른 명령들과 결합하여 매우 다양하게 확장, 응용 가능
  - 독특하지만 매우 빠르고 강력한 명령 체계

   ```
  // $ 표시는 명령을 받아들일 준비가 됐다는 의미
  // vi 파일이 없으면 만들어서 파일 실행, 있으면 에디터로 파일 실행
  $ vi myfile.c
   ```

  - 명령모드 : vi 시작하면 기본적으로 들어가는 모드. 텍스트 편집에 필요한 여러 명령들을 입력하는 모드. 메뉴(파일, 편집, 도구 등등) 단계 / 제일 아래 아무것도 없음
  - 편집모드 : i, a 누르면 텍스트를 입력 및 수정하는 모드 / 제일 아래 끼워넣기 or insert 뜸
  - ex모드 : 주로 외부 파일이나 확장 명령을 사용하기 위한 모드. 편집 종료도 ex모드에서 실행 / 제일 아래 : 콜론 표시. w저장 q종료

  ```
  :w			- 저장
  :q			- 종료
  :wq			- 저장 및 종료
  :q!			- 강제 종료. 편집 중인 파일을 저장하지 않고 vi를 종료할 수 없음. 이 때 강제 종료를 굳이 할 때 사용하면 됨

  ctrl + f	- 한 화면만큼 아래로 이동
  ctrl + b	- 한 화면만큼 위로 이동
  ctrl + d	- 화면크기의 반 만큼 아래로 이동
  ctrl + u	- 화면크기의 반 만큼 위로 이동

  H			- 화면의 첫 줄 첫 칸으로 이동
  L			- 화면의 마지막 줄 첫 칸으로 이동
  M			- 화면의 중간 줄 첫 칸으로 이동
  ^			- 현재 줄의 제일 첫 단어로 이동
  $			- 현재 줄의 마지막 단어로 이동
  w			- 한 단어씩 오른쪽으로 이동
  b			- 한 단어씩 왼쪽으로 이동
  15G			- 15번째 줄로 이동
  G			- 파일의 제일 마지막 줄로 이동
  /string		- string 검색
  x			- 커서 위치의 한 문자 삭제
  5x			- 커서 위치의 다섯 문자 삭제
  dw			- 현재 커서 오른쪽 한 단어 삭제
  db			- 현재 커서 왼쪽 한 단어 삭제
  dd			- 현재 커서가 위치한 줄 한 줄 삭제
  15dd		- 현재 커서가 위치한 줄부터 15줄 삭제
  o			- 현재 커서 위치 아래에 한 줄 생성
  O			- 현재 커서 위치 위에 한 줄 생성
  Y			- 현재 커서가 위치한 줄 복사
  15Y			- 현재 커서가 위치한 줄부터 15줄 복사
  P			- 복사 한 내용을 현재 커서가 위치한 줄 아래에 삽입
  u			- 바로 전 명령의 실행을 취소
  .			- 바로 이전 명령을 다시 한 번 더 실행
  %s/aa/bb/g	- 문서 전체의 aa를 bb로 치환
  ```

- 컴파일 및 실행

  gcc test1.c 만 입력 후 실행하면 실행파일이 a.out 으로 만들어져서 컴파일된다. -o 옵션 붙여서 실행파일 이름을 명시할 수 있다.
  -o 붙일 때 프로그램 파일과 같은 이름으로 실행 파일을 만들게 되면 기존의 프로그램 파일 내용에 덮어씌워져서 작업 내용이 날라갈 수 있으니 주의할 것

  ```
  $ gcc test1.c -o test1.out
  ```

  ```
  $ ./test1.out
  $ ./a.out
  ```

- unix

  ```
  $ ls [현 directory 내에 존재하는 모든 subdirectory 및 file들을 listing 하는 명령]
  $ ls -l [각 subdirectory 및 file에 관한 상세 정보를 함께 표시하는 명령]
  $ cat filename [해당 file의 내용을 화면에 출력]
  $ more filename [해당 file의 내용을 한 페이지 씩 화면에 출력. enter or spacebar로 다음페이지]
  $ cp file1 file2 [file1의 내용을 복사하여 file2를 생성]
  $ mv file1 file2 [file1의 이름을 file2로 변경]
  $ mv file1 dir1 [file1을 dir1으로 이동]
  $ rm filename [해당 file 삭제, 바로 삭제되니까 주의, 중요한 것들 copy 미리 해두기]
  $ rmdir dirname [directory 삭제, dir 비어있지 않으면 삭제 불가, 안에 파일/폴더 다 지우고 수행하기]
  $ mkdir dirname [해당하는 이름의 directory를 생성]
  $ cd dirname [해당 directory로 이동]
  $ chmod 0700 filename (or dirname) [해당 file(또는 directory)의 access permission 변경]
  ```


## 20180905

- 실습4 입력 10개 다 받고 덧셈만 함수에서 실행하면 됨. 맞게 함

- File System Permissions

  ```
  $ ls -l
  $ ls -al
  ```

  ```
  합계 4
  drwx------. 2 s13011022 class 4096  9월  5 00:56 LAB09-03

  합계 64
  -rw-------. 1 s13011022 class  233  9월  3 12:35 test1.c
  -rwx------. 1 s13011022 class 8624  9월  3 21:45 test1.out
  -rw-------. 1 s13011022 class  260  9월  3 21:43 test2.c
  -rwx------. 1 s13011022 class 8664  9월  3 21:45 test2.out
  -rw-------. 1 s13011022 class  254  9월  3 21:49 test3.c
  -rwx------. 1 s13011022 class 8664  9월  3 21:49 test3.out
  -rw-------. 1 s13011022 class  262  9월  3 21:55 test4.c
  -rwx------. 1 s13011022 class 8664  9월  3 21:55 test4.out
  ```

  각 subdirectory 및 file에 관한 상세 정보를 함께 표시
  rwx == 4+2+1 == 7
  d : 디렉토리 표시, r : 읽기 가능, w : 쓰기 가능, x : 실행 가능
  0 rwx rwx rwx : 내가 / 나랑 같은 그룹의 유저들이 / 나 및 나랑 같은 그룹 이외의 유저들이 읽/쓰/실 가능
  chmod 0700 filename(or dirname) 할 때 0700의 의미가 이것임
  https://www.tutorialspoint.com/unix/unix-file-permission.htm
  https://en.wikipedia.org/wiki/File_system_permissions

  | Number | Octal Permission Representation                           | Ref  |
  | :----: | --------------------------------------------------------- | :--: |
  | **0**  | No permission                                             | ---  |
  | **1**  | Execute permission                                        | --x  |
  | **2**  | Write permission                                          | -w-  |
  | **3**  | Execute and write permission: 1 (execute) + 2 (write) = 3 | -wx  |
  | **4**  | Read permission                                           | r--  |
  | **5**  | Read and execute permission: 4 (read) + 1 (execute) = 5   | r-x  |
  | **6**  | Read and write permission: 4 (read) + 2 (write) = 6       | rw-  |
  | **7**  | All permissions: 4 (read) + 2 (write) + 1 (execute) = 7   | rwx  |

  | Symbolic Notation | Numeric Notation | English                                                      |
  | :---------------: | :--------------: | ------------------------------------------------------------ |
  |   `----------`    |       0000       | no permissions                                               |
  |   `-rwx------`    |       0700       | read, write, & execute only for owner                        |
  |   `-rwxrwx---`    |       0770       | read, write, & execute for owner and group                   |
  |   `-rwxrwxrwx`    |       0777       | read, write, & execute for owner, group and others           |
  |   `---x--x--x`    |       0111       | execute                                                      |
  |   `--w--w--w-`    |       0222       | write                                                        |
  |   `--wx-wx-wx`    |       0333       | write & execute                                              |
  |   `-r--r--r--`    |       0444       | read                                                         |
  |   `-r-xr-xr-x`    |       0555       | read & execute                                               |
  |   `-rw-rw-rw-`    |       0666       | read & write                                                 |
  |   `-rwxr-----`    |       0740       | owner can read, write, & execute; group can only read; others have no permissions |

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
    0 : 표준 입력
    1 : 표준 출력
    2 : 표준 오류 출력
    3 : 여기서부터 파일 입출력
    ...
  - file table이 배열이니까 한 프로세스가 동시에 open할 수 있는 file의 갯수가 제한이 있음
    그래서 close라는 system call을 사용해서 닫아준다

- open 시스템 호출

  - 기존의 file을 open하거나, 새롭게 file을 생성하여 open하는 system call

  - 기존의 file이 있으면 filename과 oflag만 적어주면 되고, 기존에 file이 없으면 만들기위해 mode까지 적어주면 된다.

    filename : 파일 이름

    oflag : file을 access하는 방식
    파일을 access하는 방식 굳이 나눠놓은 이유는, permission과 관련
    가령, 파일 open할 때 나에게 읽기 권한이 없는데 읽으려고 하면 파일이 열리지 않는다.
    권한과 맞아들어가야 파일을 열 수 있기 때문에 구분돼있다.

    - O_RDONLY(읽기만), O_WRONLY(쓰기만), O_RDWR(읽기, 쓰기 다)
    - O_CREAT(파일 생성)
    - O_EXCL(exclusive), O_TRUNC(지우고 다시), O_APPEND(이어서)

    mode : file을 새로 생성할 때만 사용. 0600, 0664, 0644 등등

    return 값

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

    test1.c 파일을 읽기/쓰기 전용으로 열겠다
    test2.c 파일을 읽기/쓰기 전용으로 여는데, 기존에 파일이 없으면 0600권한(나만 읽기/쓰기 가능하게)으로 만들겠다
    실행 중 기존에 파일이 있으면 O_CREAT 옵션과 권한이 무시됨

    ```c
    fd = open("test.c", O_RDWR | O_CREAT | O_EXCL, 0600);
    ```

    O_EXCL : 기존에 파일이 있으면 안 열고, 없어야만 파일을 만들어서 열겠다. Exclusively 파일이 없어야만 실행
    기존에 파일이 있으면 파일을 안 열면서 실행 실패 -1을 return. fd = -1 이렇게 된다
    O_EXCL 옵션은 O_CREAT랑 같이 사용해야만함

    ```c
    fd = open("test.c", O_RDWR | O_CREAT | O_TRUNC, 0600);
    ```

    O_TRUNC : 기존에 파일이 있으면 파일 지우고 다시 만든다. 없으면 파일 생성
    O_TRUNC 옵션은 O_CREAT랑 같이 사용해야만함

    ```c
    fd = open("test.c", O_RDWR | O_CREAT | O_APPEND, 0600);
    ```

    O_APPEND : 기존에 파일이 있으면 덮어쓰지 않고, 이어서 작성한다. 파일 포인터를 제일 마지막으로 보낸다. 없으면 파일 생성
    기존에 파일을 열면 파일포인터가 제일 앞에 위치해있어서 파일에 write하면 맨 앞에서부터 덮어쓰게 됨
    근데 덮어쓰지 않고, 이어서 작성하려면 O_APPEND 옵션을 붙여주면 된다.
    근데 read할 때, O_APPEND 옵션을 붙이면 파일 포인터가 맨 뒤로 가서 꼬이게 된다.

- creat 시스템 호출

  - file을 생성하여 open하거나, 기존 file을 open하는 system call

  - 사용법

    ```c
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    int creat(const char *filename, mode_t mode);
    ```

    open("file", O_WRONLY | O_CREAT | O_TRUNC, mode);
    file을 쓰기 가능한 상태로 open함
    file이 이미 존재하면, 두 번째 권한 인자는 무시함. 기존 파일 존재하면 open하면서 0 반환

- close 시스템 호출

  - open된 file을 close할 때 사용

  - 프로세스 종료 시 open된 file들을 자동으로 close해주지만, 동시에 open할 수 있는 file수가 제한되기 때문에(파일 테이블 배열에 올라가니까) close를 사용한다

    file des : open된 파일의 file descriptor;

    return 값 : 성공 시 0; 실패 시 -1;

  - ```c
    #include <unistd.h>

    int close(int filedes);
    ```

  - ```c
    close(fd)
    ```

- read 시스템 호출

  - open된 file로부터 지정한 byte수 만큼의 data를 읽어 지정된 저장장소에 저장하는 명령

  - 사용법

    ```c
    #include <unistd.h>

    ssize_t read(int filedes, void *buffer, size_t nbytes);
    ```

    filedes : open된 file의 file descriptor

    *buffer : 읽은 data를 저장할 곳의 주소; data type은 상관 없음

    nbytes : 읽을 byte 수, data type에 상관 없이 지정된 byte 수 만큼 읽음

    읽어올 데이터 타입에 맞게 byte 지정해주기
    가령, int를 읽어야하는데 4byte 말고 5byte라고 하면 정수를 제대로 읽어오지 않고 하나 더 읽어와서 꼬임

    return값

    - 성공시 실제읽힌 byte 수 / 실패시 -1
    - return 값 < nbytes이면, 파일에 남은 data가 nbytes보다 적은 경우
    - 더 이상 읽을 내용이 없으면 return 값은 0
    - 혹시나 -1이 반환이 되는 경우 read에 실패한건데, 없는 파일을 열거나 open 잘못했을 때 -1을 return 하니까 어디서 -1을 return 하는지 잘 확인해보기
    - 이걸로 파일에서 더 읽을게 있는지 없는지 파악하면 됨

  - ```c
    #include <unistd.h>

    char buffer[100];

    fd = open("test", O_RDWR | O_CREAT, 0600);

    n = read(fd, buffer, 4 * sizeof(int));
    ```

    16byte만큼 읽어서 buffer에 저장

- write 시스템 호출

  - 지정된 저장장소에서 지정한 byte수 만큼의 data를 읽어 open된 file에 쓰는 명령

  - File pointer or read-write pointer : 쓰여질 다음 byte의 위치를 나타냄

  - 사용법

    ```c
    #include <unistd.h>

    ssize_t write(int filedes, const void *buffer, size_t nbytes);
    ```

    filedes : write를 할 file의 descriptor

    *buffer : write 할 내용이 들어 있는 저장 장소의 주소

    nbytes : write 할 byte의 수

    return값

    - 보통은 return값 = n
    - return 값 < n이면, 쓰는 도중 media가 full
    - 만약 쓰기 전에 꽉 차면 -1 return

  - 기존 file을 open하고 바로 write하면, 포인터가 맨 처음을 가리키고 있어서 처음 문자부터 덮어씌움
    그래서 ```fd = open("data", O_WRONLY | O_APPEND);``` 이렇게 파일 포인터를 파일 맨 끝으로 이동시켜줘야함

- ​

- 왜 rwx 인자로 정할 때 맨 앞에 0으로 해야하는지, 4자리 중에서 맨 앞에 0은 뭐인지

- 35 write 하면 아스키코드 문자 #으로 변경되는 이유

