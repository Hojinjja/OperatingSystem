#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> //리눅스에 있는 헤더파일

int main(int argc, char **argv)
{
    int pipe_fds[2];
    pid_t pid; // 시스템 콜에 의해 생성된 pid를 저장하기 위한 변수 
    char buf[1024];
    int wstatus;

    printf("[%d] start of function\n",getpid());
    memset(buf,0,sizeof(buf));

    if(pipe(pipe_fds)){
        perror("pipe()");
        return -1;
    }

    pid=fork();
    if(pid==0){
        /*child process*/
        close(pipe_fds[1]);
        read(pipe_fds[0],buf,sizeof(buf));
        printf("[%d] parent said... %s\n",getpid(),buf);
        close(pipe_fds[0]);

    }
    else if(pid>0){
        /*parent process*/
        close(pipe_fds[0]);
        strncpy(buf,"hello child-",sizeof(buf)-1);
        write(pipe_fds[1],buf,strlen(buf));
        close(pipe_fds[1]);

        pid=wait(&wstatus);
        //wait()는 1개 이상의 자식 프로세스의 종료를 부모 프로세스가 기다리게 하는 것
        //이때 wait()은 자식 프로세스의 pid와 자식 프로세스의 종료 상태를 뱉는데
        //pid에는 종료된 자식 프로세스의 pid를 , wstatus에는 자식 프로세스의 종료 상태를 집어넣는다. 
    }else{
        /*error case*/
        perror("fork()");
        goto err;
    }
    return 0;

err:
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    return -1;

}

//gcc pipe_practice.c
//./a.out

//fork()를 사용하면 자식 프로세스에서 한 번, 부모 프로세스에서 한 번 반환된다. 
//자식 프로세스에는 -> 항상 0을 반환한다. (생성 성공시에만 0을 반환)
//부모 프로세스에는 -> 자식 프로세스 반환 실패시 -1 반환 , 성공시에는 자식 프로세스의 pid 반환. 