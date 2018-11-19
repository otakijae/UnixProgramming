# Chapter10 시스템 V의 프로세스간 통신

- 앞서 사용한 Read/write는 동기화가 되지 않음. 예를 들어 정수, 실수, 구조체는 크기가 딱 정해져있어서 구분해서 데이터를 받아오는데 상관이 없지만, 문자열 같은 경우 구분이 안 되니 한 번에 읽어올 수밖에 없음. 단방향 채널 통신이라 누가 읽어가는지 지정 해주지 못하면 데이터가 얽히게 된다.

- 하지만 IPC에서 message 나눠서 보내면 나눠서 받을 수 있음. 양방향 채널 통신 가능함. 메세지큐는 어느 메세지를 누가 가져가는지 메세지 id로 구분 가능하기 때문.

- IPC(Inter Processor Communication) 설비

  - 이건 파일 시스템이 아니고 한 군데에서 관리를 하기 때문에 어느 디렉토리든지 상관없이 이름이 곂치면 안 됨. 그래서 unique한 

  - key

    - message_queue, semaphore, shared memory segment에 대한 identifier(파일 이름에 해당)
    - 서로 다른 process들도 동일 IPC 객체는 같은 key값으로 접근
    - 시스템에서 unique한 key값을 사용하여야 함. ==> 숫자를 사용하여 메세지큐 이름으로 이용. 곂치면 가장 빨리 만든 사람의 소유로 되기 때문에 얽히니까 key값 생성 시스템 콜을 사용함.

  - key값 생성

    ```c
    #include <sys/ipc.h>
    
    key_t ftok(const char *path, int id);
    ```

    - 해당 파일의 st_dev, st_ino와 id로 key값 생성

- IPC 객체 상태 구조

  ```
  struct ipc_perm{
      uid_t cuid : 생성자의 uid;
      gid_t cgid : 생성자의 gid;
      uid_t uid : 소유자의 uid;
      gid_t gid : 소유자의 gid;
      mode_t mode : permission(execution은 의미 없음)
  }
  ```

  - permission
    - 읽기 가능 : 큐 통해서 메세지 받을 수 있음
    - 쓰기 가능 : 큐 이용해서 메세지 보낼 수 있음
    - 실행 가능 : (읽기/쓰기만 할거라 실행은 안 씀. 의미 없음)

- IPC 정보 검색 및 삭제

  - ipc 관련 정보 검색 ```$ ipcs```

    ```
    ------ Message Queues --------
    key        msqid      owner      perms      used-bytes   messages    
    0x004007e0 1325301760 s122130    644        160          2           
    0x00f44f3c 1325957121 s16011068  644        0            0           
    0x00d5cb51 884738     s14011217  644        320          4           
    0x017b0674 1325596675 s13011093  644        80           1           
    0x00d5ca98 1325367300 s14011032  644        0            0           
    0x00d5cac8 1933317    s14011080  644        0            0           
    0x0001dcc9 557062     s122057    644        0            0           
    0x0001dd47 1325105159 s122183    644        720          9                     
    
    ------ Shared Memory Segments --------
    key        shmid      owner      perms      bytes      nattch     status      
    0x6c7f146d 0          root       600        3339312    3                       
    0x01130197 360449     root       600        1000       9                       
    0x00000000 131074     oracle     600        30040064   548                     
    0x00000000 163843     oracle     600        40399536128 274                     
    0x00000000 196612     oracle     600        104177664  274                     
    0x382b361c 229381     oracle     600        24576      274                     
    
    ------ Semaphore Arrays --------
    key        semid      owner      perms      nsems     
    0x7a7f146d 0          root       600        13        
    0x00000000 1966081    apache     600        1         
    0x00000000 1998850    apache     600        1         
    0x00000000 98307      apache     600        1         
    0x00000000 2031620    apache     600        1         
    0x00000000 2064389    apache     600        1         
    0x00000000 2097158    apache     600        1         
    0xd680bf9c 393223     oracle     600        140       
    ```

  - ipc 삭제 ```$ipcrm -m shmid or -q msqid or -s semid```

