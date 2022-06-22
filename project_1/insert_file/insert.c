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

	offset = atoi(argv[1]); //�Է¹��� �������� ���ڿ����� ����� ��ȯ

	if(argc != 4){
		fprintf(stderr, "Usage : %s <offset> <""Data""> <file_name>\n Programm exit(1)\n", argv[0]);
		exit(1);
	}

	if((fd = open(argv[3], O_RDWR)) < 0){ //������ �б�,���� �뵵�� ����
		fprintf(stderr, "Open Error : %s\n Programm exit(1)\n",argv[3]);
		exit(1);
	}
	
	if((fsize = lseek(fd, 0, SEEK_END)) < 0){ //���� ��ü ������ ������ �� �ش� ��ġ 
		fprintf(stderr,"lseek Error\n Programm exit(1)\n");
		exit(1);
	} 

	if((lseek(fd, offset+1, SEEK_SET)) < 0){ // lseek�� �̿��Ͽ� �����ֱ� �� ���������� �̵� [SEEK_SET�� �̿��� ���� ó������ �̵�]
		fprintf(stderr, "lseek Error\n Programm exit(1)\n");
		exit(1);
	}

	if((length = read(fd, buffer, BUFFER_SIZE)) < 0){ //������ ��������ġ���� read�Ͽ� buffer�� �ְ� read�� ������ ũ�⸦ length�� �־���
		fprintf(stderr, "Read Error\n Programm exit(1)\n");
		exit(1);
	}

	if((lseek(fd, offset+1, SEEK_SET)) <0){ //�Է� �޾Ҵ� ���������� �̵�
		fprintf(stderr,"lseek Error\n Programm exit(1)");
		exit(1);
	} 

	if((write(fd, argv[2], strlen(argv[2]))) < 0){ //�Է¹��� �����͸� write
		fprintf(stderr, "Write Error : %s\n Programm exit(1)",argv[2]);
		exit(1);
	}

	ftruncate(fd,(offset + strlen(argv[2]))); //������ ó������ offset���� �Է¹��� data ����� ������ ����

	
    
    if((write(fd, buffer, length))<0){ //bufffer�� �ִ� ���� �ٽ� ���Ͽ� write
        fprintf(stderr,"Write Error\n Programm exit(1)\n");
        exit(1);
    } 
	
	close(fd);
	
    return 0;
}