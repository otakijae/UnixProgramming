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

  ```c
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

## 20180905

- 1-a.		
  "test1" file을 나만 읽기쓰기 가능하게 생성하여 쓰기용으로 open 한 후, “test1" file에 문자 변수 ch에 저장된 문자를 10번 반복하여 씁니다. ch은 ‘X'로 초기화 되어 있습니다. 프로그램 종료 후 shell 상에서 cat 명령을 사용하여 ”test1" file에 문자 ’X'가 10개 쓰여 있는지 확인 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      char ch = 'X';
      int i, fd = open("test1", O_WRONLY | O_CREAT, 0600);
      for(i=0;i<10;i++){
          write(fd, &ch, 1);
      }
      return 0;
  }
  ```

   1-b.		
  “test2" file을 나는 읽기쓰기 가능하고, 나와 같은 그룹의 사용자들은 읽기만 가능하게 생성하여 쓰
  기용으로 open 합니다. “test2" file에 정수 변수 data에 저장된 정수를 10번 반복하여 씁니다. data는
  35로 초기화 되어 있습니다. 프로그램 종료 후 shell 상에서 cat 명령을 사용하여 ”test2" file에 35가
  10번 쓰여 있는지 확인 합니다. shell 상에서 ls -l 명령을 사용하여 “test2" file의 크기를 확인 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      int i, data = 35;
      int fd = open("test2", O_WRONLY | O_CREAT, 0640);
      for(i=0;i<10;i++){
          write(fd, &data, 4);
      }
      return 0;
  }
  ```

   1-c.		
  이미 존재하는 “test1" file을 쓰기 가능하게 open 합니다. “test1" file에 문자 변수 ch에 저장된 문자를 5번 반복하여 씁니다. ch은 ‘Y'로 초기화 되어 있습니다. 프로그램 종료 후 shell 상에서 cat 명령을 사용하여 ”test1" file에 문자 ’Y'가 5개, 이어서 문자 ‘X'가 5개 쓰여 있는지 확인 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      char ch = 'Y';
      int i, fd = open("test1", O_WRONLY);
      for(i=0;i<5;i++){
          write(fd, &ch, 1);
      }
      return 0;
  }
  ```

- 1-d.
  이미 존재하는 “test1" file 내용을 보존하고, file의 가장 마지막 위치부터 쓰기 가능하게 open 합니 다. “test1" file에 문자 변수 ch에 저장된 문자를 5번 반복하여 씁니다. ch은 ‘Z'로 초기화 되어 있습니 다. 프로그램 종료 후 shell 상에서 cat 명령을 사용하여 ”test1" file에 문자 ’Y'가 5개, 이어서 문자 ‘X' 가 5개, 그리고 마지막으로 문자 ’Z'가 5개 쓰여 있는지 확인 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      char ch = 'Z';
      int i, fd = open("test1", O_WRONLY | O_APPEND);
      for(i=0;i<5;i++){
          write(fd1, &ch, 1);
      }
      return 0;
  }
  ```

- 1-e.
  이미 존재하는 “test1" file의 기존 내용을 전부 지우고, 쓰기 가능하게 open 합니다. “test1" file에 문자 변수 ch에 저장된 문자를 3번 반복하여 씁니다. ch은 ‘K'로 초기화 되어 있습니다. 프로그램 종료 후 shell 상에서 cat 명령을 사용하여 ”test1" file에 문자 ’K'만 3개가 쓰여 있는지 확인 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      char ch = 'K';
      int i, fd = open("test1", O_WRONLY | O_TRUNC);
      for(i=0;i<3;i++){
          write(fd, &ch, 1);
      }
      return 0;
  }
  ```

