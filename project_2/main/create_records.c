#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
// 필요하면 header 파일 추가 가능

//
// input parameters: 학생 레코드 수, 레코드 파일
//
typedef struct Student
{
    char id[10];
    char name[20];
    char school[20];
    char school_major[20];
    char sub_info[130];
} Student;

int main(int argc, char **argv)
{
    
    Student person;

    int rfile;

    int i;


    if (argc != 3) { //실행 인자 입력이 제대로 되지 않은 경우
        fprintf(stderr, "usage : %s <#record> <filename>\n exit(1)\n", argv[0]);
        exit(1);
        printf("\n");
    }

    if((rfile=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0666))<0){
	fprintf(stderr,"Open error for %s\n exit(1)\n",argv[2]);
	exit(1);
    printf("\n");
    }
    
    int num = atoi(argv[1]); // 레코드 파일 개수 입력
    
    for(i=0; i<num; i++){  
        strcpy(person.id,"20180775");
        strcpy(person.name,"ohjungmin");
        strcpy(person.school,"soongsil university");
        strcpy(person.school_major,"computer");
        strcpy(person.sub_info,"he like apple");

    }

    for(i=0; i<num; i++){
         if(write(rfile,(char *)&person,sizeof(person))<0){
         fprintf(stderr,"Write Error\n exit(1)\n");
         exit(1);
         printf("\n");
        }
    }


    close(rfile);
	return 0;
}