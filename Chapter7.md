# Chapter7. 시그널

- signal

  - software interrupt
  - kernel => process, process => process
  - 자료 전송보다는 비정상적인 상황을 알릴 때 사용
  - 실행 프로세스 중단시키는게 원래 목적

  예) program 수행 중 CTRL-C(interrupt key) 누르면, kernel이 문자를 감지 후 해당 session에 있는 모든 process에게 SIGINT라는 signal을 보낸다. 모든 process는 종료되지만 shell process는 무시

- signal 종류

  ```
  1. SIGHUP: 연결된terminal이hangup하였을때(terminate)
  2. SIGINT: interrupt key(^C)를입력하였을때(terminate)
  3. SIGQUIT: quit key(^\)를입력하였을때(terminate+core)
  4. SIGILL: illegal instruction을수행하였을때(terminate+core)
  5. SIGTRAP: implementation defined hardware fault (terminate+core)
  6. SIGABRT: abort시스템호출을불렀을때(terminate+core)
  7. SIGBUS: implementation defined hardware fault (terminate+core)
  8. SIGFPE: arithmetic exception, /0, floating-point overflow (terminate+core)
  9. SIGKILL: process를kill하기위핚signal, catch 혹은ignore될수없는signal임(terminate)
  10. SIGUSR1: user defined signal 1 (terminate)
  11. SIGSEGV: invalid memory reference (terminate+core)
  12. SIGUSR2: user defined signal 2 (terminate)
  13. SIGPIPE: reader가terminate된pipe에write핚경우발생(terminate)
  14. SIGALRM: alarm시스템호출후timer가expire된경우(terminate)
  15. SIGTERM: kill시스템호출이보내는software termination signal (terminate)
  16. SIGCHLD: child가stop or exit되었을때parent에게전달되는신호(ignore)
  17. SIGCONT: continue a stopped process (continue/ignore)
  18. SIGSTOP: sendable stop signal, cannot be caught or ignored (stop process)
  19. SIGTSTP: stop key(^Z)를입력하였을때(stop process)
  20. SIGTTIN: background process가control tty로부터read핛경우(stop process)
  21. SIGTTOU: background process가control tty로write핛경우(stop process)
  22. SIGURG: urgent condition on IO, socket의OOB data (ignore)
  23. SIGXCPU: exceeded CPU time limit (terminate+core/ignore)
  24. SIGXFSZ: exceeded file size limit (terminate+core/ignore)
  25. SIGVTALRM: virtual time alarm, setitimer, (terminate)
  26. SIGPROF: profiling time alarm, setitimer, (terminate)
  27. SIGWINCH: terminal window size changed, (ignore)
  28. SIGIO: 어떤fd에서asynchronous I/O event가발생하였을경우(terminate/ignore)
  29. SIGPWR: system power fail (terminate/ignore)
  30. SIGSYS: bad argument to system call (terminate+core)
  
  출처: http://jangpd007.tistory.com/90 [참 놀라운 세상]
  ```

- signal의 기본처리

  - 종료(signal에 의한 정상 종료)
  - 코어덤프 후 종료(signal에 의한 비정상 종료) : core file(종료직전의 memory 상태) 생성 후 종료
  - 중지
  - 무시

  ```
  1. SIG_DFL (SIG_PF)0
  2. SIG_ERR (SIG_PF)-1
  3. SIG_IGN (SIG_PF)1
  4. SIG_HOLD (SIG_PF)2
  
  출처: http://jangpd007.tistory.com/90 [참 놀라운 세상]
  ```

- Child process의 종료 상태 확인

  ```c
  #include <sys/wait.h>
  #include <unistd.h>
  #include <stdlib.h>
  
  int main(int argc, char **argv){
  	pid_t pid;
      int status, exit_status;
  
      if((pid = fork()) < 0){
          perror("fork failed");
          exit(1);
      }
  
      if(pid == 0){
          sleep(4);
          exit(5);
      }
  
      if(pid=wait(&status) == -1){
          perror("wait failed");
          exit(2);
      }
  
      if(WIFEXITED(status)){
          exit_status = WEXITSTATUS(status);
          printf("Exit status from %d was %d\n", pid, exit_status);
      }
  
      exit(0);
  }
  ```

  - 시그널을 받고 종료한 경우, WTERMSIG 시그널 번호를 반환함

  ```c
  if(WIFSIGNALED(status)){
      printf("%d\n", WTERMSIG(status));
  }
  ```