- 1-f.
  “test3" file이 이미 존재하면, file open을 포기하고 ”test3" file이 존재하지 않는 경우, 나는 읽기쓰 기 가능하고 다른 모든 사용자들은 읽기만 가능하게 생성 한 후, 쓰기 가능하게 open 합니다. "test3" file의 open에 실패한 경우에는 오류 메시지를 출력하고, 프로그램을 종료 합니다. 그렇지 않은 경우에 는 “test3" file에 문자 변수 ch에 저장된 문자를 10번 반복하여 씁니다. ch은 ‘K'로 초기화 되어 있습니 다. 프로그램 종료 후 shell 상에서 cat 명령을 사용하여 ”test3" file의 내용을 확인 합니다. 문자 ’K'가 10개가 쓰여 있는지 확인 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      char ch = 'K';
      int i, fd = open("test3", O_WRONLY | O_CREAT | O_EXCL, 0644); // 교수님 0640으로 하셨음

      if(fd < 0){ // or fd == -1, 항상 오픈이 제대로 됐는지 확인하는 과정이 있어야 좋음
          printf("File opening failed");
      }
      else{
          for(i=0;i<10;i++){
              write(fd, &ch, 1);
          }
      }
      return 0;
  }
  ```

- 1-g.
  위의 프로그램을 다시 한 번 실행 시켜, 오류 메시지 없이 실행이 되는지 확인 합니다.

  ```c
  //1차적으로는 잘 열리지만, 한 번 더 실행시키면 O_EXCL 때문에 오류메세지가 뜨게 됨. 그렇게 설정을 해놨으니까
  //File opening failed
  ```

- 1-h.
  “test2" file을 읽기 가능하게 open하고, 정수 5개를 읽어 정수 배열 data[10]의 0번부터 4번 원소 에 저장합니다. data[10] 배열은 0으로 초기화 되어 있습니다. printf() 명령을 사용하여 data 배열의 내 용을 출력 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      int data[10] = {0}, i, fd = open("test2", O_RDONLY);

      read(fd, data, 5 * sizeof(int));

      for(i=0;i<10;i++){
          printf("%-5d", data[i]);
      }

      return 0;
  }
  ```

  ​

  다음과 같은 작업을 수행하는 mycopy(실행 파일 이름)라는 프로그램을 작성 하시오.

- 2-a / b / c.
  두 개의 file 이름을 문자열로 입력을 받는다. (scanf() 명령을 사용 합니다.)
  첫 번째 이름의 file이 존재하지 않으면, 오류 메시지를 출력한다.
  첫 번째 이름의 file이 존재하면, 첫 번째 이름의 file의 내용을 복사하여, 두 번째 이름의 file에 쓴다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>
  #include<string.h>

  #define BUFSIZE 512

  int main(){

      char file1[100], file2[100], buffer[BUFSIZE];
      int index, user_input, fd1, fd2, nread;

      //a. 두 개의 file 이름을 문자열로 입력을 받는다
      scanf("%100s %100s", file1, file2);
      fflush(stdin);

      fd1 = open(file1, O_RDONLY);

      if (fd1 == -1) {
          printf("file opening failed");
          return 0;
      } else {
      //c. 첫 번째 이름의 file이 존재하면, 첫 번째 이름의 file의 내용을 복사하여 두 번째 이름의 file에 쓴다
          fd2 = open(file2, O_WRONLY | O_CREAT | O_EXCL, 0600);
          nread = read(fd1, buffer, BUFSIZE);
          while (nread > 0) {

              write(fd2, buffer, nread);

              nread = read(fd1, buffer, BUFSIZE);
          }
      }
  }

  ```

- 2-d / e.
  만약, 두 번째 이름의 file이 이미 존재하면, file 복사 전에 경고 message를 출력하고, 경고 message 후 정수 1을 입력 받으면, 복사를 수행한다. 만약, 정수 1 이외의 입력을 받으면 복사를 포기 한다.
  두 번째 이름의 file이 이미 존재하고, 정수 1이 입력된 경우, 기존 file의 내용은 지우고, 첫 번째 이 름의 file을 복사한다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  #define BUFSIZE 512

  int main() {
      char file1[100], file2[100], buffer[BUFSIZE];
      int index, user_input, fd1, fd2, nread;

      //a. 두 개의 file 이름을 문자열로 입력을 받는다
      scanf("%100s %100s", file1, file2);
      fflush(stdin);

      fd1 = open(file1, O_RDONLY);

      //b. 첫 번째 이름의 file이 존재하지 않으면, 오류 메시지를 출력한다
      if (fd1 == -1) {
          printf("file opening failed\n");
          return 0;
      } else {
          //c. 첫 번째 이름의 file이 존재하면, 첫 번째 이름의 file의 내용을 복사하여, 두 번째 이름의 file에 쓴다
          //d. 만약, 두 번째 이름의 file이 이미 존재하면, file 복사 전에 경고 message를 출력하고, 경고 message 후 정수 1을 입력 받으면 복사를 수행한다
          //만약 정수 1 이외의 입력을 받으면 복사를 포기한다

          fd2 = open(file2, O_WRONLY | O_CREAT | O_EXCL, 0600);

          //두 번째 파일이 존재하는 경우
          if(fd2 == -1){
              printf("Warning : Existing file can be modified. Are you sure to overwrite the file?\n");
              scanf("%d", &user_input);

              if(user_input == 1){
                  //overwrite 동의해서 삭제 후 다시 생성
                  fd2 = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0600);
              } else{
                  printf("terminating");
                  return 0;
              }
          }

          //복사 수행
          nread = read(fd1, buffer, BUFSIZE);
          while(nread > 0){
              write(fd2, buffer, nread);
              nread = read(fd1, buffer, BUFSIZE);
          }
      }

      return 0;
  }
  ```

  - ### mycopy 실습 요점

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

  - 참고 코드

    ```c
    #define BUFSIZE 512
    #define PERM 0644

    //name1을 name2로 복사한다
    int copyfile(const char *name1, const char *name2){
        int infile, outfile;
        ssize_t nread;
        char buffer[BUFSIZE];

        if((infile = open(name1, O_RDONLY)) == -1)
            return -1;

        if((outfile = open(name2, O_WRONLY | O_CREAT | O_TRUNC, PERM)) == -1){
            close(infile);
            return -2;
        }

        //name1으로부터 한 번에 BUFSIZE 문자를 읽는다
        while((nread = read(infile, buffer, BUFSIZE)) > 0){
            //buffer를 출력 화일에 쓴다
            if(write(outfile, buffer, nread) < nread){
                close(infile);
                close(outfile);

                //쓰기오류
                return -3;
            }

            close(infile);
            close(outfile);

            if(nread == -1)
                //마지막 읽기에서 오류
                return -4;
            else
                //copy 완료
                return 0;
        }
    }
    ```

