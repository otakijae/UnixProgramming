# UnixProgramming

## 20180903

- 포인터

  ```c
  #include <stdio.h>

  void swapNumber(int *first, int *second)
  {
      int temp;
      temp = *first;
      *first = *second;
      *second = temp;
  }

  int main()
  {
      int num1 = 10;
      int num2 = 20;

      swapNumber(&num1, &num2);

      printf("%d %d\n", num1, num2);

      return 0;
  }
  ```

  ```c
  20 10
  ```

  변수의 메모리 주소를 함수에 전달하면 함수 안에서는 메모리 주소에 접근하여 값을 저장할 수 있음

  함수의 반환값은 값을 하나만 반환할 수 있는데 이처럼 매개변수를 포인터로 사용하면 함수 바깥으로 여러 개의 값을 전달할 수 있음

  지금까지 scanf 함수를 사용할 때 변수를 그대로 넣지 않고 &를 사용하여 변수의 메모리 주소를 넣었다
  왜냐하면 scanf함수는 값을 여러 개 입력받을 수 있는데 반환값만으로는 여러 개의 값을 함수 바깥으로 가져올 수 없었기 때문

  ```c
  int num1;
  int num2;
  int num3;

  scanf("%d %d %d", &num1, &num2, &num3); // scanf에서 값을 3개 가져옴(scanf는 바깥으로 값을 3개 전달)
  ```

  ![](https://dojang.io/pluginfile.php/614/mod_page/content/31/unit63-4.png)

- 배열과 포인터

  ```c
  #include <stdio.h>

  void printArray(int arr[], int count)    // 배열의 포인터와 요소의 개수를 받음
  {
      for (int i = 0; i < count; i++)
      {
          printf("%d ", arr[i]);
      }

      printf("\n");
  }

  int main()
  {
      int numArr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      printArray(numArr, sizeof(numArr) / sizeof(int));    // 배열과 요소의 개수를 넣음

      return 0;
  }
  ```

  ```
  1 2 3 4 5 6 7 8 9 10
  ```

  - 함수에서 배열을 매개변수로 사용할 때는 매개변수 이름 뒤에 []만 붙이면 됨
    (대괄호 안에 크기를 지정하더라도 무시된다)
  - 즉, arr[]은 배열의 메모리 주소를 담고 있는 포인터이며 배열의 실제 크기를 알 수 없으므로 다른 매개변수를 통해 배열의 크기를 받아야함
  - 이때 요소의 개수를 구하는 방법은 sizeof(배열) / sizeof(자료형)임
    배열의 크기인 10을 그대로 넣어도 되지만 sizeof로 크기를 구하면 배열의 크기가 바뀌었을 때 코드를 수정하지 않아도 됨
  - printArray 안에서는 매개변수 arr과 count를 사용하여 배열의 요소를 출력할 수 있음
  - 매개변수를 arr[]과 같이 지정했다면 arr은 포인터임
    따라서 다음과 같이 함수 안에서 매개변수로 받은 배열의 요소를 변경하면 함수 바깥에 있는 배열의 요소가 바뀌게 됨

  ```c
  #include <stdio.h>

  void printArray(int *arr, int count)    // 매개변수를 포인터로 지정하여 배열을 받음
  {
      for (int i = 0; i < count; i++)
      {
          printf("%d ", arr[i]);
      }

      printf("\n");
  }

  int main()
  {
      int numArr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      printArray(numArr, sizeof(numArr) / sizeof(int));    // 배열과 요소의 개수를 넣음

      return 0;
  }
  ```

  ```
  1 2 3 4 5 6 7 8 9 10
  ```

  - 1차원 배열은 int *arr과 같이 int형 포인터로 받을 수 있음
    물론 포인터로는 배열의 크기를 알 수 없으므로 다른 매개변수를 통해 배열의 크기를 받을 수 있음
  - (pArr + n)은, 포인터 pArr이 가리키는 곳에서 n칸 건너편의 요소의 값을 참조하라 라는 뜻
  - 공식이라 생각하기 : arr[i] == *(arr + i)
  - 배열을 다룰 때 범위를 넘어선 접근을 하지 않도록 주의해야함. 포인터 연산을 하다보면, 간혹 선언해놓은 배열의 범위를 종종 넘어서는 경우가 있음. 이러한 오류는 컴파일 과정에서 발견되지 않고, 원하지 않는 결과를 가져옴. 주의할 것

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
  $ rm filename [해당 file 삭제]
  $ mkdir dirname [해당하는 이름의 directory를 생성]
  $ cd dirname [해당 directory로 이동]
  $ chmod 0700 filename (or dirname) [해당 file(또는 directory)의 access permission 변경]
  ```

- 실습1. vi editor 작성 실습

  ```c
  #include<stdio.h>

  int main(void){
      
      int i, data[10];
      
      for (i=0;i<10;i++){
          scanf("%d", &data[i]);
      }
      
      for (i=0;i<10;i++){
          data[i]=data[i]+5;
      }
      
      for (i=0;i<10;i++){
          printf("%-5d", data[i]);
      }
      
      printf("\n");
      
      return 0;
  }
  ```

- 실습2. 함수 사용 실습

  ```C
  int add_five(int data){
      return data + 5;
  }

  int main(void){
      int i, data[10];

      for(i=0;i<10;i++){
          scanf("%d", &data[i]);
          data[i] = add_five(data[i]);
      }

      for(i=0;i<10;i++){
          printf("%-5d", data[i]);
      }

      printf("\n");

      return 0;
  }
  ```

- 실습3. 포인터 사용 연습

  ```c
  int add_five(int *data){
      *data = *data + 5;
  }

  int main(void){
      int i, data[10];

      for(i=0;i<10;i++){
          scanf("%d", &data[i]);
          add_five(&data[i]);
      }

      for(i=0;i<10;i++){
          printf("%-5d", data[i]);
      }

      printf("\n");

      return 0;
  }
  ```

- 실습4. 배열 포인터 사용 연습

  ```c
  int add_five(int *data){
      int i;
      for(i=0;i<10;i++){
          scanf("%d", &data[i]);
          data[i] = data[i] + 5;
      }
  }

  int main(void){
      int i, data[10];

      add_five(data);

      for(i=0;i<10;i++){
          printf("%-5d", data[i]);
      }

      printf("\n");

      return 0;
  }
  ```

  ```c
  int add_five(int *data){
      int i;
      for(i=0;i<10;i++){
          scanf("%d", (data + i));
          *(data + i) = *(data + i) + 5;
      }
  }

  int main(void){
      int i, data[10];

      add_five(data);

      for(i=0;i<10;i++){
          printf("%-5d", data[i]);
      }

      printf("\n");

      return 0;
  }
  ```

- 실습5

  ```
  //home directory에 LAB09-03 directory를 만든 후, 1번, 2번, 3번, 4번에서 만든 파일들을 LAB09-03 directory로 옮긴다

  $ mkdir LAB09-03
  $ mv test1.c LAB09-03
  $ mv test1.out LAB09-03
  ```

- 실습6

  ```
  //LAB09-03 directory의 파일 중 프로그램 파일에는 0600, 실행파일에는 0700의 access
  permission을 부여 한다

  $ chmod 0600 test1.c
  $ chmod 0700 test1.out
  ...
  ```

- 실습7

  ```
  // LAB09-03 directory에 0700의 access permission을 부여한다

  $ chmod 0700 LAB09-03
  ```

- ​
