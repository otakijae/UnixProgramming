# Chapter1. UNIX 기본 사용법

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

## File System Permissions

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

  ​

