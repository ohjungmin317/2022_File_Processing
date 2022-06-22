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
    int offset=atoi(argv[1]); // offset 문자열에서 상수로 전환

    if(argc != 4){ // argc가 4개 아닌경우 해당 에러메세지 출력
        fprintf(stderr,"Usage : %s <offset> <""data""> <filename>\nProgram exit(1)\n",argv[0]);
        exit(1);
    }
    
    if((fd=open(argv[3], O_RDWR)) < 0) { // 해당 파일을 읽고 쓰기로 open + 잘못입력했을시에 error 메세지 출력  
        fprintf(stderr,"open Error: %s\nProgram exit(1)\n",argv[3]);
        exit(1);
    }
    
    if((lseek(fd, offset, SEEK_CUR)) <0){ // 해당 offset을 파일위치에 이동 
        fprintf(stderr,"lseek Error\nProgram exit(1)\n");
        exit(1);
    }
    
    if((write(fd, argv[2], strlen(argv[2])))<0){ // 입력받은 문자열을 파일에 덮어씌운다.
        fprintf(stderr,"write Error %s\nProgram exit(1)",argv[2]);
        exit(1);
    }
    close(fd);
    return 0;

    
}
