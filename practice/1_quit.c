#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void
handler (int sig)
{
    if (sig==SIGINT){ // SIGINT -> CTRL+C 누르면 발생하는 시그널
        printf("Do you want quit?");
        if(getchar() == 'y')
            exit(0);
    }
}

int 
main()
{
    signal(SIGINT, handler); // signal() 시그널 처리기 등록 ,SIGINT 발생시 handler 함수 호출
    while(1);
}