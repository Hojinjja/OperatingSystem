#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>

void
sigchld_handler(int sig)
{
    pid_t child; // �ڽ� ���μ��� pid ����
    int exitcode; // exitcode ����
    child = wait(&exitcode); //wait()�Լ��� child(�ڽ� ���μ���)�� ����Ǳ� ��ٸ���, ���� ������ exitcode�� ����
                            // child���� wait()�Լ��� ����� ����� �ڽ� ���μ����� pid�� ����.
    printf("> child process %d is terminated with exit code %d\n",
            child, WEXITSTATUS(exitcode));
            //WEXITSTATUS(exitcode)�� exitcode���� ���� ���� �ڵ带 ȣ��.

}

int 
main()
{
    signal(SIGCHLD, sigchld_handler); //SIGCHLD �ñ׳� �߻��� ,sigchld_handlerȣ��

    pid_t child; // �ڽ� ���μ��� pid ����
    if (child = fork()){ //fork()�� ȣ���Ͽ� ���� ���μ��� ����, ��ȯ�� 0= �ڽ����μ��� / 0�� �ƴϸ� �θ� ���μ���
        //�θ� ���μ����� �۵��ϴ� �ڵ�
        sleep(3); //3�ʰ� ����
        kill(child, SIGTERM); // kill()�Լ��� ����Ͽ� �ڽ� ���μ������� SIGTERM �ñ׳��� ������ ���� ��û
        while(1); // kill���� �ڽ� ���μ����� ��������, �θ� ���μ����� ���⿡�� ���� ������ �ɸ�.
                    //������ ���α׷��� ���ᰡ �ȵȴ�.
    }
    else{
        //�ڽ� ���μ����� �۵��ϴ� �ڵ�.
        while(1);
    }

}