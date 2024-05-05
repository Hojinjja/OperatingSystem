#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipes[2] ; // 전역 변수로 pipes 선언

void
child_proc()
{
	dup2(pipes[1], 1 /*standard output*/) ; // 두번째 인자 '1'이 표준 출력을 말함.
     // 자식 프로세스의 표준 출력(디스크립터 1)을 pipes[1]로 리다이렉트.
     // 자식프로세스에서는 execl을 통해서 ./hello를 실행한다. -> 실행 결과는 pipe[1]로 리다이렉트
	
    execl("./hello", "hello", (char *) 0x0) ; // 현재 프로세스를 ./hello 프로그램으로 대체.
            // ./hello : 실행 하려는 프로그램의 경로 -> ./ = 현재 디렉터리를 의미 
            // "hello" : 프로그램 내부에서 자신을 어떻게 참조할 것인지 정의 -> 일반적으로 프로그램 이름 
            // (char *) 0x0 : 인자 목록의 끝을 나타내기 위한 NULL포인터
}

void
parent_proc()
{
	char buf[32] ;
	ssize_t s ;

	close(pipes[1]) ; // 파이프 1번 (쓰기 닫음)

	while ((s = read(pipes[0], buf, 31)) > 0) { // 파이프 0번으로 읽음, buf에 31바이트만큼 읽어서 저장
		buf[s + 1] = 0x0 ; // 문자열 종료를 표시하기 위한 마지막에 null
		printf(">%s\n", buf) ; // 읽어온 문자 출력
	}
	exit(0) ;
}

int
main()
{
	int exit_code ;

	if (pipe(pipes) != 0) { // 파이프화 
		perror("Error") ;
		exit(1) ;
	}

	pid_t child_pid ;
	if (child_pid = fork()) { // fork로 자식 프로세스,부모 프로세스 분기
		parent_proc() ;
	}
	else {
		child_proc() ;
	}
	wait(&exit_code) ;

	exit(0) ;
}