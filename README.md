# UnixProgramming

## 20181029

- p8-1.c
  Parent process는 세 개의 child process를 만들고, 1초 간격으로 각 child process에게 SIGUSR1 signal을 보낸 후, child process들이 종료 할 때까지 대기 하였다가, 종료한 child process의 id와 exit status를 출력 후 종료 합니다. 각 child process는 생성 후 parent로부터 signal이 올 때까지 대기 하 였다가 (pause() 사용), signal을 받으면 자신의 process id를 세 번 출력 한 후 종료 합니다. 이와 같 이 동작하는 프로그램을 아래 코드를 사용하여 작성 하시오.

  ```c
  
  ```

- P8-2.c
  Parent process는 다섯 개의 child process들을 만들고, 대기하였다가 child process가 모두 종료 한 후 종료 합니다. 각 child process는 자신의 순서가 될 때까지 대기 하였다가, 1초씩 쉬면서 (sleep (1); 사용) 자신의 process id(getpid(); 사용)를 5회 출력하는 작업을 한 후 종료 합니다. child process의 id 츨력 순서는 생성 순서의 역순이며, 이와 같은 순서 동기화 작업은 signal을 보내서 진행 합니다.

  ```c
  
  ```

- P8-3.c
  10개의 정수를 입력으로 받아 그 합을 출력하는 프로그램을 작성 합니다. 단, 10초간 사용자로부터 의 입력이 없으면, 경고 메시지를 출력하고 다시 입력을 기다립니다.

  ```c
  
  ```

## 20181031

- 
