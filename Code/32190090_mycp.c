/**
 * 32190090_mycp.c	: cp command program
 * @author			: 강창진
 * @student ID 		: 32190090
 * @email			: 32190090@dankook.ac.kr
 * @date			: 2022.11.02
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_BUF 16

int main(int argc, char *argv[]) {
	int fd1, fd2, read_size;
	char buf[MAX_BUF];
	struct stat mycp_attribute;

	//입력한 인자가 3개가 아니면 실행을 취소한다.
	if(argc != 3) {
		printf("USAGE : %s file_name\n", argv[0]);
		exit(-1);
	}

	fd1 = open(argv[1], O_RDONLY);
	if(fd1 < 0) {
		printf("Can't open %s file with errno %d\n", argv[1], errno);
		exit(-1);
	}
	
	//fd2 파일을 생성해야 하므로 O_CREAT를 사용하고 O_EXCL을 통해 동일한 이름을 가진 파일이 있으면 생성을 하지 않도록 한다.
	fd2 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IRGRP | S_IROTH);
	if(fd2 < 0) {
		printf("Can't open %s file with errno %d\n", argv[2], errno);
		exit(-1);
	}
	
	while(1) {
		read_size = read(fd1, buf, MAX_BUF);
		if(read_size == 0)
			break;
		write(fd2, buf, read_size);
	}
	
	//fd1의 권한을 불러온다.
	fstat(fd1, &mycp_attribute);
	//불러온 권한을 토대로 fd2의 권한을 변경한다.
	fchmod(fd2, mycp_attribute.st_mode);
	
	close(fd1);
	close(fd2);
	return 0;
}
