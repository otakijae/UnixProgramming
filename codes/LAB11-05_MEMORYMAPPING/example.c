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

#define BUFSIZE 512

  int main(int argc, char **argv){
      int i, in, pid, p[2][2];
      
      for(i=0;i<2;i++){
          pipe(p[i]);
      }
      pid = fork();
      if(pid == 0){
          close(p[0][1]);
          close(p[1][0]);
          read(p[0][0], &in, sizeof(int));
          in++;
          write(p[1][1], &in, sizeof(int));
          exit(0);
      }
      close(p[0][0]);
      close(p[1][1]);
      scanf("%d", &in);
      write(p[0][1], &in, sizeof(int));
      read(p[1][0], &in, sizeof(int));
      printf("%d\n", in);
      wait(0);
      exit(0);
  }

/*
int main(int argc, char **argv){

        int i, fd;
        char *addr;

	int pagesize;

	pagesize = sysconf(_SC_PAGESIZE);

	printf("pagesize : %d", pagesize);

        fd = open("data", O_RDWR);
        addr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
//        addr = mmap(NULL, 10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 4096);

        for(i=0;i<5;i++){
                printf("%c : ", *(addr + i));
                *(addr + i) = *(addr + i) + 1;
                printf("%c\n", *(addr + i));
        }

        printf("%s\n", addr);

        exit(0);
}*/