- Message passing

  - message queue를 통한 message 전달
    - msgget : queue 생성
    - msgsnd : message 보내기
    - msgrcv : message 받기

- msgget 시스템 호출

  - 사용법. 파일 open 기능과 유사

    ```c
    #include <sys/msg.h>
    
    int msgget(key_t key, int permflags);
    ```

  - key : message queue의 key 값

  - permflags = queue에 대한 access permission

    - IPC_CREAT
      - 해당 queue가 없으면, 생성한 후 return; 있으면 return;
      - 이 flag가 설정 되지 않은 경우에는, queue가 존재하는 경우에만 return
    - IPC_EXCL
      - 해당 queue가 존재하지 않는 경우만 성공, 아니면 -1 return
      - IPC_CREAT와 같이 사용해야함

  - return 값 : 음수가 아닌 queue identifier. 이걸 잘 가지고 있어야 메세지를 보내고 받을 수 있음

- msgsnd 시스템 호출

  - 사용법

    ```c
    #include <sys/msg.h>
    
    int msgsnd(int mqid, const void *message, size_t size, int flags);
    ```

  - mqid : message queue identifier

  - message의 주소 : 보낼 message가 저장된 주소. cont void로 타입 지정된건 정수이든 문자열이든 구조체 등등 이든 타입 상관이 없기 때문.

  - size : long mtype 제외한 message의 크기

  - flags

    - 필요한 flag 없으면 0으로 설정
    - IPC_NOWAIT
      - send가 불가능하면 즉시 return(queue가 가득 찬 경우). queue가 가득 차면 blocking되고 send가 불가한데, 이때 block하지 않고 -1을 바로 return하겠다하는 경우 사용
      - flag가 설정되지 않으면 (0이면), 성공 시까지 blocking

  - return 값은 0 or -1

  - message의 구조

    - long type의 정수 값을 갖는 mtype과 임의의 message의 내용으로 구성
    - message의 size는 message 내용의 크기만
    - long mtype에서 long 타입은 계속 유지시켜야함. 바꾸면 안 되고, 음수가 들어가면 안 됨. 0보다 큰 양수 아무나 들어가면 된다.

    ```c
    struct mymsg{
        long mtype;				//message type(양의 정수)
        char mtext[SOMEVALUE];	//message 내용
    }
    ```

  - msg queue 생성 및 msg 보내기 예제

    ```c
    struct q_entry{
        long mtype;
        int mnum;
    };
    
    int main(int argc, char **argv){
        int qid, i, in;
        struct q_entry msg;
        key_t key;
    
        key = ftok("keyfile", 1);
        qid = msgget(key, 0600|IPC_CREAT);
    
        for(i=0;i<3;i++){
            scanf("%d", &in);
            msg.mtype = i+1;
            msg.mnum = in;
            msgsnd(qid, &msg, sizeof(int), 0);
        }
        exit(0);
    }
    ```

    - fifo 같은 경우 프로그램 2개를 동시에 open 순서 잘 맞춰서 실행했어야 했는데, 이렇게 메세지를 보내는 코드만 작성하고 실행해도 문제가 없는 이유는,,,
    - blocking queue가 아니라 그냥 보내놓으면 있으면 받고 아니면 안 받기 때문에 가능함.
    - fifo로 데이터를 보내다가 데이터가 아직 fifo에 있는데 프로그램을 종료하면 fifo 데이터 남아있지 않고 사라짐
    - 근데 message queue는 데이터가 남아있음. 그래서 가능

