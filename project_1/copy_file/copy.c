#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>


#define BUFFER_SIZE 999


int main(int argc, char const *argv[])

{
    char buffer[BUFFER_SIZE];
    int fd1,fd2;
    int cnt;

    if(argc != 3){
        fprintf(stderr, "Usage : %s <orignal_file_name> <copy_file_name>\n exit(1)\n",argv[0]); // argc가 입력받는 것이 3개가 아닌경우 error 메세지 반환
        exit(1);
    }

    if((fd1 = open(argv[1], O_RDONLY)) < 0){  // argc[1] file1번 open하면서 권한은 읽기만 가능
        printf("Open Error for %s\n Program exit(1)\n",argv[1]);
        exit(1);
    }

    if((fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC,0666)) < 0){ // argv[2] 입력을 받고 복사본파일은 쓰기전용 0666으로 오픈
        printf("Open Error for %s\n Program exit(0)\n",argv[2]);
        exit(1);
    }

      while((cnt = read(fd1,buffer,10)) > 0){ //fd1에 있는 파일은 10byte씩 read하고 buffer에 넣음 read한 것은 buffer 크기만큼 cnt해서 write 함수에 cnt 한만큼 넣어서 파일에 복사
          if((write(fd2,buffer,cnt)<0)){
              fprintf(stderr,"write error\n Program exit(1)");
              exit(1);
          }
      }

      close(fd1);
      close(fd2);
    
    
    return 0;
}