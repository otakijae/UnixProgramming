# Chapter4. 시스템 정보

- uid 검색
  
  - uid : 만든 사람의 id
  
- euid : 지금 실행중인 사람의 id
  
  ```c
  #include<sys/types.h>
  #include<unistd.h>
  uid_t getuid(void);
  uid_t geteuid(void);
  ```
  
- guid 검색
  
- gid : 만든 사람의 그룹 id

- egid : 지금 실행중인 사람의 그룹 id

  ```c
  #include<sys/types.h>
  #include<unistd.h>
  gid_t getgid(void);
  gid_t getegid(void);
  ```

- Directory tree

  - sub directory : directory 바로 밑에 directory

  - descendant : directory 밑에 모든 sub directory

  - 사용법

    ```c
    #include <ftw.h>
    int ftw(const char *path, int(*func)(), int depth);
    ```

  - path에서 시작해서 recursive하게 subdirectory와 file들에 func() 함수를 적용
    
  - 시작점. cwd부터 탐색하겠다 하면 "." 주면 된다. 아니면 dir 이름
  
  - func()는 무조건 int를 반환하는 함수 포인터. 함수시작주소는 함수 이름이다(배열시작주소는 배열 이름인 것 처럼) 함수를 만들어서 인자로 넘겨주면된다
    
  
  - depth : ftw에 의해 사용 가능한 file descriptor 수
    
    파일 테이블 배열은 배열이니까 무제한으로 파일을 여는게 불가능함. 그래서 depth만큼 open하고 close하면서 아래로 내려감
  
- int func() 작성하는 방법
  
  ```c
    int func (const char *name, const struct stat *sptr,int type) { }
    ```
    
  - 항상 마무리는 return 0; (OS가 0을 반환받아야 문제가 없다는 것을 인지하기 때문에 주의할 것)
    
  - 프로그램을 실행시키면 ftw(".", list, 1) 시스템 콜을 호출하니까 그 순간 OS에서 control을 가져감
    
    - ftw 시스템 호출 내부에는 while문으로 list함수(프로그램에 자체적으로 구현한 함수)를 계속해서 실행시킨다
  - 매번 호출 시, 파일/dir에 대한 정보를 준다
      - char * name : 파일/dir 이름
      - const struct stat * status : 파일/dir에 대한 stat 정보를 OS가 가지고 있으면 포인터 주소로 가져올 수 있음
      - int type : 파일인지, 아닌지 알려줌
        - type = FTW_F: object가 파일임
        - type = FTW_D: object가 directory임
        - type = FTW_DNR: object는 읽어올 수 없는 directory. permission 4가 없는 경우
        - type = FTW_NS: ex) stat관련된 정보를 얻어올 수 없는 symlink 타입. symlink인데 가리키는 파일 이름이 변경된 경우