- msgrcv

  - 사용법

    ```c
    #include <sys/msg.h>
    
    int msgrcv(int mqid, void *message, size_t size, long msg_type, int flags);
    ```

  - mqid : message queue identifier

  - message 주소 : 받은 message를 저장할 저장 장소의 주소

  - size : long mtype 제외한, 준비된 저장 장소의 크기

  - msg_type

    - 0 : queue의 첫 message. 메세지 타입 상관없이 큐 제일 앞 메세지를 순서대로 받겠다.
    - _ > 0 : 해당 값을 갖는 첫 message. 1적으면 맨 앞 1번 message, 100 적으면 맨 앞 100번 message 골라서 읽겠다.
    - _ < 0 : mtype값이 절대값보다 작거나 같은 것 중 최소값을 갖는 첫 message. -5 적으면 5보다 작은 아무 메세지를 받겠다.

  - flags

    - IPC_NOWAIT
      - 메세지 받을 때 기본적으로 blocking인데,,, 메세지 안 와도 안 기다리고 다른 작업을 하겠다 하면 사용. 없으면 -1 바로 return
      - receive가 불가능하면 즉시 return(queue에 해당 msg가 없는 경우)
      - return 값은 -1; errno = EAGAIN
      - flag가 설정되지 않으면 (값이 0이면), 성공 시까지 blocking
    - MSGNOERROR
      - message가 size보다 길면 초과분을 자른다
      - 정수 5개가 올 걸 예상하여 5개 공간만 만들어놓았는데, 정수 10개를 받으면 원칙적으로 실패하면서 -1을 return 하는데, 경우에 따라 뒤에 5개를 포기하고 앞에 5개만 받겠다 이런 경우 flag 설정
      - flag가 설정 되지 않으면 size 초과 시 error

  - return 값

    - receive 성공 시 받은 message의 길이
    - 실패 시 -1
    - access permission 때문에 실패한 경우 error = EACCESS

  - message 송수신 예

    - 예1

    ```c
    struct q_entry{
        long mtype;
        int mnum;
    };
    
    int main(int argc, char **argv){
        int qid, i, in;
        struct q_entry msg;
        key_t key;
    
        key = ftok("keyfile", 1);
        qid = msgget(key, 0600|IPC_CREAT);
    
        for(i=0;i<3;i++){
            msgrcv(qid, &msg, sizeof(int), -3, 0);
            printf("%d\n", msg.mnum);
        }
        exit(0);
    }
    ```

    - 앞에 메세지 큐 생성 및 메세지 보내기 이후, 메시지 수신 예제 이후 메세지 큐 어떻게 되는지 확인한 결과.
      10,11,12 세 개의 숫자를 입력받아서 12바이트에 3개의 QNUM이 있는 것 확인
      -3으로 msg_type 설정했기 때문에 1,2,3 id의 숫자 아무거나 세 번 수신하는 코드라 3개의 메세지를 다 읽어왔고, 비어있는 메세지 큐 삭제한 과정

    ```
    sinjaeheoguiMBP:unix jaehyukshin$ ./example.out
    10
    11
    12
    sinjaeheoguiMBP:unix jaehyukshin$ ipcs -qo
    IPC status from <running system> as of Sat Nov 17 13:21:50 KST 2018
    T     ID     KEY        MODE       OWNER    GROUP CBYTES  QNUM
    Message Queues:
    q 131072 0xffffffff --rw------- jaehyukshin    staff     12      3
    
    sinjaeheoguiMBP:unix jaehyukshin$ ./example2.out
    10
    11
    12
    sinjaeheoguiMBP:unix jaehyukshin$ ipcs -qo
    IPC status from <running system> as of Sat Nov 17 13:22:02 KST 2018
    T     ID     KEY        MODE       OWNER    GROUP CBYTES  QNUM
    Message Queues:
    q 131072 0xffffffff --rw------- jaehyukshin    staff      0      0
    
    sinjaeheoguiMBP:unix jaehyukshin$ ipcrm -q 131072
    sinjaeheoguiMBP:unix jaehyukshin$ ipcs -qo
    IPC status from <running system> as of Sat Nov 17 13:22:18 KST 2018
    T     ID     KEY        MODE       OWNER    GROUP CBYTES  QNUM
    Message Queues:
    
    ```

    - 예2
      메세지 큐에 있는 1,2,3 id의 데이터를 받은 후에 받은 값에 +8하여 id 4로 다시 메시지를 보냄. 그리고 다시 메시지 큐에 있는 id 4의 데이터를 가져와서 출력함. flag에 0 말고 -1을 넣으니까 IPC_NOWAIT이랑 비슷한 것 같음. 0을 넣으면 메시지가 들어올 때까지 계속 기다림.

    ```c
    struct q_entry{
        long mtype;
        int mnum;
    };
    
    int main(int argc, char **argv){
        int qid, i, in;
        struct q_entry msg;
        key_t key;
    
        key = ftok("keyfile", 1);
        qid = msgget(key, 0600|IPC_CREAT);
    
        if(qid == -1){
            perror("msgget");
            exit(0);
        }
    
        while(msgrcv(qid, &msg, sizeof(int), -3, -1) > 0){
            msg.mtype = 4;
            msg.mnum = msg.mnum + 8;
            printf("%d\n", msg.mnum);
            msgsnd(qid, &msg, sizeof(int), 0);
        }
    
        for(i=0;i<3;i++){
            msgrcv(qid, &msg, sizeof(int), 4, 0);
            printf("%dth this is it : %d\n", i, msg.mnum);
        }
        exit(0);
    }
    
    ```


