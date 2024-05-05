#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipes[2] ; // ���� ������ pipes ����

void
child_proc()
{
	dup2(pipes[1], 1 /*standard output*/) ; // �ι�° ���� '1'�� ǥ�� ����� ����.
     // �ڽ� ���μ����� ǥ�� ���(��ũ���� 1)�� pipes[1]�� �����̷�Ʈ.
     // �ڽ����μ��������� execl�� ���ؼ� ./hello�� �����Ѵ�. -> ���� ����� pipe[1]�� �����̷�Ʈ
	
    execl("./hello", "hello", (char *) 0x0) ; // ���� ���μ����� ./hello ���α׷����� ��ü.
            // ./hello : ���� �Ϸ��� ���α׷��� ��� -> ./ = ���� ���͸��� �ǹ� 
            // "hello" : ���α׷� ���ο��� �ڽ��� ��� ������ ������ ���� -> �Ϲ������� ���α׷� �̸� 
            // (char *) 0x0 : ���� ����� ���� ��Ÿ���� ���� NULL������
}

void
parent_proc()
{
	char buf[32] ;
	ssize_t s ;

	close(pipes[1]) ; // ������ 1�� (���� ����)

	while ((s = read(pipes[0], buf, 31)) > 0) { // ������ 0������ ����, buf�� 31����Ʈ��ŭ �о ����
		buf[s + 1] = 0x0 ; // ���ڿ� ���Ḧ ǥ���ϱ� ���� �������� null
		printf(">%s\n", buf) ; // �о�� ���� ���
	}
	exit(0) ;
}

int
main()
{
	int exit_code ;

	if (pipe(pipes) != 0) { // ������ȭ 
		perror("Error") ;
		exit(1) ;
	}

	pid_t child_pid ;
	if (child_pid = fork()) { // fork�� �ڽ� ���μ���,�θ� ���μ��� �б�
		parent_proc() ;
	}
	else {
		child_proc() ;
	}
	wait(&exit_code) ;

	exit(0) ;
}