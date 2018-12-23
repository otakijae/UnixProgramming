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

#define BUFSIZE 512

/*
 * LAB09-03 : 기본 포인터 관련
 * LAB09-05 : 파일 open 관련
 * LAB09-10 : lseek, mycopy(읽어온만큼 쓰기)
 * LAB09-12 : stat, 파일인지 디렉터리인지, link, symlink, ls기능, 표준입력0출력1오류2
 * LAB09-17 : chdir, mkdir, DIR 정보, ls/cd기능, 권한에 맞는 파일인지 구분
 * LAB10-01 : getpid() / getppid() / getpgrp() / getpgid(0) / setpgid(pid, gid) / getsid(pid) / setsid(), fork wait기본
 * LAB10-08 : execl, execlp, execv, execvp, wait, waitpid
 * QUIZ10-10 : 첫 퀴즈
 * QUIZ10-15 : fork 사용하여 process 실행
 * QUIZ10-17 : backup 함수
 *
 * read return값
 * - 성공시 실제읽힌 byte 수 / 실패시 -1
 *   - return 값 < nbytes이면, 파일에 남은 data가 nbytes보다 적은 경우
 *   - 더 이상 읽을 내용이 없으면 return 값은 0
 *   - 혹시나 -1이 반환이 되는 경우 read에 실패한건데, 없는 파일을 열거나 open 잘못했을 때 -1을 return 하니까 어디서 -1을 return 하는지 잘 확인해보기
 *   - 이걸로 파일에서 더 읽을게 있는지 없는지 파악하면 됨
 *
 *   write return 값
 *   - 보통은 return값 = n
 *   - return 값 < n이면, 쓰는 도중 media가 full
 *   - 만약 쓰기 전에 꽉 차면 -1 return
 *
 *   0764 = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IROTH
 *
 *   04000 S_IUID : 실행이 시작되면 소유자의 uid가 euid가 된다
 *   02000 S_ISGID : 실행이 시작되면 소유자의 gid가 egid가 된다
 *
 *   int chmod(const char *pathname, mode_t mode);
 *   int fchmod(int fd, mode_t mode);
 *   int readlink (const char *sympath, char *buffer, size_t buffsize); sympath로 originalfile 가져와서 buffer에 저장
 *   int lstat (const char *linkname, struct stat *buf); symfile 자체 정보 얻을 때
 *   int mkdir(const char *pathname, mode_t mode); mkdir("A", 0733);
 *   int rmdir(const char *pathname); int rename (const char *oldpathname, const char *newpathname);
 *   char *getcwd(char *name, size_t size); getcwd(ch, 99);
 *   chdir(“/usr/ben”);
 *   void rewinddir(DIR *dirptr);
 *
 *   type = FTW_Fobject가 파일임
 *   type = FTW_Dobject가 directory임
 *   type = FTW_DNRobject는 읽어올 수 없는 directory. permission 4가 없는 경우
 *   type = FTW_NSex) stat관련된 정보를 얻어올 수 없는 symlink 타입. symlink인데 가리키는 파일 이름이 변경된 경우
 *
 *   if(access("data1", R_OK | W_OK) == 0) F_OK 파일 존재하는지 읽기 쓰기 가능한지 파악
 *
 *   if(S_ISREG(buffer.st_mode)) 파일인지 디렉토리인지 파악
 *
 *   pid_t getpid(void); 지금 실행 중인 process ID
 *   pid_t getppid(void); 지금 실행 중인 process의 parent ID
 *   pid_t getpgrp(void); 나의 group id 알아보기
 *   pid_t getpgid(pid_t pid);
 *   int setpgid(pid_t pid, pid_t pgid); setpgid(getpid(), getpid()); 지금부터 새로운 group leader가 되겠다는 명령
 *   pid_t getsid(pid_t pid); session id를 획득
 *
 *   */

//struct stat {
////    dev_t     st_dev;         /* 장치파일의 위치 및 여부 device id */
////    ino_t     st_ino;         /* 파일의 inode 번호 */
////    mode_t    st_mode;        /* file permission 정보 */
////    nlink_t   st_nlink;       /* 하드링크의 갯수 */
////    uid_t     st_uid;         /* user id */
////    gid_t     st_gid;         /* group id */
////    dev_t     st_rdev;        /* 장치파일(inode)를 기술 */
////    off_t     st_size;        /* 해당 파일의 총 크기*/
////    blksize_t st_blksize;     /* 효율적인 I/O 파일 시스템 위한 블럭 사이즈*/
////    blkcnt_t  st_blocks;      /* 할당된 블럭 사이즈 */
////};



/*
 *     char s1[100] = "A Garden Diary A Garden Diary A Garden Diary";
 *     char *ptr = strstr(s1, "den");
 *
 *     while (ptr != NULL){
 *         printf("%s\n", ptr);
 *         ptr = strstr(ptr + 1, "den");
 *     }
 *
 *     den Diary A Garden Diary A Garden Diary
 *     den Diary A Garden Diary
 *     den Diary
 */

 /*
 *     char s2[30] = "A Garden Diary";
 *     char *ptr2 = strchr(s2, 'a');
 *
 *     while (ptr2 != NULL){
 *         printf("%s\n", ptr2);
 *         ptr2 = strchr(ptr2 + 1, 'a');
 *     }
 *
 *     arden Diary
 *     ary
 */

/*
    //인자로 입력받은 프로세스 실행
    char inst[100] = {"./"}, *arg[100] = {0};
    int i, status;
    pid_t pid;
    //for(i=0;argv[i+1]!=NULL;i++)
    //    arg[i] = argv[i+1];
    strcat(inst, argv[1]);
    pid = fork();
    if(pid == 0)
        execv(inst, argv+1); //execv(inst, arg);
    wait(&status);
*/

int del_dir(const char *name, const struct stat *status, int type){

    int k;
    struct dirent *d;
    DIR *dp;

    if(type == FTW_D){
        k = 0;
        dp = opendir(name);
        while((d = readdir(dp)) != NULL)
            k++;
        if(k<=2)
            rmdir(name);
    }

    return 0;
}

void A(char **argv){

}

void B(char **argv){

}

void C(char **argv){

}

void D(char **argv){

}

void E(char **argv){

}

void F(char **argv){

}

void G(char **argv){
	int i;
    	for(i = 0;argv[i]!=NULL;i++){
        	printf("%s ", argv[i]);
    	}
}

int main(int argc, char **argv){

    char in[50], *res[20] = {0};
    char *inst[6] = {"A", "B", "C", "D", "E", "F"};
    void (*f[7])(char **) = {A,B,C,D,E,F,G};
    int i;

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

        for(i=0;i<6;i++){
            if(!strcmp(res[0], inst[i]))
                break;
        }
        f[i](res);
    }
}