## 20181119

- msgctl

  - Fifo vs msg queue 가장 큰 차이

    - fifo는 전부 종료 시 사라짐
    - msg queue는 전부 종료 시 msg 그대로 남아있음

  - message queue 사용하면 문자열 같은 경우도 얼마나 나눠서 보냈는지 알 수 있음

  - Message queue에 대한 정보 획득. msgctl은 메시지 큐와 관련된 정보를 얻기 위해 사용

  - Message queue 제거 기능. message queue 같은 경우 만들 수 있는 갯수가 제한돼서 사용하고 지워야함. 일일이 지우기도 하는데 msgctl로 지워줌

    ```c
    #include <sys/msg.h>
    
    int msgctl(int mqid, int command, struct msqid_ds *msq_stat);
    ```

  - mqid : message queue identifier

  - command

    - IPC_STAT : msg queue의 상태 정보 확인. stat 정보 저장할 공간을 3번째 인자에 적어줘야함
    - IPC_RMID : msg queue 삭제. 3번째 인자 필요 없기 때문에 0으로 적어주면 된다.

  - msqid_ds의 구조

    ```
    struct ipc_perm msg_perm;	//소유권
    msgqnum_t msg_qnum;			//msg 수
    msglen_t msg_qbytes;		//bytes 수
    pid_t msg_lspid;			//last sender
    pid_t msg_lrpid;			//last receiver
    time_t msg_stime;			//last sending time
    time_t msg_rtime;			//last receipt time
    time_t msg_ctime;			//last s/r time
    ```

- semaphore

  - p(sem) or wait(sem)

    ```
    if(sem > 0){
    	//세마포 값 하나 뺌. 그리고 바로 return. no blocking
       	decrement sem by one;
    }
    else{
    	//세마포 0보다 큰 값이 될 때까지 blocking.
        wait until sem becomes non-zero;
        then decrement;
    }
    //그냥 OS에서는 빼고나서 block했는데, 유닉스는 깨어날 때 바로 빼줌.
    //결론은 같은 작업이나 순서가 다른 이유는, unix에서 세마포는 음수를 사용하지 않기 때문. 그래서 0보다 커질 때까지 기다렸다가 나중에 깨어나면서 빼준다.
    ```

  - v(sem) or signal(sem)

    ```
    //세마포 값을 하나 더함
    increment sem by one;
    
    //큐가 비어있는지 확인. 큐가 안 비어있으면 첫번째 block된걸 다시 실행.
    if(queue of waiting processes not empty)
    	restart first process in wait queue;
    ```

  - 사용 예

    - 세모포 설정할 때 초기값 설정하는게 중요함

    ```
    p(sem);					//wait
    something interesting;	//critical section
    v(sem);					//signal
    ```

