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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define BUFSIZE 512

/*
###중간고사
LAB09-03 : 기본 포인터 관련
LAB09-05 : 파일 open 관련
LAB09-10 : lseek, mycopy(읽어온만큼 쓰기)
LAB09-12 : stat, 파일인지 디렉터리인지, link, symlink, ls기능, 표준입력0출력1오류2
LAB09-17 : chdir, mkdir, DIR 정보, ls/cd기능, 권한에 맞는 파일인지 구분
LAB10-01 : getpid() / getppid() / getpgrp() / getpgid(0) / setpgid(pid, gid) / getsid(pid) / setsid(), fork wait기본
LAB10-08 : execl, execlp, execv, execvp, wait, waitpid

###기말고사
LAB10-29 : [signal] [ex]scanf시 interrupt signal 무시했다가 다시 복귀 예제 코드, [p8-1.c]parent에서 child signal, [p8-2.c]child끼리 signal줘서 생성 역순으로 자기 id 출력, [p8-3.c]10초동안 입력 없을 시 알람 시그널로 알려줌
LAB11-05 : [memory mapping] 아직 동기화 작업은 없음. [p9-1.c]10개 정수 메모리매핑한 공간에 쓰고 읽기 AB프로그램, [p9-2.c]문자열 메모리매핑한 공간에 쓰고 읽기 AB프로그램, [p9-3.c]메모리매핑 사용하여 생성 순서 역순으로 동기화
LAB11-07 : [pipe] [ex]select 예제, [p10-1.c]parent는 12개 정수 입력받아서 세 child에게 순서대로 나눠주는 프로그램, [p10-2.c]파이프 사용하여 생성순서 역순으로 동기화 child끼리 동기화 작업
LAB11-12 : [fifo] [p11-1.c]fifo통신 +8해주는 AB프로그램, [p11-2.c]fifo 사용한 select clientserver문제, [p11-3.c]main argument에 맞게 네 개 프로세스 fifo 사용하여 동기화
LAB11-14 : [message queue] [ex]message send/receive 예제, [p12-1.c]message queue 사용하여 +8해주는 serverclient문제, [p12-2.c]message queue 사용하여 네 개 프로세스 알아서 동기화
LAB11-19 : [semaphore] [p13-1.c]메모리매핑 문자열 읽기쓰기 semaphore 사용 동기화, [p13-2.c]네 개 프로세스 semaphore 3개 사용 동기화, [p13-3.c]네 개 프로세스 sempahore 1개 사용 동기화, [p13-4a.c], [p13-4b.c]
LAB11-21 : [shared memory] [p14-1.c]shared memory에 정수 10개 읽기쓰기 semaphore 사용 동기화 AB 프로그램, [p14-2.c]1번 문제 semaphore 사용 안 하고 풀기, [p14-3.c]shared memory +8 serverclient문제
LAB11-26 : [lock/unlock] [p15-1.c]세 개 child 프로세스가 한 파일의 데이터 접근하여 +10하는 프로그램 lock으로 동기화해주기, [p15-2.c]네 개 프로세스 순서 안 정하고 먼저 lock한 순서에 맞게 출력 프로그램

이전 내용은 midterm_main.c 에 있음.

###Child process의 종료 상태 확인
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
###시그널을 받고 종료한 경우, WTERMSIG 시그널 번호를 반환함
if(WIFSIGNALED(status)){
    printf("%d\n", WTERMSIG(status));
}

###시그널 보내기
int kill(pid_t pid, int sig);
- 시그널을 받을 process 또는 process group 지정 가능
  - pid > 0 : 해당 id의 process에게 signal 전달
  - pid = 0 : sender와 같은 process group에 속하는 모든 process에게 signal 전달. sender 자신도 포함
  - pid = -1: uid가 sender의 euid와 같은 모든 process에게 signal 전달. sender 자신도 포함
  - pid < 0 & pid != -1 : process의 group id가 pid의 절대값과 같은 모든 process에게 signal 전달. -7800 :  7800 group에 동시에 시그널 보내겠다
- 다른 사용자의 process에게 signal을 보내면 -1 return. 나의 process에게만 보낼 수 있고, kernel만 아무 process에 다 보낼 수 있음
int raise(int sig);
- raise : raise 함수를 호출한 process에게 지정한 sig를 보낸다

###sigaction
int sigaction(int signo, const struct sigaction *act, struct sigaction *oact);

struct sigaction{
    void (*sa_handler)(int);
    sigset_t sa_mask;
    int sa_flags;
    void(*sa_sigaction)(int, siginfo_t *, void *)
}
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

###Ctrl + c 입력 받으면 정의한 함수 실행
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

###Ctrl + c 입력 받으면 무시
act.sa_handler = SIG_IGN;
sigaction(SIGINT, &act, NULL);

###Ctrl + c 입력 받으면 종료
act.sa_handler = SIG_DFL;
sigaction(SIGINT, &act, NULL);

###여러개의 시그널 무시하는 경우
act.sa_handler = SIG_IGN;
sigaction(SIGINT, &act, NULL);
sigaction(SIGQUIT, &act, NULL);

###한 프로세스에서 무시되는 signal은 exec()후에도 계속 무시된다.
exec으로 실행하면 process가 동일하기 때문에 signal을 계속 무시하게 된다

###Signal 집합 지정
sigset_t mask1, mask2;

sigemptyset(&mask1);
sigaddset(&mask1, SIGINT);
sigaddset(&mask1, SIGQUIT);

sigfillset(&mask2);
sigdelset(&mask2, SIGCHLD);

if(sigismember(&mask1, SIGINT))
    printf("SIGINT is setting.\n");

###Signal 집합 지정 중 sa_sigaction()에 의한 signal handling
int main(void){
    static struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handler;
    sigaction(SIGUSR1, &act, NULL);
    ...
}
void handler(int signo, siginfo_t *sf, ucontext_t *uc){
    psiginfo(sf, "...");
    printf("%d\n", sf->si_code);
}

###이전의 설정 복원
static struct sigaction act, oact;
SIGTERM : 소프트웨어 종료 시그널
SIGTERM 을 위한 과거의 행동을 남겨둔다
sigaction(SIGTERM, NULL, &oact);
SIGTERM 을 위한 새로운 행동 지정.
act.sa_handler = SIG_IGN;
sigaction(SIGTERM, &act, NULL);
작업...
과거 행동 복원
sigaction(SIGTERM, &oact, NULL);

###알람
unsigned int alarm(unsigned int secs);
- alarm은 exec 후에도 계속 작동한다. 프로그램 내용은 변해도 프로세는 같기 때문에
- 하지만 fork 후에는 자식 process에 대한 alarm은 작동하지 않는다. fork는 alarm을 상속하지 않음
- 주의 : alarm(0) 은 alarm 끄기. 바로 알람 보낸다는거 아님
- alarm은 누적되지 않음. 2번 사용되면, 두 번째 alarm이 대체
- 두 번째 alarm의 return 값이 첫 alarm의 잔여시간

###signal blocking
int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
void catchint(int);
int main(int argc, char **argv){
    int i, j, num[10], sum = 0;
    static struct sigaction act;
    sigset_t mask;

    act.sa_handler = catchint;
    sigaction(SIGINT, &act, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    for(i=0;i<5;i++){
        sigprocmask(SIG_SETMASK, &mask, NULL);
        scanf("%d", &num[i]);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        sum += num[i];
        for(j=0;j<=i;j++){
            printf("...%d\n", num[i]);
            sleep(1);
        }
    }
    exit(0);
}
void catchint(int signo){
    printf("DO NOT INTERRUPT ... \n");
}

###pause
int pause(void);
- signal 도착까지 실행을 일시 중단(CPU 사용 없이)
- signal이 포착되면 처리 routine 수행 & -1 return
*/

