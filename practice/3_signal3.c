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
    scanf("%d",&d); //�̰� ������ ./a.out���ڸ��� ����. 
}

//ps -u (���μ��� ��� ����)
//kill 10878 (10878 ���μ��� kill �ϱ�)
//kill -9 10878 (10878 ���μ��� �ٷ� kill ��������)