- semget 시스템 호출

  - 사용법

    ```c
    #include <sys/sem.h>
    #include <sys/types>
    #include <sys/ipc.h>
    
    int semget(key_t key, int nsems, int permflags);
    ```

  - key : semaphore 집합 이름

  - nsems : semaphore 집합 내의 semaphore 수. 집합으로 필요한만큼 세마포 생성 가능.

  - permflags : 0600, IPC_CREAT, IPC_EXCL … / 메시지 큐 만들 때처럼 사용한 옵션 의미 그대로 사용 가능.

  - return 값 : semaphore 집합 identifier

  - 세마포 집합 배열에 한꺼번에 만들 수 있는데, 독립적으로 수행된다. 그래서 몇 번 index의 세마포가 ~한 역할을 한다고 명시해주면 된다.

    - semid[0] : index0 : semval = 2
    - semid[1] : index1 : semval = 4
    - semid[2] : index2 : semval = 1
    - semid[3] : index3 : semval = 3
    - ==> 이렇게 된 경우, nsems = 4 가 된다

  - 집합 내 각 semaphore와 연관된 값

    - semval : semaphore 값 (semapahore 값의 초기화 필요)
    - sempid : 최근 semaphore를 access 한 process id. 초기화, wait, signal 등으로 가장 최근에 변경한 세마포 값
    - 유닉스 세마포에는 두 개의 큐가 존재. 근데 유닉스에는 음수값을 갖지 않기 때문에 큐에 몇 명이 기다리는지 알 수 없음. 그래서 이걸 사용하여 큐에 몇 명이 기다리는지 알 수 있음
      - semncnt : semaphore 값이 증가하기를 기다리는 process 수. 0일 때 도착해서 기다리는 경우
      - semzcnt : semaphore 값이 0이 되기를 기다리는 process 수. 0이 아닐 때 도착해서 0이 되기를 기다리는 경우

- semctl 시스템 호출

  - 사용법

    ```c
    #include <sys/sem.h>
    
    int semctl(int semid, int sem_num, int command, union semun arg);
    ```

  - semid : semaphore identifier

  - sem_num : 집합 내 특정 semaphore 지정

  - command

    - IPC_STAT : 상태 정보를 arg.stat에 저장. 세마포와 관련된 정보를 저장하기 위해 네 번째 인자에 명시한 곳에 저장
    - IPC_RMID : semaphore 집합 삭제. 삭제만 하는거라 네 번째 인자 값에 0으로 설정

  - command => 단일 semaphore에 영향을 미치는 기능. (semctl return 값으로 반환해줌)

    - GETVAL : semval 값 return
    - SETVAL : semavl 값을 arg.val 값으로 지정
    - GETPID : sempid 값을 return
    - GETNCNT : semncnt 값을 return. 첫 번째 큐에서 기다리는 숫자
    - GETZCNT : semzcnt 값을 return. 두 번째 큐에서 기다리는 숫자

  - command => semaphore 집합 전체에 영향을 미치는 기능. (semctl return 값으로 반환해줌)

    - GETALL : 모든 semval 값을 arg.array에 저장. 여러개의 값 한꺼번에 오니까 return값 사용 안 하고, 저장 공간의 주소를 알려주면 된다.
    - SETALL : arg.array 값으로 모든 semval 값을 지정. 세마포 값이 10개면 10개 값 한꺼번에 다 설정

  - struct semun arg;

    ```c
    union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    };
    ```

    - struct와 union의 차이 : 구조체는 a,b,c 정보 필요하면 다 멤버로 가지지만, union은 a,b,c 정보 필요하면 어쩔때는 a로 쓰이고, 어쩔때는 b로 쓰이고,,, 이렇게 하나를 골라서 사용함.

  - semaphore 하나 만들고 초기값 설정하기

    ```c
    union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
    };
    
    key_t key;
    union semun arg;
    key = ftok("key", 3);
    semid = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    arg.val = 3;
    semctl(semid, 0, SETVAL, arg);
    ```

    - ### 세마포 있으면 아예 만들지 말고, 세마포 없을 때만 만들라는 뜻인데 왜 이 옵션이 필요한지에 대해 잘 생각해보기

  - semaphore 집합으로 만들고 초기값 설정하기

    ```c
    union semun{
        int val;
        struct semid_ds *buf;
        ushort *array;
    };
    
    int i;
    key_t key;
    union semun arg;	//여기 안에는 포인터밖에 없으니 이 값들을 또 별도로 저장할 변수 buf[3] 생성
    ushort buf[3];
    
    semid = semget(key, 3, 0600|IPC_CREAT|IPC_EXCL);
    for(i=0;i<3;i++)
        buf[i] = i + 1;
    arg.array = buf;
    semctl(semid, 0, SETALL, arg);
    ```