- signal 보내기

  - pid : signal을 받게될 process 지정
  - sig : 보낼 signal 지정

  ```c
  #include <sys/types.h>
  #include <signal.h>
  
  int kill(pid_t pid, int sig);
  ```

  - 시그널을 받을 process 또는 process group 지정 가능
    - pid > 0 : 해당 id의 process에게 signal 전달
    - pid = 0 : sender와 같은 process group에 속하는 모든 process에게 signal 전달. sender 자신도 포함
    - pid = -1: uid가 sender의 euid와 같은 모든 process에게 signal 전달. sender 자신도 포함
    - pid < 0 & pid != -1 : process의 group id가 pid의 절대값과 같은 모든 process에게 signal 전달. -7800 :  7800 group에 동시에 시그널 보내겠다
  - 다른 사용자의 process에게 signal을 보내면 -1 return. 나의 process에게만 보낼 수 있고, kernel만 아무 process에 다 보낼 수 있음
  - raise : raise 함수를 호출한 process에게 지정한 sig를 보낸다

  ```c
  #include <signal.h>
  
  int raise(int sig);
  ```

- Signal handling

  - 기본동작 수행. default action. 각 시그널은 기본 동작이 지정되어 있음
  - 지정된 함수 수행. 정의된 action. 특정 함수 만들어서 실행
  - 시그널 무시

- 책에는 signal, sigset 있는데 수업 때 안 함. 바로 sigaction 구조체 사용해서 시그널 처리함

- sigaction

  - sigaction 지정 : signal 수신 시 원하는 행동을 취할 수 있도록 한다
    SIGSTOP(process의 일시중단), SIGKILL(process의 종료)의 경우는 별도의 action을 취할 수 없다

  - 지정 방법

    ```c
    #include <signal.h>
    
    int sigaction(int signo, const struct sigaction *act, struct sigaction *oact);
    
    struct sigaction{
    	void (*sa_handler)(int);
        sigset_t sa_mask;
        int sa_flags;
        void(*sa_sigaction)(int, siginfo_t *, void *)
    }
    ```

  - void(*sa_handler)(int);

    - signo를 수신하면 취할 행동 지정
    - SIG_DFL. default 행동, 즉 종료
    - SIG_IGN. 무시
    - 정의된 함수. signal을 받으면 함수로 제어 이동, 함수 실행 후 signal을 받기 직전의 처리 문장으로 return

  - sigset_t sa_mask;

    - 여기 정의된 signal들은, sa_handler에 의해 지정된 함수가 수행되는 동안 blocking 된다.
    - 당장 처리하지 않고 현재 처리 중인 것 끝내고 처리하겠다. 우선순위가 높은 signal들 먼저 처리

  - int sa_flags

    - SA_RESETHAND : handler로부터 복귀 시 signal action을 SIG_DFL로 재설정
    - SA_SIGINFO : sa_handler 대신 sa_sigaction 사용

- sigaction 사용

  - Ctrl + c 입력 받으면 정의한 함수 실행

  ```c
  #include <signal.h>
  
  int main(int argc, char **argv){
  
          static struct sigaction act;
          void catchint(int);
  
          act.sa_handler = catchint;
          sigaction(SIGINT, &act, NULL);
  
          printf("sleep call1\n");
          sleep(1);
          printf("sleep call2\n");
          sleep(1);
          printf("exiting\n");
          exit(0);
  }
  
  void catchint(int signo){
          printf("CATCHINT : %d\n", signo);
          psignal(signo, "[Received Signal]");
  }
  ```

  - Ctrl + c 입력 받으면 무시

  ```c
  act.sa_handler = SIG_IGN;
  sigaction(SIGINT, &act, NULL);
  ```

  - Ctrl + c 입력 받으면 종료

  ```c
  act.sa_handler = SIG_DFL;
  sigaction(SIGINT, &act, NULL);
  ```

  - 여러개의 시그널 무시하는 경우
    - SIGINT: interrupt key(^C)를입력하였을때(terminate)
    - SIGQUIT: quit key(^\)를입력하였을때(terminate+core)

  ```c
  act.sa_handler = SIG_IGN;
  sigaction(SIGINT, &act, NULL);
  sigaction(SIGQUIT, &act, NULL);
  ```

- 한 프로세스에서 무시되는 signal은 exec()후에도 계속 무시된다.
  exec으로 실행하면 process가 동일하기 때문에 signal을 계속 무시하게 된다

- Signal  집합 지정

  - sigemptyset => sigaddset : 전부 0으로 설정한 후 block할 시그널 설정
  - sifgillset => sigdelset : 전부 다 1로 설정한 후 block하지 않을 시그널 설정

  - 사용방법

    ```c
    #include <signal.h>
    
    int sigemptyset(sigset_t *set);
    int sigfillset(sigset_t *set);
    
    int sigaddset(sigset_t *set, int signo);
    int sigdelset(sigset_t *set, int signo);
    
    //0으로 설정됐는지 1로 됐는지 확인
    int sigismember(sigset_t *set, int signo);
    ```

    ```c
    sigset_t mask1, mask2;
    
    sigemptyset(&mask1);
    sigaddset(&mask1, SIGINT);
    sigaddset(&mask1, SIGQUIT);
    
    sigfillset(&mask2);
    sigdelset(&mask2, SIGCHLD);
    
    if(sigismember(&mask1, SIGINT))
        printf("SIGINT is setting.\n");
    ```

- 