## 20180910

- p3-1.
  아래 코멘트 부분에 해당하는 코드를 작성하여, 10개의 정수를 data1[] 배열에 입력받아 파일에 저장 한 후, 다시 파일에서 읽은 10개의 정수를 data2[] 배열에 저장하고, 두 배열의 내용을 출력하는 프로그램을 작성 하시오. lseek() 명령을 사용 합니다.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>

  int main(void){
      int i, fd, data1[10], data2[10];

      for (i=0;i<10;i++){
          scanf("%d", &data1[i]);
      }
      fd = open("data1", O_RDWR | O_CREAT, 0600);
      // 배열 data1[]에 저장 된 정수를 “data1" 파일에 저장한다.
      write(fd, data1, 10 * sizeof(int));
      //쓰고나면 파일 포인터가 제일 마지막에 가있기 때문에 lseek사용해서 포인터 위치 변경해주면 됨
      lseek(fd, 0, SEEK_SET);
      // “data1" 파일에 저장된 정수를 읽어 data2[] 배열에 저장한다.
      read(fd, data2, 10 * sizeof(int));
      
      for (i=0;i<10;i++){
          printf("%-5d", data1[i]);
      }
      printf("\n");
      for (i=0;i<10;i++){
          printf("%-5d", data2[i]);
      }
      printf("\n");

      return 0;
  }
  ```

- p3-2.
  a. 읽기쓰기 가능한 "data2" file을 생성하고, 문자 ‘X'를 10개 씁니다.
  b. "data2" file의 첫 번째 위치로 file pointer를 옮긴 후 문자 ‘Y'를 하나 씁니다.		
  c. "data2" file의 가장 마지막 위치로 file pointer를 옮긴 후 문자 ‘Y'를 하나 씁니다.
  d. "data2" file의 2, 4, 6, 8번째 문자를 ‘Z'로 바꾸어 씁니다.
  e. "data2" file의 15번째 위치에 문자 ‘T'를 씁니다.
  f. "data2" file의 뒤에서 2번째 위치에 문자 ‘S'를 씁니다.
  g. 'S' 바로 앞에 문자 ‘W'를 씁니다.
  h. 프로그램 실행 후 cat 명령을 이용하여 “data2" file의 내용이 ”YZXZXZXZXXYWST“ 인지 확인 합니
  다. vi 명령으로 ”data" file의 내용을 확인 합니다. cat 명령으로 확인한 내용과의 차이점을 설명 하시
  오.

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){
      int fd, i;
      char ch;

      ch = 'X';

      fd = open("data2", O_RDWR | O_CREAT | O_TRUNC, 0600);

      for(i=0;i<10;i++){
          write(fd, &ch, sizeof(char));
      }

      lseek(fd, 0, SEEK_SET);
      write(fd, "Y", sizeof(char));

      lseek(fd, 0, SEEK_END);
      write(fd, "Y", sizeof(char));

      for(i=1;i<8;i+=2){
          lseek(fd, i, SEEK_SET);
          write(fd, "Z", sizeof(char));
      }

      lseek(fd, 14, SEEK_SET);
      write(fd, "T", sizeof(char));

      lseek(fd, -2, SEEK_END);
      write(fd, "S", sizeof(char));

      lseek(fd, -2, SEEK_CUR);
      write(fd, "W", sizeof(char));

      return 0;
  }
  ```

  - 결과

  index 0부터 14 위치까지 문자를 썼지만 cat 명령으로 data2를 보면 14개의 문자만 출력이 되고, vi 명령으로 파일을 보면 중간에 특수 문자가 있는 것을 볼 수 있다
  index 11 위치에는 데이터 아무것도 쓰지 않았기 때문에 cat 명령으로는 아예 출력이 안 된 것이고, vi 명령으로는 빈 값을 뜻하는 쓰레기 값이 들어가있어서 이렇게 출력이 된 것임

  ```
  //cat data2
  YZXZXZXZXXYWST

  //vi data2
  YZXZXZXZXXY^@WST
  ```

  ### P3-2 실습에서 요점 및 주의할 점

  - SEEK_END는 항상 맨 마지막 문자 다음 위치로 위치한다
  - 포인터는 데이터를 쓴 후, 그 다음 데이터가 쓰여질 위치로 이동한다
  - 끝에서 두 번째 위치에 문자를 쓰라고 하면 -2를 하는 이유가 SEEK_END는 항상 맨 마지막 문자 다음 위치로 위치하기 때문

