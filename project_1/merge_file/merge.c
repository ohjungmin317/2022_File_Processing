#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>

#define BUFFER_SIZE 999

int main(int argc, char const *argv[]) {
    
    int fd1,fd2,fd3;
    char buffer[BUFFER_SIZE];

    if(argc != 4){ // argc�� 4�� �ƴѰ�� �ش� �����޼��� ���
        fprintf(stderr, "Usage : %s <file1> <file2> <file3>\nProgram exit(1)\n",argv[0]);
        exit(1);
    }
    
    if((fd1=open(argv[1], O_RDWR)) < 0) { // �б� ����� ����1�� ����
        fprintf(stderr,"Open Error: %s\nProgram exit(1)\n",argv[1]);
        exit(1);
    }
    
    if((fd2=open(argv[2], O_RDWR)) < 0) { // �б� ����� ����2�� ����
        fprintf(stderr,"Open Error: %s\nProgram exit(1)\n",argv[2]);
        exit(1);
    }
    
    if((fd3=creat(argv[3],0666)) < 0) { // mode 666�� merge�� ���� create 
        fprintf(stderr,"Create Error: %s\nProgram exit(1)\n",argv[3]);
        exit(1);
    }
    
    // ����1�� ���� merge(����)
    while(1) {
        int length;
        if((length = read(fd1, buffer, BUFFER_SIZE)) <0) { // //����1�� �����º��� read�Ͽ� buffer�� �ְ� read�� ������ ũ�⸦ length�� �־���
            fprintf(stderr,"Read Error: %s\nProgram exit(1)\n",argv[1]);
            exit(1);
        }
        
        if((write(fd3, buffer, length))<0){ // ���� 3�� �ش� length�� �ְ� �̰��� buffer �ְ� ��� (file3���� file1 file2�� ���� ���� merge)
            fprintf(stderr,"Write Error\n Program exit(1)\n");
            exit(1);
        }
        
        for(int i=0;i<BUFFER_SIZE;i++) buffer[i] = '\0'; // null���� ���������� buffer�ȿ� �ִ� ���ڸ� ���� 
        
        if(length!=BUFFER_SIZE) break; // lenght�� BUFFER_SIZE ���� ������ break
        
    }
    
    // ����2�� ���� merge(����)
    while(1) {
        int length;
        if((length = read(fd2, buffer, BUFFER_SIZE)) <0){ //����2�� ��������ġ���� read�Ͽ� buffer�� �ְ� read�� ������ ũ�⸦ length�� �־���
            fprintf(stderr,"Read Error: %s\nProgram exit(1)\n",argv[2]);
            exit(1);
        }
        
        if((write(fd3, buffer, length)) < 0){ // ���� 3�� �ش� length�� �ְ� �̰��� buffer �ְ� ��� (file3���� file1 file2�� ���� ���� merge)
            fprintf(stderr,"Write Error\n Program exit(1)");
            exit(1);
        }
        
        if (length==BUFFER_SIZE) { // null���� ���������� buffer�ȿ� �ִ� ���ڸ� ���� 
            for(int i=0;i<BUFFER_SIZE;i++) buffer[i] = '\0';
        }
        else break;
        
    } 
    close(fd1);
    close(fd2);
    close(fd3);
    
    return 0;
}
