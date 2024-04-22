#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void 
handler(int sig)
{
    printf("RING\n");
}

int
main()
{
    struct itimerval t; // 구조체 인스턴스 t 생성

    signal(SIGALRM,handler); //SIGALRM 시그널 발생시 handler() 호출
    t.it_value.tv_sec = 1;
    t.it_value.tv_usec= 100000; //1.1sec 100,000 마이크로초 = 0.1초
    // t.it_value = (tv_sec)+(tv_usec)
    t.it_interval = t.it_value;
    //t.it_interval= 타이머가 다시 시작을 하는 주기

    setitimer(ITIMER_REAL, &t, 0x0); // 실제 시간 기반 ITIMER_REAL 타이머 시작
    //setitimer(실행할 타이머, 타이머 설정값, 이전 타이머 설정(NULL))
    while(1);
    
}


// ctrl+c를 누르기 전까지 1.1초마다 SIGALRM을 보내서 핸들러가 작동하게 하는 코드.