#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipes[2] ; //������ ����

void
parent_proc()
{
	char * buf = 0x0 ;
	ssize_t s ; // ��ȣ�� �ִ� ���� Ÿ��
	size_t len = 0 ;

	close(pipes[0]) ; // �б� �� �ݱ�

	while ((s = getline(&buf, &len, stdin)) != -1) { 
        // stdin(ǥ�� �Է�)�� ���� ������ �ޱ�.
        // buf = ���� ������ ������ �� / len�� buf�� ũ�⸦ �����ϴ� ����
        // s�� -1�� �ƴϸ� (getline()�� �� ���� �����Ͱ� ���ų� ���� �߻��� -1 ��ȯ.)
		//getline()�� ����Ͽ� �� �پ� �Է� ����.
        buf[s - 1] = 0x0 ; //���� ���ڿ��� �������� null�� �־� ���ڿ� ����/ ������ ���� ǥ��

		ssize_t sent = 0 ;
		char * data = buf ; // �Ⱦ��̴� ����,,?

		while (sent < s) { // �Է¹��� �����͸� �������� ���� ��(pipes[1])�� ���� �ڽ� ���μ����� ����/
			//write()�� ���⸦ �޺�
            sent += write(pipes[1], buf + sent, s - sent) ;
		}

		free(buf) ;
		buf = 0x0 ;
		len = 0 ;
	}
	close(pipes[1]) ; // ���� �������� �ݱ�.
}

void
child_proc()
{
	char buf[32] ; // ���� ����� -> 0�� �ε���~31�� �ε���
	ssize_t s ;

	close(pipes[1]) ; //������ ���� �� �ݱ�

    //�������� �б� ���� pipes[0]���� �ִ� 31����Ʈ �迭�� �о�´�.
    //s�� read()�Լ��� ���� ������ ���� ����Ʈ ���� ����. 
    // -> ���������� �о��� �� ���� ����Ʈ �� ��ȯ, ������ �� ���� �� 0, ���� -1 ��ȯ. 
	while ((s = read(pipes[0], buf, 31)) > 0) { 
		buf[s + 1] = 0x0 ; // ���ڿ� ���Ḧ ���� �������� null ��� 
		printf(">%s\n", buf) ; //�о���� ���ڿ� ������ buf�� ���
	}
	exit(0) ;
}

int
main()
{
	pid_t child_pid ;
	int exit_code ;

	if (pipe(pipes) != 0) { // pipe()�� pipes�� ������ȭ -> ���� �� 0 ��ȯ
		perror("Error") ;
		exit(1) ;
	}
	printf("%d %d\n", pipes[0], pipes[1]) ; // �������� ���� ��ũ���� ���

	child_pid = fork() ; // ���� ���μ��� �����ؼ� �ڽ� ���μ����� ����. (�ڽ�=0, �θ�=�ڽ�pid)
	if (child_pid == 0) { //child_pid�� 0�̸�  
		child_proc() ; //child_proc()����
	}
	else { //child_pid�� 0�� �ƴϸ�
		parent_proc() ; // parent_proc()����
	}
	wait(&exit_code) ; // �θ� ���μ����� �ڽ� ���μ����� ���Ḧ ��ٸ��� , �����ڵ带 exit_code�� ����

	exit(0) ;
}

// ������ �� ./a.out�ϸ� 
// 3 4 �� ��µǴµ� �̰� ���� ��ũ���͸� ���� 3=pipe[0] �б� , 4=pipe[1] ����
// �� 31����Ʈ�� �ҷ����� 31����Ʈ�� ������ ���� �ٿ� ��µ� (�ݺ�) -> 31����Ʈ�� ©�� ���