/*
 * ###시그널 종류
 * 1. SIGHUP: 연결된terminal이hangup하였을때(terminate)
 * 2. SIGINT: interrupt key(^C)를입력하였을때(terminate)
 * 3. SIGQUIT: quit key(^\)를입력하였을때(terminate+core)
 * 4. SIGILL: illegal instruction을수행하였을때(terminate+core)
 * 5. SIGTRAP: implementation defined hardware fault (terminate+core)
 * 6. SIGABRT: abort시스템호출을불렀을때(terminate+core)
 * 7. SIGBUS: implementation defined hardware fault (terminate+core)
 * 8. SIGFPE: arithmetic exception, /0, floating-point overflow (terminate+core)
 * 9. SIGKILL: process를kill하기위핚signal, catch 혹은ignore될수없는signal임(terminate)
 * 10. SIGUSR1: user defined signal 1 (terminate)
 * 11. SIGSEGV: invalid memory reference (terminate+core)
 * 12. SIGUSR2: user defined signal 2 (terminate)
 * 13. SIGPIPE: reader가terminate된pipe에write핚경우발생(terminate)
 * 14. SIGALRM: alarm시스템호출후timer가expire된경우(terminate)
 * 15. SIGTERM: kill시스템호출이보내는software termination signal (terminate)
 * 16. SIGCHLD: child가stop or exit되었을때parent에게전달되는신호(ignore)
 * 17. SIGCONT: continue a stopped process (continue/ignore)
 * 18. SIGSTOP: sendable stop signal, cannot be caught or ignored (stop process)
 * 19. SIGTSTP: stop key(^Z)를입력하였을때(stop process)
 * 20. SIGTTIN: background process가control tty로부터read핛경우(stop process)
 * 21. SIGTTOU: background process가control tty로write핛경우(stop process)
 * 22. SIGURG: urgent condition on IO, socket의OOB data (ignore)
 * 23. SIGXCPU: exceeded CPU time limit (terminate+core/ignore)
 * 24. SIGXFSZ: exceeded file size limit (terminate+core/ignore)
 * 25. SIGVTALRM: virtual time alarm, setitimer, (terminate)
 * 26. SIGPROF: profiling time alarm, setitimer, (terminate)
 * 27. SIGWINCH: terminal window size changed, (ignore)
 * 28. SIGIO: 어떤fd에서asynchronous I/O event가발생하였을경우(terminate/ignore)
 * 29. SIGPWR: system power fail (terminate/ignore)
 * 30. SIGSYS: bad argument to system call (terminate+core)
 *  */

