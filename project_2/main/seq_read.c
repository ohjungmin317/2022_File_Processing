#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>

#define BUF 200

int main(int argc, char **argv)
{
    //
    // 레코드 파일로부터 전체 레코들을 순차적으로 read할 때
    // 걸리는 전체 시간을 측정하는 코드 구현
    //
    
    struct timeval startTime, endTime, diffTime;
    int fp;
    int length;
    char* buffer = malloc(sizeof(char) * BUF);

     if (argc != 2) { //실행 인자 입력이 제대로 되지 않은 경우
        fprintf(stderr, "usage : %s <record_filename>\n exit(1)\n", argv[0]);
        exit(1);
    }

    
    
    if((fp = open(argv[1], O_RDONLY)) < 0){ //인자로 입력받은 파일 오픈
		fprintf(stderr,"Open Error for %s\n exit(1)\n", argv[1]);
		exit(1);
	}

    gettimeofday(&startTime, NULL); // 측정 시작


    if((length = lseek(fp, 0, SEEK_END))<0){// 레코드 파일 전체 크기 
        fprintf(stderr,"fseek error\n exit(1)\n");
        exit(1);
    } 

    int num = length / BUF; // 레코드 파일 개수 

    
    
    for(int i=0; i<num; i++){
        if((read(fp, buffer, BUF)) <0 ){
            fprintf(stderr,"read error\n exit(1)\n");
            exit(1);
            printf("\n");
        }
    }

    gettimeofday(&endTime, NULL);

     diffTime.tv_usec = endTime.tv_usec - startTime.tv_usec;
    if( diffTime.tv_usec < 0 )
    {
        diffTime.tv_usec = diffTime.tv_usec + 1000000;
    }

    printf("%ld usec\n",diffTime.tv_usec);
    
    free(buffer);

    return 0;
}