## 20180912

- p3-3.c	
  data1 배열의 0-4번째 정수를 “data3" 파일의 0, 2, 4, 6, 8번째 위치에 저장
  data1 배열의 5-9번째 정수를 “data3" 파일의 1, 3, 5, 7, 9번째 위치에 저장
  "data3" 파일에 저장된 정수 10개를 읽어 data2 배열에 저장

  ```c
  #include<stdio.h>
  #include<sys/types.h>
  #include<sys/stat.h>
  #include<fcntl.h>
  #include<unistd.h>

  int main(){

      int i, fd, k, k1, data1[10], data2[10];

      fd=open("data3", O_RDWR|O_CREAT|O_TRUNC, 0600);

      for (i=0;i<10;i++){
          scanf("%d", &data1[i]);
      }

      for(i=0;i<5;i++){
          write(fd, data1 + i, sizeof(int));
          lseek(fd, 4, SEEK_CUR);
      }

      for(i=9;i>4;i--){
          lseek(fd, -4, SEEK_CUR);
          write(fd, data1 + i, sizeof(int));
          lseek(fd, -8, SEEK_CUR);
      }

      read(fd, data2, 10 * sizeof(int));

      for (i=0;i<10;i++) {
          printf("%-5d", data1[i]);
      }

      printf("\n");

      for (i=0;i<10;i++){
          printf("%-5d", data2[i]);
      }

      printf("\n");

      return 0;
  }
  ```

  int는 4byte씩 데이터가 쓰여진다는 것을 주의할 것
  처음 파일 포인터가 0부터 시작해서 int 데이터 하나를 쓰면, 포인터는 0123까지 데이터를 쓰고 그 다음 쓰여질 4에 포인터가 위치한다
  array[0] 0 1 2 3		파일 위치
  array[1] 4 5 6 7		파일 위치
  array[2] 8 9 10 11		파일 위치
  array[3] 11 12 13 14	파일 위치
  array[4] 15 16 17 18	파일 위치
  array[5] 19 20 21 22	파일 위치
  array[6] 23 24 25 26	파일 위치
  array[7] 27 28 29 30	파일 위치
  array[8] 31 32 33 34	파일 위치
  array[9] 35 36 37 38	파일 위치


- p4-1
- p4-2
- p4-3
- p4-4
- p4-5
- p4-6
- p4-7