- semop 시스템 호출

  - 사용법

    ```c
    #include <sys/sem.h>
    
    int semop(int semid, struct sembuf *op_array, size_t num_ops);
    ```

  - semid : semaphore identifier

  - oparray : 수행할 연산 지정

  - num_ops : op_array 내의 sembuf의 수. (여러개의 semaphore에 대한 연산을 동시에 지정할 수 있음)

  - sembuf의 구조

    ```c
    struct sembuf{
        unsigned short sem_num;
        short sem_op;
        short sem_flg;
    }
    ```

    - sem_num : semaphore index
    - sem_op : 수행할 연산(양의 정수 또는 음의 정수 또는 0). sem_op가 -1이면 wait / +1이면 signal. -2, -3, -5도 사용가능하는데 이 숫자들을 잘 조합하면 다양한 경우의 sem 가능.
    - sem_flg : IPC_NOWAIT or SEM_UNDO
      - 이건 주의 : 사용하지 말 것. 항상 0으로 설정할 것
      - IPC_NOWAIT : sem 기본적으로 blocking을 해야함. block 하기 싫어서 옵션 사용하면 0인지 아닌지 검사만 하고 그냥 지나감
      - SEM_UNDO : 프로그램 시작 후 종료까지 지금까지 뺀 값들을 undo함. -10했으면 마지막을 +10을 해주고 끝남. 제일 마지막에 초기값으로 돌아가고 싶을 때 사용함. 그래서 나중에 원치않게 꼬일 수 있으니까 교수님이 하지말라고 하심

  - sem_op : 수행할 연산

    - 음수 p() or wait() 연산

      ```c
      if(semval >= |sem_op|){
          set semval to semval - |sem_op|;
      }
      else{
          wait until semval reaches or exceeds |sem_op|;
          then set semval to semval - |sem_op|;
      }
      ```

    - 양수 v() or signal() 연산

      ```
      set semval to semval + |sem_op|;
      ```

    - 0

      ```
      semval 값의 변화는 없음
      sem_op는 semval이 0이 될 때까지 waiting(blocking)
      //(애매함)특정 조건이 만족할 때까지 기다렸다가, 한꺼번에 다 풀어줘서 한 번에 실행할 수 있게 설정해주기
      ```

  - 예제. 이렇게 하면 그냥 멈춰버림. 세마포 wait했다가 안 풀어주니까

    ```c
    union semun{
            int val;
            struct semid_ds *buf;
            ushort *array;
    };
    
    int main(int argc, char **argv){
            int i, id, pid, semid;
            key_t key;
            union semun arg;
            ushort buf[3] = {0};
            struct sembuf p_buf;
    
            key = ftok("key", 1);
            semid = semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
    
            p_buf.sem_num = 0;	//0번 세마포
            p_buf.sem_op = -1;	//wait 명령
            p_buf.sem_flg = 0;	//그냥 0으로 설정해주면 된다. 근데 꼭 0으로 설정한거 명시하기
            semop(semid, &p_buf, 1);
    
            printf("process %d in critical section\n", getpid());
            sleep(2);
            printf("process %d leaving critical section\n", getpid());
    
            p_buf.sem_num = 0;	//0번 세마포
            p_buf.sem_op = 1;	//signal 명령
            p_buf.sem_flg = 0;	//그냥 0으로 설정해주면 된다. 근데 꼭 0으로 설정한거 명시하기
            semop(semid, &p_buf, 1);
    
            exit(0);
    }
    ```

- 