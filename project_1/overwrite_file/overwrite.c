#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc,  char const *argv[]) {
    
    int fd;
    int offset=atoi(argv[1]); // offset ���ڿ����� ����� ��ȯ

    if(argc != 4){ // argc�� 4�� �ƴѰ�� �ش� �����޼��� ���
        fprintf(stderr,"Usage : %s <offset> <""data""> <filename>\nProgram exit(1)\n",argv[0]);
        exit(1);
    }
    
    if((fd=open(argv[3], O_RDWR)) < 0) { // �ش� ������ �а� ����� open + �߸��Է������ÿ� error �޼��� ���  
        fprintf(stderr,"open Error: %s\nProgram exit(1)\n",argv[3]);
        exit(1);
    }
    
    if((lseek(fd, offset, SEEK_CUR)) <0){ // �ش� offset�� ������ġ�� �̵� 
        fprintf(stderr,"lseek Error\nProgram exit(1)\n");
        exit(1);
    }
    
    if((write(fd, argv[2], strlen(argv[2])))<0){ // �Է¹��� ���ڿ��� ���Ͽ� ������.
        fprintf(stderr,"write Error %s\nProgram exit(1)",argv[2]);
        exit(1);
    }
    close(fd);
    return 0;

    
}
