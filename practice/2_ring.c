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
    struct itimerval t; // ����ü �ν��Ͻ� t ����

    signal(SIGALRM,handler); //SIGALRM �ñ׳� �߻��� handler() ȣ��
    t.it_value.tv_sec = 1;
    t.it_value.tv_usec= 100000; //1.1sec 100,000 ����ũ���� = 0.1��
    // t.it_value = (tv_sec)+(tv_usec)
    t.it_interval = t.it_value;
    //t.it_interval= Ÿ�̸Ӱ� �ٽ� ������ �ϴ� �ֱ�

    setitimer(ITIMER_REAL, &t, 0x0); // ���� �ð� ��� ITIMER_REAL Ÿ�̸� ����
    //setitimer(������ Ÿ�̸�, Ÿ�̸� ������, ���� Ÿ�̸� ����(NULL))
    while(1);
    
}


// ctrl+c�� ������ ������ 1.1�ʸ��� SIGALRM�� ������ �ڵ鷯�� �۵��ϰ� �ϴ� �ڵ�.