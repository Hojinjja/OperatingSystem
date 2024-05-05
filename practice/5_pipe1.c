#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pipes[2] ; //파이프 생성

void
parent_proc()
{
	char * buf = 0x0 ;
	ssize_t s ; // 부호가 있는 정수 타입
	size_t len = 0 ;

	close(pipes[0]) ; // 읽기 측 닫기

	while ((s = getline(&buf, &len, stdin)) != -1) { 
        // stdin(표준 입력)을 통해 데이터 받기.
        // buf = 읽은 데이터 저장할 곳 / len은 buf의 크기를 저장하는 변수
        // s가 -1이 아니면 (getline()은 더 읽을 데이터가 없거나 오류 발생시 -1 반환.)
		//getline()을 사용하여 한 줄씩 입력 받음.
        buf[s - 1] = 0x0 ; //읽은 문자열의 마지막에 null을 넣어 문자열 종료/ 끝나는 지점 표시

		ssize_t sent = 0 ;
		char * data = buf ; // 안쓰이는 변수,,?

		while (sent < s) { // 입력받은 데이터를 파이프의 쓰기 측(pipes[1])을 통해 자식 프로세스로 전송/
			//write()로 쓰기를 받복
            sent += write(pipes[1], buf + sent, s - sent) ;
		}

		free(buf) ;
		buf = 0x0 ;
		len = 0 ;
	}
	close(pipes[1]) ; // 쓰기 끝났으면 닫기.
}

void
child_proc()
{
	char buf[32] ; // 버퍼 만들기 -> 0번 인덱스~31번 인덱스
	ssize_t s ;

	close(pipes[1]) ; //파이프 쓰기 측 닫기

    //파이프의 읽기 측인 pipes[0]에서 최대 31바이트 배열을 읽어온다.
    //s는 read()함수에 의해 실제로 읽힌 바이트 수를 저장. 
    // -> 성공적으로 읽었을 때 읽은 바이트 수 반환, 파일의 끝 도달 시 0, 오류 -1 반환. 
	while ((s = read(pipes[0], buf, 31)) > 0) { 
		buf[s + 1] = 0x0 ; // 문자열 종료를 위해 마지막에 null 사용 
		printf(">%s\n", buf) ; //읽어들인 문자열 데이터 buf를 출력
	}
	exit(0) ;
}

int
main()
{
	pid_t child_pid ;
	int exit_code ;

	if (pipe(pipes) != 0) { // pipe()로 pipes를 파이프화 -> 성공 시 0 반환
		perror("Error") ;
		exit(1) ;
	}
	printf("%d %d\n", pipes[0], pipes[1]) ; // 파이프의 파일 디스크립터 출력

	child_pid = fork() ; // 현재 프로세스 복제해서 자식 프로세스를 생성. (자식=0, 부모=자식pid)
	if (child_pid == 0) { //child_pid가 0이면  
		child_proc() ; //child_proc()으로
	}
	else { //child_pid가 0이 아니면
		parent_proc() ; // parent_proc()으로
	}
	wait(&exit_code) ; // 부모 프로세스는 자식 프로세스의 종료를 기다리고 , 종료코드를 exit_code에 저장

	exit(0) ;
}

// 컴파일 후 ./a.out하면 
// 3 4 가 출력되는데 이건 파일 디스크립터를 말함 3=pipe[0] 읽기 , 4=pipe[1] 쓰기
// 총 31바이트를 불러오고 31바이트가 넘으면 다음 줄에 출력됨 (반복) -> 31바이트씩 짤라서 출력