void cat_m(char **s){
    char buffer[BUFSIZE];
    int nread, fd = open(s[1], O_RDONLY);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
    }

    close(fd);
}

void cd_m(char **s){
    chdir(s[1]);
}

void cp_m(char **s){
    char buffer[BUFSIZE];
    int fd1, fd2, nread;

    fd1 = open(s[1], O_RDONLY);

    if (fd1 == -1) {
        return;
    } else {
        fd2 = open(s[2], O_WRONLY | O_CREAT | O_EXCL, 0777);

        if(fd2 == -1){
            fd2 = open(s[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        }

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
        }
    }

    close(fd1);
    close(fd2);
}

void mkdir_m(char **s){
    mkdir(s[1], 0777);
}

void ls_m(char **s){
    DIR *dp;
    struct dirent *d;
    dp = opendir(".");
    while((d = readdir(dp)) != NULL){
        if(d->d_name[0] != '.'){
            printf("%s\n", d->d_name);
        }
    }
}

void vi_m(char **s){
    char buffer[BUFSIZE];
    int nread, fd = open(s[1], O_RDWR|O_CREAT, 0777);

    while((nread = read(fd, buffer, BUFSIZE)) > 0){
        write(1, buffer, nread);
    }
    while((nread = read(0, buffer, BUFSIZE)) > 0){
        if((strncmp(buffer, "quit", 4) == 0) && (buffer[4] == '\n'))
            break;
        write(fd, buffer, nread);
    }
}

char b_dir[BUFSIZE] = "./";

int backup(const char *name, const struct stat *status, int type){
    char buffer[BUFSIZE], file1[BUFSIZE], file2[BUFSIZE];
    int fd1, fd2, i, nread;

    strcpy(file2, b_dir);

    if(type == FTW_F){
//        if(strncmp((char*)name, file2, strlen(file2)) == 0)
//            return 0;

        fd1 = open(name, O_RDONLY);
        strcpy(file1, name + 2);

        for(i=0;i<strlen(file1);i++){
            if(file1[i] == '/')
                file1[i] = '_';
        }

        strcat(file2, file1);
        fd2 = open(file2, O_WRONLY | O_CREAT, status->st_mode&0777);

        while((nread = read(fd1, buffer, BUFSIZE)) > 0){
            write(fd2, buffer, nread);
        }
    }
    return 0;
}

void backup_m(char **s){
    strcat(b_dir, s[1]);
    strcat(b_dir, "/");
    mkdir(b_dir, 0333);
    ftw(".", backup, 1);
}

void exec_m(char **s){
    execv(s[0], s);
}

int flag;

int delete_all(const char *name, const struct stat *status, int type){
    int cnt = 0;
    DIR *dp;
    struct dirent *d;

    if(type == FTW_F){
        remove(name);
    }
    else if(type == FTW_D){
        dp = opendir(name);
        while(d = readdir(dp))
            cnt++;
        if(cnt <= 2)
            rmdir(name);
        else
            flag = 1;
    }
    return 0;
}

void rmall_m(char **s){
    do{
        flag = 0;
        ftw(s[1], delete_all, 1);
    }while(flag == 1);
}

int main(int argc, char **argv){
    char in[50], *res[20] = {0};
    char *inst[8] = {"cat", "cd", "cp", "mkdir", "ls", "vi", "backup", "rmall"};
    void (*f[9])(char **) = {cat_m, cd_m, cp_m, mkdir_m, ls_m, vi_m, backup_m, rmall_m, exec_m};
    int i, background = 0;
    pid_t pid, b_pid;

    while(1){
        printf("> ");
        gets(in);
        i = 0;
        res[i] = strtok(in, " ");
        if(strcmp(res[0], "exit")==0)
            exit(0);

        while(res[i]){
            i++;
            res[i] = strtok(NULL, " ");
        }

        for(i=0;i<8;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }

        if(i == 1){
            cd_m(res);
        }
        else{
            if (i == 6){
                background++;
                b_pid = fork();
                if (b_pid == 0){
                    f[i](res);
                    exit(0);

                }
            }
            else{
                pid = fork();
                if (pid == 0){
                    f[i](res);
                    exit(0);
                }
                else{
                    waitpid(pid, 0, 0);
                }
            }
        }
        if (background){
            if(waitpid(b_pid, 0, WNOHANG)){
                background = 0;
            }
        }
    }
}
