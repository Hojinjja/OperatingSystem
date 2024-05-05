#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void
handler (int sig) //���ڷ� sig(���������� �ñ׳� ����) ����
{
    if (sig==SIGINT){ // SIGINT -> CTRL+C ������ �߻��ϴ� �ñ׳�
        printf("Do you want quit?");
        if(getchar() == 'y')
            exit(0);
    }
}

int 
main()
{
    signal(SIGINT, handler); // signal() �ñ׳� ó���� ��� ,SIGINT �߻��� handler �Լ� ȣ��
    while(1);
}


//ctrl+c�� ������ SIGINT �ñ׳��� �߻��Ǵµ� �װ� �ڵ鷯�� ó���ϴ� �ڵ�.