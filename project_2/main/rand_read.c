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
#define SUFFLE_NUM    10000   // �� ���� ������� ������ �� ����.

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
// �ʿ��� �Լ��� ������ �� �߰��� �� ����.

int main(int argc, char **argv)
{
    int *read_order_list;
    int num_of_records; // ���ڵ� ���Ͽ� ����Ǿ� �ִ� ��ü ���ڵ��� ��
  
    struct timeval startTime, endTime, diffTime;

    int fp;
    int length;

    char* buffer = malloc(sizeof(char) * BUF); // 200byte buffer�� �����Ҵ�

    if (argc != 2) { //���� ���� �Է��� ����� ���� ���� ���
        fprintf(stderr, "usage : %s <record_filename>\n exit(1)\n", argv[0]);
        exit(1);
    } 
    
    
    
    if((fp = open(argv[1], O_RDONLY)) < 0){ //���ڷ� �Է¹��� ���� ����
		fprintf(stderr,"Open Error for %s\n exit(1)\n", argv[1]);
		exit(1);
	} 
    
    gettimeofday(&startTime, NULL); // ��������

    if((length = lseek(fp, 0, SEEK_END))<0){ // ���ڵ� ���� ��ü ũ��
        fprintf(stderr, "fseek error\n exit(1)\n");
        exit(1);
    } 

    num_of_records = length / BUF; // ���ڵ� ���� ���� 
    
    read_order_list = (int*) malloc(num_of_records*sizeof(int)); // ���ڵ� ����


    for(int i=0 ; i<num_of_records; i++) //�Է��� ���ڵ� �� ��ŭ �迭�� ����
		read_order_list[i] = i;

    // �� �Լ��� �����ϸ� 'read_order_list' �迭���� �о�� �� ���ڵ� ��ȣ���� �����Ǿ� �����
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
        
    gettimeofday(&endTime, NULL); // ��������

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
