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

    if(argc != 4){ // argc가 4개 아닌경우 해당 에러메세지 출력
        fprintf(stderr, "Usage : %s <file1> <file2> <file3>\nProgram exit(1)\n",argv[0]);
        exit(1);
    }
    
    if((fd1=open(argv[1], O_RDWR)) < 0) { // 읽기 쓰기로 파일1번 오픈
        fprintf(stderr,"Open Error: %s\nProgram exit(1)\n",argv[1]);
        exit(1);
    }
    
    if((fd2=open(argv[2], O_RDWR)) < 0) { // 읽기 쓰기로 파일2번 오픈
        fprintf(stderr,"Open Error: %s\nProgram exit(1)\n",argv[2]);
        exit(1);
    }
    
    if((fd3=creat(argv[3],0666)) < 0) { // mode 666로 merge한 파일 create 
        fprintf(stderr,"Create Error: %s\nProgram exit(1)\n",argv[3]);
        exit(1);
    }
    
    // 파일1에 대한 merge(내용)
    while(1) {
        int length;
        if((length = read(fd1, buffer, BUFFER_SIZE)) <0) { // //파일1를 오프셋부터 read하여 buffer에 넣고 read한 파일의 크기를 length에 넣어줌
            fprintf(stderr,"Read Error: %s\nProgram exit(1)\n",argv[1]);
            exit(1);
        }
        
        if((write(fd3, buffer, length))<0){ // 파일 3은 해당 length에 넣고 이것을 buffer 넣고 출력 (file3번에 file1 file2에 대한 내용 merge)
            fprintf(stderr,"Write Error\n Program exit(1)\n");
            exit(1);
        }
        
        for(int i=0;i<BUFFER_SIZE;i++) buffer[i] = '\0'; // null문자 만날때까지 buffer안에 있는 문자를 읽음 
        
        if(length!=BUFFER_SIZE) break; // lenght가 BUFFER_SIZE 까지 읽으면 break
        
    }
    
    // 파일2에 대한 merge(내용)
    while(1) {
        int length;
        if((length = read(fd2, buffer, BUFFER_SIZE)) <0){ //파일2를 오프셋위치에서 read하여 buffer에 넣고 read한 파일의 크기를 length에 넣어줌
            fprintf(stderr,"Read Error: %s\nProgram exit(1)\n",argv[2]);
            exit(1);
        }
        
        if((write(fd3, buffer, length)) < 0){ // 파일 3은 해당 length에 넣고 이것을 buffer 넣고 출력 (file3번에 file1 file2에 대한 내용 merge)
            fprintf(stderr,"Write Error\n Program exit(1)");
            exit(1);
        }
        
        if (length==BUFFER_SIZE) { // null문자 만날때까지 buffer안에 있는 문자를 읽음 
            for(int i=0;i<BUFFER_SIZE;i++) buffer[i] = '\0';
        }
        else break;
        
    } 
    close(fd1);
    close(fd2);
    close(fd3);
    
    return 0;
}
