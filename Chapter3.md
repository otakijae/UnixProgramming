# Chapter3. 파일과 디렉토리

## 20180912

### mycopy 실습 요점 / 나중에 시험에서 read & write 시 주의할 점

```C
		nread = read(fd1, buffer, BUFSIZE);
        while(nread > 0){
            write(fd2, buffer, nread);
            nread = read(fd1, buffer, BUFSIZE);
        }
```

이렇게 nread 값을 사용해서 write하는 이유 :
write할 때 512 byte 단위로 받아오는데, 512 그대로 write하면 (마지막 512byte에서 512만큼 다 안 채워져있는 경우, 불필요한 아무 것도 안 없는 쓰레기 부분마저 write를 하기 때문에) copy 같은 기능을 수행할 때 파일이 더 커져서 내용이 복사가 될 수 있다.
그래서 nread라는 변수에 read()한만큼의 byte를 저장해서 그만큼만 write를 해주면 된다.

read return값

- 성공시 실제읽힌 byte 수 / 실패시 -1
- return 값 < nbytes이면, 파일에 남은 data가 nbytes보다 적은 경우
- 더 이상 읽을 내용이 없으면 return 값은 0
- 혹시나 -1이 반환이 되는 경우 read에 실패한건데, 없는 파일을 열거나 open 잘못했을 때 -1을 return 하니까 어디서 -1을 return 하는지 잘 확인해보기
- 이걸로 파일에서 더 읽을게 있는지 없는지 파악하면 됨

write return 값

- 보통은 return값 = n
- return 값 < n이면, 쓰는 도중 media가 full
- 만약 쓰기 전에 꽉 차면 -1 return

## 파일과 디렉토리

- 디렉토리라는 파일이 있는 것임. 폴더라는 것이 사실 존재하지는 않음
### 파일 정보의 획득
- stat - open되지 않은 파일에 대한 정보도 획득 가능
- fstat - open된 파일에 대한 정보 획득 가능

  ```c
  #include<fcntl.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  int stat (const char *pathname, struct stat *buf);
  int fstat (int filedes, struct stat *buf);
  ```

- buf엔 파일 정보가 저장됨

  - st_dev, st_ino		identifier (논리적 장치번호와 inode 번호)
  	 st_mode 			permission mode
  	 st_nlink 			 	link의 수
  	 st_uid, st_gid			user의 uid와 gid
  	 st_rdev				file이 장치인 경우만 사용
  	 st_size				논리적 크기
  	 st_atime, st_mtime, st_ctime	file의 최근 access time,update time, stat 구조의 update time
  	 st_blksize			I/O block 크기
  	 st_blocks			파일에 할당된 block의 수

  ```c
  struct stat {
                 dev_t     st_dev;         /* 장치파일의 위치 및 여부 device id */
                 ino_t     st_ino;         /* 파일의 inode 번호 */
                 mode_t    st_mode;        /* file permission 정보 */
                 nlink_t   st_nlink;       /* 하드링크의 갯수 */
                 uid_t     st_uid;         /* user id */
                 gid_t     st_gid;         /* group id */
                 dev_t     st_rdev;        /* 장치파일(inode)를 기술 */
                 off_t     st_size;        /* 해당 파일의 총 크기*/
                 blksize_t st_blksize;     /* 효율적인 I/O 파일 시스템 위한 블럭 사이즈*/
                 blkcnt_t  st_blocks;      /* 할당된 블럭 사이즈 */
             };
             
  // 출처: http://cokk.tistory.com/51 [試行錯誤(시행착오)]
  ```

  - example.c

  ```C
  #include<fcntl.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<stdio.h>

  int main(){
      //os는 구조체 타입으로 stat/fstat의 값을 반환하기 때문에 구조체 타입 선언이 필요함
      struct stat buf;
      stat("data", &buf);
      //%o 8진수 출력, st_mode &0777 비트화해서 출력하는 것은 그 부분만 출력하기 위해서
      printf("%o %ld\n", buf.st_mode&0777, buf.st_size);
  }
  ```

  - 실행 및 출력

  ```
  [s13011022@bce LAB09-12]$ gcc example.c
  [s13011022@bce LAB09-12]$ ./a.out
  644 6

  [s13011022@bce LAB09-12]$ ls -l
  합계 20
  -rwxr-xr-x. 1 s13011022 class 8632  9월 12 16:55 a.out
  -rw-r--r--. 1 s13011022 class    6  9월 12 16:55 data
  -rw-r--r--. 1 s13011022 class  306  9월 12 16:55 example.c
  ```

