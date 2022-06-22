#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>


#define BUFFER_SIZE 999

int main(int argc, char const *argv[]){
	int fd;
	int offset;
	int length;
	off_t fsize;
	char buffer[BUFFER_SIZE];

	offset = atoi(argv[1]); //입력받은 오프셋을 문자열에서 상수로 전환

	if(argc != 4){
		fprintf(stderr, "Usage : %s <offset> <""Data""> <file_name>\n Programm exit(1)\n", argv[0]);
		exit(1);
	}

	if((fd = open(argv[3], O_RDWR)) < 0){ //파일을 읽기,쓰기 용도로 오픈
		fprintf(stderr, "Open Error : %s\n Programm exit(1)\n",argv[3]);
		exit(1);
	}
	
	if((fsize = lseek(fd, 0, SEEK_END)) < 0){ //파일 전체 사이즈 파일의 끝 해당 위치 
		fprintf(stderr,"lseek Error\n Programm exit(1)\n");
		exit(1);
	} 

	if((lseek(fd, offset+1, SEEK_SET)) < 0){ // lseek을 이용하여 끼워넣기 할 오프셋으로 이동 [SEEK_SET을 이용해 파일 처음부터 이동]
		fprintf(stderr, "lseek Error\n Programm exit(1)\n");
		exit(1);
	}

	if((length = read(fd, buffer, BUFFER_SIZE)) < 0){ //파일을 오프셋위치부터 read하여 buffer에 넣고 read한 파일의 크기를 length에 넣어줌
		fprintf(stderr, "Read Error\n Programm exit(1)\n");
		exit(1);
	}

	if((lseek(fd, offset+1, SEEK_SET)) <0){ //입력 받았던 오프셋으로 이동
		fprintf(stderr,"lseek Error\n Programm exit(1)");
		exit(1);
	} 

	if((write(fd, argv[2], strlen(argv[2]))) < 0){ //입력받은 데이터를 write
		fprintf(stderr, "Write Error : %s\n Programm exit(1)",argv[2]);
		exit(1);
	}

	ftruncate(fd,(offset + strlen(argv[2]))); //파일의 처음부터 offset에서 입력받은 data 남기고 나머지 삭제

	
    
    if((write(fd, buffer, length))<0){ //bufffer에 있던 내용 다시 파일에 write
        fprintf(stderr,"Write Error\n Programm exit(1)\n");
        exit(1);
    } 
	
	close(fd);
	
    return 0;
}