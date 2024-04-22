#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

void
sigchld_handler(int sig)
{
    pid_t child; // 자식 프로세스 pid 저장
    int exitcode; // exitcode 저장
    child = wait(&exitcode); //wait()함수로 child(자식 프로세스)가 종료되길 기다리고, 종료 정보를 exitcode에 저장
                            // child에는 wait()함수의 결과로 종료된 자식 프로세스의 pid가 들어간다.
    printf("> child process %d is terminated with exit code %d\n",
            child, WEXITSTATUS(exitcode));
            //WEXITSTATUS(exitcode)는 exitcode에서 실제 종료 코드를 호출.

}

int 
main()
{
    signal(SIGCHLD, sigchld_handler); //SIGCHLD 시그널 발생시 ,sigchld_handler호출

    pid_t child; // 자식 프로세스 pid 저장
    if (child = fork()){ //fork()를 호출하여 현재 프로세스 복제, 반환값 0= 자식프로세스 / 0이 아니면 부모 프로세스
        //부모 프로세스가 작동하는 코드
        sleep(3); //3초간 슬립
        kill(child, SIGTERM); // kill()함수를 사용하여 자식 프로세스에게 SIGTERM 시그널을 보내어 종료 요청
        while(1); // kill으로 자식 프로세스는 없앴지만, 부모 프로세스는 여기에서 무한 루프에 걸림.
                    //때문에 프로그램이 종료가 안된다.
    }
    else{
        //자식 프로세스가 작동하는 코드.
        while(1);
    }

}