### Access Permission

- [file mode](https://github.com/ninetyfivejae/UnixProgramming/blob/master/Chapter1.md#file-system-permissions)

- 상징형 모드
  0764 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH
  [S_I]
  [R/W/X]
  [USR/GRP/OTH]

- uid : 파일 만든 사람, owner의 id.
  소유권자 : 파일을 생성한 사람, uid, guid

- euid : 파일 사용하는 사람의 id
  사용자 : 파일을 사용하는 사람, euid, egid

- 04000 S_IUID

  - 실행이 시작되면 소유자의 uid가 euid가 된다

  - id = 95인 친구가 내 dir에 들어와서 실행가능한 파일을 실행하면 euid(effective uid)가 95가 된다는 뜻

    |                         나의 id = 73                         |                        친구의 id = 95                        |
    | :----------------------------------------------------------: | :----------------------------------------------------------: |
    |                 내가 만든 Program1 uid = 73                  |                                                              |
    | 0711의 permission. 나는 읽기/쓰기/실행 다 가능. 나 이외는 실행만 가능 |                                                              |
    |                                                              |  친구가 내 dir에 들어와서 프로그램을 실행가능해서 실행했음   |
    | 근데 Program1은 data1이라는 파일을 읽고 쓰는 프로그램임. 근데 내가 data1 파일을 0600 permission을 주게 되면 ==> | 친구는 Program1은 실행 가능하지만 Program1이 읽고 쓰는 data1파일을 열지못하기 때문에 P1프로그램이 실행이 안 된다 |

    이러한 경우, Program1은 실행가능하게 permission을 부여했지만 어차피 data1파일을 못열기 때문에 permission에 의미가 없어진다
    그래서 Program1 실행 도중에만 data1파일을 open 가능하게 만들게 하려면 04711이라는 permission을 주면 된다.
    (data1에다가 permission을 부여하는지? Program1에다가 permission을 부여하는지?)
    그러면 euid가 친구의 id 95에서 73으로 바뀐다.

- 02000 S_ISGID

  - 실행이 시작되면 소유자의 gid가 egid가 된다

- 왜 이런 permission이 생겼는지
  그냥 data파일을 0644 permission을 부여하면 되지 않는가 생각할 수 있지만 이건 너무 안일한 생각임
  그러면 다른사람들이 data파일을 cat 명령으로 확인 가능함
  가령 비밀번호 변경 프로그램 ```passwd``` 처럼 data1이라는 파일에 사용자들의 비밀번호가 저장돼있을 수 있는데, 사용자들의 비밀번호를 owner말고 다른 사람들이 조회를 하면 안 되는 상황에 사용

  - 실행 파일이고,
  - 여러 사람이 실행시킬 수 있는 프로그램인데
  - 파일을 open해서 데이터에 접근하고 있고,
  - 근데 데이터는 나만 읽고 쓰기 하고싶은 경우

- 파일 접근 권한 확인 / 파일 종류 확인

  ```c
  //0400 Read USER
  if(s.st_mode&S_IRUSR)
  	printf("소유자 읽기 권한 설정");

  //파일 종류 확인
  if(S_ISREG(s.st_mode))
  	printf("일반 파일")

  const char *get_filetype(struct stat *buf)
  {
      if(S_ISREG(buf->st_mode))
          return "regular";
      if(S_ISDIR(buf->st_mode))
          return "directory";
      if(S_ISCHR(buf->st_mode))
          return "charater";
      if(S_ISBLK(buf->st_mode))
          return "block";
      if(S_ISFIFO(buf->st_mode))
          return "fifo";
      return "unknown";
  }
  ```

- Access 시스템 호출

  - 특정 파일에 대한 읽기/쓰기/실행이 가능한지 확인하는 system call

  - 사용법

    ```c
    #include <unistd.h>
    int access(const char *pathname, int amode);
    ```

    - amode : R_OK , W_OK, X_OK, 또는 F_OK(파일이 존재하는지) ```F_OK | X_OK``` 파일 존재하고, 실행 가능한지 둘 다 참이면 0, 하나라도 아니면 -1
    - return 값 : 0 or -1
    - euid가 아니라 uid에 근거하여 process가 file에 접근 가능한지를 표현

- chmod 시스템 호출

  - 특정 파일의 access permission을 변경하는 system call

  - 사용법

    ```C
    #include <sys/types.h>
    #include <sys/stat.h>
    int chmod(const char *pathname, mode_t mode);
    int fchmod(int fd, mode_t mode);
    ```

  - 소유자만 사용 가능함

- link 시스템 호출

  - 기존 파일에 새로운 이름을 부여하는 system call

  - 사용법

    ```c
    #include <unistd.h>
    int link (const char *orginal_path, const char *new_path);
    ```

    return 값 : 0 or new_path가 이미 존재하는 이름이면 -1반환

  - 한 파일에 하나 이상의 이름
    파일에 두 개 이상의 이름을 붙일 수 있음

    ```link("a.c", "b.c")``` 원래는 a.c 라는 이름표가 파일을 가리키고 있는데, b.c라는 이름표로 하나 더 가리키는 것임
    (시험볼 때 그림그리라고 하면 둘 다 가리키고 있는 모습을 그려야함. a.c를 원래 이름이라고 해서 포인터로 가리키고 있는 것을 안 그리면 안 됨)
    파일에 원래 이름이란 것이 딱 없어서 a.c, b.c 둘 다 가리키고 있는 것임

  - link count : link의 수

  - unlink ```unlink("a.out")```
    link를 제거 하는 것. link_count— —;
    파일은 삭제가 되는 것이 아니라 unlink() 함수 기능으로 파일을 가리키고 있는 이름표 포인터를 제거하는(unlink) 것임
    이름표 포인터 link를 모두 제거하면 파일을 가리키고 있는 포인터가 없어서 파일에 접근을 못하는 것임
    link_count가 0이 되면 free block으로 실제로 파일이 제거된다고 함

  - 예제

    ```
    $ vi test1
    //aaaaa
    //bbbbb
    //ccccc
    //ddddd
    //eeeee
    ```

    ```c
    #include<stdio.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<fcntl.h>
    #include<unistd.h>

    int main(){

            link("test1", "test2");
            link("test1", "A/B/C/test3");

            return 0;
    }
    ```

    test1에 대한 link test2, test3를 생성하면 이렇게 link수가 3으로 연결된 이름 세 개가 존재한다는 것을 알 수 있고 terminal앱 자체에서 파일 이름 색도 같게 표시해주고 셋 중에 하나의 파일을 변경해도 다 같이 바뀜(수정 시간이 같은 것을 보면 알 수 있음)

    ```
    [s13011022@bce LAB09-12]$ ls -l
    합계 48
    drwxr-xr-x. 3 s13011022 class 4096  9월 12 20:13 A
    -rwxr-xr-x. 1 s13011022 class 8632  9월 12 16:55 a.out
    -rw-r--r--. 1 s13011022 class    6  9월 12 16:55 data
    -rw-r--r--. 1 s13011022 class  306  9월 12 16:55 example.c
    -rw-r--r--. 1 s13011022 class  183  9월 12 20:18 link.c
    -rwxr-xr-x. 1 s13011022 class 8512  9월 12 20:18 link.out
    -rw-r--r--. 3 s13011022 class   19  9월 12 20:20 test1
    -rw-r--r--. 3 s13011022 class   19  9월 12 20:20 test2
    ```

    그래서 link는 파일 하나에 여러개의 이름표 포인터를 가지고 있다고 보면 됨

- symbolic link / 주의 link와 다름. 윈도우에서 바로가기라고 생각하면 됨

  -  link의 제한점

    - 디렉토리에 대한 link생성 불가
    - 다른 파일 시스템에 있는 파일에 대해서는 link생성 불가

  - 그래서 symbolic link 자체가 파일인, 그 안에 다른 file에 대한 경로 수록

  - 사용법

    ```c
    #include <unistd.h>
    int symlink (const char *realname, const char *symname);
    ```

    return 값 : 0 or symname이 이미 존재하면 -1 반환

  - 예제. link 예제 이어서

    ```
    $ vi test1
    //aaaaa
    //bbbbb
    //ccccc
    //ddddd
    //eeeee
    ```

    ```c
    #include<stdio.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<fcntl.h>
    #include<unistd.h>

    int main(){

            symlink("test1", "test_sym");

            return 0;
    }
    ```

    ```
    [s13011022@bce LAB09-12]$ ls -l
    합계 48
    drwxr-xr-x. 3 s13011022 class 4096  9월 12 20:13 A
    -rwxr-xr-x. 1 s13011022 class 8632  9월 12 16:55 a.out
    -rw-r--r--. 1 s13011022 class    6  9월 12 16:55 data
    -rw-r--r--. 1 s13011022 class  306  9월 12 16:55 example.c
    -rw-r--r--. 1 s13011022 class  215  9월 12 20:41 link.c
    -rwxr-xr-x. 1 s13011022 class 8568  9월 12 20:41 link.out
    -rw-r--r--. 3 s13011022 class   31  9월 12 20:40 test1
    -rw-r--r--. 3 s13011022 class   31  9월 12 20:40 test2
    lrwxrwxrwx. 1 s13011022 class    5  9월 12 20:42 test_sym -> test1
    ```

    이런 상태에서
    test1, 2, 3는 link로 같은 파일을 가리키고 있으니 test1, 2, 3 중 하나를 변경하면 다 같이 변경이 된다.

    ```
    $ vi test_sym
    //aaaaa
    ```

    근데 test_sym 파일을 vi로 변경을 해서 aaaaa만 남겨두고 나머지를 다 지운다
    그리고 ls -l 명령으로 확인해보면 수정 시간이 test1, 2 , 3만 바뀌고 test_sym 파일 자체는 수정 시간이 변동이 안 돼있다
    test_sym 파일은 사실상 test1이라는 파일을 가리키고만 있고 내용은 가지고 있지 않는 것이다
    test_sym이라는 파일을 따로 만들어놓은 것이고, test1이라는 이름표 포인터를 갖고 있는 것임

## 6주차 첫 프로젝트 진행할 예정. 2시간 안에 해결하기. 프로젝트 시 노트북 사용 안 됨

- readlink
  symbolic file안에 있는 내용을 보여줌

  - 사용법

    ```c
    #include <unistd.h>
    int readlink (const char *sympath, char *buffer, size_t buffsize);
    ```

    ```c
    char ch[100] = {0};
    readlink("test", ch, 99);
    printf("%s\n", ch);
    ```

  ​	return : 성공하면 0 / 이름이 공간보다 크면 안 된다고 알려줌 -1 반환

- lstat

  - stat(test, ~)	: test 정보말고 test가 가리키고 있는 A/B/C/test에 대한 정보를 준다. Symbolic link의 파일에 대한 정보를 얻지 못함

  - lstat(test, ~)	: symbolic link 자체 파일 정보를 받기 위해 사용

  - 사용법

    ```c
    #include <sys/types.h>
    #include<sys/stat.h>
    int lstat (const char *linkname, struct stat *buf);
    ```

- directory 생성

  - Directory file에 .와 ..를 넣어서 생성

  - 사용법

    ```C
    #include <sys/types.h>
    #include <sys/stat.h>
    int mkdir(const char *pathname, mode_t mode);
    ```

    ```c
    mkdir("A", 0733);
    ```

  - directory에서의 permission은 조금 다름

    - 읽기 4 : directory 내의 directory와 파일들을 볼 수 있다는 의미
    - 쓰기 2 : directory와 file을 만들 수 있다
    - 실행 1 : directory 안으로 들어갈 수 있다. 1이 없게 되면 들어가지 못하기 때문에 나머지 permission들이 의미가 없어지니까, 다른 permission을 허용하는 경우 같이 써줌

- directory 제거

  - directory가 비어있는 경우 제거 가능

  - 사용법

    ```c
    #include <unistd.h>
    int rmdir(const char *pathname);
    ```

- Rename

  - 이름 바꾸기

  - 사용법

    ```c
    #include <stdio.h>
    int rename (const char *oldpathname, const char *newpathname);
    ```

  - newpathname이 존재해도 -1을 반환하지 않음
    기존 file을 제거하고 새 이름 부여하기 때문

  - file은 그렇지만, directory의 경우에는 하위에 directory와 file이 존재하는 경우에는 지워지지 않음

- getcwd

  - current working directory

  - 사용법

    ```c
    #include <unistd.h>
    char *getcwd(char *name, size_t size);
    ```

    ```c
    ch[100] = {0};
    getcwd(ch, 99);
    printf("%s", ch);
    ```

  - return

    - Current working directory의 경로 이름 반환

  - size는 실제 길이 보다 +1이 커야 한다
    마지막 NULL 문자 고려

  - 성공 시 directory의 경로 이름이 name에 copy
    실패 시 null pointer return;

- chdir

  - change directory

  - 사용법

    ```c
    #include <unistd.h>
    int chdir(const char *path);
    ```

    ```c
    fd1=open(“/usr/ben/abc”, O_RDONLY);
    fd2=open(“/usr/ben/xyz”, O_RDWR);

    ==> 이걸 아래와 같이 변경 가능

    chdir(“/usr/ben”);
    fd1=open(“abc”, O_RDONLY);
    fd2=open(“xyz”, O_RDWR);
    ```

    ```c
    ch[100] = {0};

    mkdir("T1", 0700);
    chdir("T1");

    mkdir("T2", 0700);
    chdir("T2");

    mkdir("T3", 0700);
    chdir("T3");

    getcwd(ch, 99);
    printf("%s", ch);
    ```

    ```
    [s13011022@bce LAB09-17]$ ./example.out
    /home/account/class/tspark/s13011022/LAB09-17/T1/T2/T3
    [s13011022@bce LAB09-17]$ 
    ```

    이렇게 결과가 출력이 되는데, 프로그램이 종료하면 원래 위치로 복귀하는 것을 볼 수 있다
    LAB09-17에 있는 프로세스와 T3에 있는 프로세스가 다른 것임
    쉘 프로세스에서 명령 하나하나가 실행될 때마다 child 프로세스를 만들어서 명령을 수행하기 때문에 쉘 프로세스와 child 프로세스가 다르게 실행되고 다른 역할을 함
    child는 T3 위치에 가있지만, 쉘 프로세스는 원 위치에 그대로 있음
    첫 프로젝트 때 주의해서 설계할 것

- Directory 열기 및 닫기

  - Directory 열기 사용법

    ```c
    #include <sys/types.h>
    #include <dirent.h>
    DIR *opendir(const char *dirname);
    ```

  - DIR형의 data structure에 대한 pointer를 return
    실패 시 null pointer return

  - Directory는 사실 파일이고, 내부에 목록 리스트로 데이터를 가지고 있음
    그래서 파일이 가지고 있는 entry 정보를
    운영체제가 entry 일부만 가져와서 가지고 있으면
    P1은 포인터를 가지고 entry 값을 참조한다 ==> 약간 이해가 안 가지만 계속 찾아보다가 질문

  - Directory 닫기 사용법

    ```c
    #include <dirent.h>
    int closedir(DIR *dirptr);
    ```

- Directory 읽기

  - 사용법

    ```c
    #include <sys/types.h>
    #include <dirent.h>
    struct dirent *readdir (DIR *dirptr);
    ```

  - return 값 : dirptr이 가리키는 DIR 구조내의 한 항

  - struct dirent

    - ino_t d_ino

    - char d_name[]

    - ```c
      struct dirent{
          ino_t d_ino;				//inode number
          off_t d_off;				//offset to the next dirent
          unsigned short d_reclen;	//length of this record
          unsigned char d_type;		//type of file. not supported by all file system types
          char d_name[256];			//filename
      }
      ```

  - pointer dirptr은 read 후 다음 항을 가리킨다.

  - directory의 끝에 도달하면 null pointer를 return

  - 현재 디렉토리 파일과 dir정보 출력 실습 예제

    ```c
    int main(){
            DIR *dp;
            struct dirent *d;
            dp = opendir(".");
            d = readdir(dp);
            while(d != NULL){
                    printf("%ld : %s \n", d->d_ino, d->d_name);
                    d = readdir(dp);
            }

            return 0;
    }
    ```

- Directory file pointer 이동

  - 사용법

    ```c
    #include <sys/types.h>
    #include <dirent.h>
    void rewinddir(DIR *dirptr);
    ```

    ​