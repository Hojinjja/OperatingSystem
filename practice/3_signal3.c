#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void
handler (int sig)
{
    printf("Do you want quit?");
    if(getchar() == 'y')
        exit(0);
}

int 
main()
{
    int d;
    signal(SIGTERM, handler);
    scanf("%d",&d); //이거 없으면 ./a.out하자마자 끝남. 
}

//ps -u (프로세스 목록 보기)
//kill 10878 (10878 프로세스 kill 하기)
//kill -9 10878 (10878 프로세스 바로 kill 강제종료)