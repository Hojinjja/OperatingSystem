#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void
handler (int sig) //인자로 sig(정수형으로 시그널 종류) 받음
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


//ctrl+c를 누르면 SIGINT 시그널이 발생되는데 그걸 핸들러가 처리하는 코드.