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
        fprintf(stderr, "Usage : %s <orignal_file_name> <copy_file_name>\n exit(1)\n",argv[0]); // argc�� �Է¹޴� ���� 3���� �ƴѰ�� error �޼��� ��ȯ
        exit(1);
    }

    if((fd1 = open(argv[1], O_RDONLY)) < 0){  // argc[1] file1�� open�ϸ鼭 ������ �б⸸ ����
        printf("Open Error for %s\n Program exit(1)\n",argv[1]);
        exit(1);
    }

    if((fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC,0666)) < 0){ // argv[2] �Է��� �ް� ���纻������ �������� 0666���� ����
        printf("Open Error for %s\n Program exit(0)\n",argv[2]);
        exit(1);
    }

      while((cnt = read(fd1,buffer,10)) > 0){ //fd1�� �ִ� ������ 10byte�� read�ϰ� buffer�� ���� read�� ���� buffer ũ�⸸ŭ cnt�ؼ� write �Լ��� cnt �Ѹ�ŭ �־ ���Ͽ� ����
          if((write(fd2,buffer,cnt)<0)){
              fprintf(stderr,"write error\n Program exit(1)");
              exit(1);
          }
      }

      close(fd1);
      close(fd2);
    
    
    return 0;
}