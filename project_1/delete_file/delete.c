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
	off_t fsize; // filesize ���� -> ������ü������
	char buffer[BUFFER_SIZE];
	offset = atoi(argv[1]); // offset ����
	Data_byte = atoi(argv[2]);  // Databyte ����
	
	if(argc != 4){ // argc�� 4�� �ƴѰ�� �ش� �����޼��� ���
		fprintf(stderr, "Usage : %s<offset> <data_byte> <filename>\n Program exit(1)", argv[0]);
		exit(1);
	}

	if((fd = open(argv[3], O_RDWR)) < 0){ //������ �б�,���� �������� ����
		fprintf(stderr, "Open Error : %s\n Program exit(1)",argv[3]);
		exit(1);
	}

	if((fsize = lseek(fd, 0, SEEK_SET)) < 0){ //������ ��ü ������ ���� ��ġ�� ����
		fprintf(stderr, "lseek Error\n Program exit(1)");
		exit(1);
	}

	if(((fsize)<(offset + Data_byte))&&(fsize > offset)){ //<����Ʈ ��>�� ���밪��ŭ�� �����Ͱ� ���� Ȥ�� �����ʿ� �������� ������ �� ���� ������ ����
		ftruncate(fd, offset); 
	}

	else{

		if(Data_byte<0){ // Databyte�� ������ ��� [offset�� �������� �̵�]
			offset+=Data_byte;

		if((lseek(fd, offset + abs(Data_byte), SEEK_SET)) < 0){ // Databyte�� ���� �����̱⿡ ���밪���� ���� ��ġ�� ��Ÿ���� lseek�� ����ؼ� offset �̵�  
			fprintf(stderr, "lseek Error\n Program exit(1)");
			exit(1);
		}
			}

		else{
			if((lseek(fd, offset + Data_byte, SEEK_SET)) < 0){ // Databyte�� ���� ����� ��� lseek�� ����ؼ� offset �̵�
			fprintf(stderr, "lseek Error\n Program exit(1)");
			exit(1);
		}	
			}

		if((length = read(fd, buffer, BUFFER_SIZE)) < 0){ // offset �̵��� ��ġ���� read 
			fprintf(stderr, "read Error\n Program exit(1)");
			exit(1);
		}
	
		ftruncate(fd, offset); //�ش� �Է¹��� offset���� ���ΰ� �������� ���� 
	
		if((lseek(fd, offset, SEEK_SET)) <0){ //offset �̵�
			fprintf(stderr,"lseek Error\n Program exit(1)");
			exit(1);
		} 
		if((write(fd, buffer, length)) < 0){ //buffer�� �־���� ������ write�ؼ� �� ���
			fprintf(stderr, "write error\n Program exit(1)");
			exit(1);
			}
		}

	close(fd);
	return 0;
}