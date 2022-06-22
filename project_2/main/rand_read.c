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
#define SUFFLE_NUM    10000   // 이 값은 마음대로 수정할 수 있음.

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
// 필요한 함수가 있으면 더 추가할 수 있음.

int main(int argc, char **argv)
{
    int *read_order_list;
    int num_of_records; // 레코드 파일에 저장되어 있는 전체 레코드의 수
  
    struct timeval startTime, endTime, diffTime;

    int fp;
    int length;

    char* buffer = malloc(sizeof(char) * BUF); // 200byte buffer에 동적할당

    if (argc != 2) { //실행 인자 입력이 제대로 되지 않은 경우
        fprintf(stderr, "usage : %s <record_filename>\n exit(1)\n", argv[0]);
        exit(1);
    } 
    
    
    
    if((fp = open(argv[1], O_RDONLY)) < 0){ //인자로 입력받은 파일 오픈
		fprintf(stderr,"Open Error for %s\n exit(1)\n", argv[1]);
		exit(1);
	} 
    
    gettimeofday(&startTime, NULL); // 측정시작

    if((length = lseek(fp, 0, SEEK_END))<0){ // 레코드 파일 전체 크기
        fprintf(stderr, "fseek error\n exit(1)\n");
        exit(1);
    } 

    num_of_records = length / BUF; // 레코드 파일 개수 
    
    read_order_list = (int*) malloc(num_of_records*sizeof(int)); // 레코드 파일


    for(int i=0 ; i<num_of_records; i++) //입력한 레코드 수 만큼 배열에 넣음
		read_order_list[i] = i;

    // 이 함수를 실행하면 'read_order_list' 배열에는 읽어야 할 레코드 번호들이 나열되어 저장됨
    GenRecordSequence(read_order_list, num_of_records);
   
    
    for(int i=0;i<num_of_records;i++) {
        /*if((lseek(fp, read_order_list[i]*BUF, SEEK_SET))<0){
            fprintf(stderr,"fseek error\n exit(1)\n");
            exit(1);
        }*/
        lseek(fp, read_order_list[i]*BUF, SEEK_SET);
                if(read(fp,buffer,BUF)<0){
                    fprintf(stderr,"read error\n exit(1)\n");
                    exit(1);
                }
		}
       close(fp);
        
    gettimeofday(&endTime, NULL); // 측정종료

    diffTime.tv_usec = endTime.tv_usec - startTime.tv_usec;
    if( diffTime.tv_usec < 0 )
    {
        diffTime.tv_usec = diffTime.tv_usec + 1000000;
    }

    printf("%ld usec\n",diffTime.tv_usec);    
    
    
    free(buffer);

    return 0;
}

void GenRecordSequence(int *list, int n)
{
    int i, j, k;

    srand((unsigned int)time(0));

    for(i=0; i<n; i++)
    {
        list[i] = i;
    }
    
    for(i=0; i<SUFFLE_NUM; i++)
    {
        j = rand() % n;
        k = rand() % n;
        swap(&list[j], &list[k]);
    }
}

void swap(int *a, int *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}
