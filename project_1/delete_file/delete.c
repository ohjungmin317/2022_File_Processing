#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#define BUFFER_SIZE 999

int main(int argc, char const *argv[]){
	int fd;
	int offset, Data_byte;
	int length;
	off_t fsize; // filesize 선언 -> 파일전체사이즈
	char buffer[BUFFER_SIZE];
	offset = atoi(argv[1]); // offset 선언
	Data_byte = atoi(argv[2]);  // Databyte 선언
	
	if(argc != 4){ // argc가 4개 아닌경우 해당 에러메세지 출력
		fprintf(stderr, "Usage : %s<offset> <data_byte> <filename>\n Program exit(1)", argv[0]);
		exit(1);
	}

	if((fd = open(argv[3], O_RDWR)) < 0){ //파일을 읽기,쓰기 전용으로 오픈
		fprintf(stderr, "Open Error : %s\n Program exit(1)",argv[3]);
		exit(1);
	}

	if((fsize = lseek(fd, 0, SEEK_SET)) < 0){ //파일의 전체 사이즈 시작 위치로 고정
		fprintf(stderr, "lseek Error\n Program exit(1)");
		exit(1);
	}

	if(((fsize)<(offset + Data_byte))&&(fsize > offset)){ //<바이트 수>의 절대값만큼의 데이터가 왼쪽 혹은 오른쪽에 존재하지 않으면 그 다음 데이터 삭제
		ftruncate(fd, offset); 
	}

	else{

		if(Data_byte<0){ // Databyte가 음수인 경우 [offset을 왼쪽으로 이동]
			offset+=Data_byte;

		if((lseek(fd, offset + abs(Data_byte), SEEK_SET)) < 0){ // Databyte의 값이 음수이기에 절대값으로 만들어서 위치를 나타내고 lseek를 사용해서 offset 이동  
			fprintf(stderr, "lseek Error\n Program exit(1)");
			exit(1);
		}
			}

		else{
			if((lseek(fd, offset + Data_byte, SEEK_SET)) < 0){ // Databyte의 값이 양수인 경우 lseek을 사용해서 offset 이동
			fprintf(stderr, "lseek Error\n Program exit(1)");
			exit(1);
		}	
			}

		if((length = read(fd, buffer, BUFFER_SIZE)) < 0){ // offset 이동한 위치까지 read 
			fprintf(stderr, "read Error\n Program exit(1)");
			exit(1);
		}
	
		ftruncate(fd, offset); //해당 입력받은 offset까지 놔두고 나머지는 삭제 
	
		if((lseek(fd, offset, SEEK_SET)) <0){ //offset 이동
			fprintf(stderr,"lseek Error\n Program exit(1)");
			exit(1);
		} 
		if((write(fd, buffer, length)) < 0){ //buffer에 넣어놨던 내용을 write해서 값 출력
			fprintf(stderr, "write error\n Program exit(1)");
			exit(1);
			}
		}

	close(fd);
	return 0;
}