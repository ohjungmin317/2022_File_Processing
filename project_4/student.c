#include <stdio.h>		// �ʿ��� header file �߰� ����
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "student.h"
#define BUFFER_SIZE 1024

//------------------------------------function----------------------------------------//
int readRecord(FILE *fp, STUDENT *s, int rrn);
void unpack(const char *recordbuf, STUDENT *s);
int writeRecord(FILE *fp, const STUDENT *s, int rrn);
void pack(char *recordbuf, const STUDENT *s);
int appendRecord(FILE *fp, char *id, char* name, char *dept, char *addr, char *email);
void searchRecord(FILE *fp, enum FIELD f, char *keyval);
void printRecord(const STUDENT *s);
enum FIELD getFieldID(char *fieldname);
//-----------------------------------------------------------------------------------//


STUDENT* s;
char* f_name;
enum FIELD enumfile;

typedef struct HEADER{
   int header_record;
   int header_reserved;   
}HEADER;

int readRecord(FILE *fp, STUDENT *s, int rrn){
   char *recordbuf;
	recordbuf = (char*)malloc(RECORD_SIZE); // recordbuf�� �� �Ҵ� 

	fseek(fp, HEADER_SIZE + (rrn *RECORD_SIZE), SEEK_SET); // rrn�� ���� ���� ��ġ�� ����
	
	if(fread(recordbuf, RECORD_SIZE, 1, fp) < 0){ 
      fprintf(stderr,"fread error\n");
      return 0;
   } // fread�� recordbuf��ŭ �о���� ���࿡ �� ������ �ȵǸ� return 0���� ��ȯ
   else{
      unpack(recordbuf, s);
      return 1;
   } // ������ �ߵǸ� unpack() �Լ� ȣ�� return 1�� ��ȯ

}


void unpack(const char *recordbuf, STUDENT *s){
   char *temp_recordbuf;

   temp_recordbuf = (char*)malloc(RECORD_SIZE); //���ڵ� ���� �ӽ� ���� ������ �Ҵ�
   memset(temp_recordbuf, 0, sizeof(STUDENT));
   
	char *token; //��ū �и� ���� ������
	strcpy(temp_recordbuf, recordbuf); //�ӽ� ���� ������ ����

	token = strtok(temp_recordbuf, "#"); //������ �������� ��ū �и�
	strcpy(s->id, token); //�й� ����

	token = strtok(NULL, "#");
	strcpy(s-> name, token); //�̸� ����

	token = strtok(NULL, "#");
	strcpy(s -> dept, token); //�а� ����

	token = strtok(NULL, "#");
	strcpy(s -> addr, token); //�ּ� ����

	token = strtok(NULL, "#");
	strcpy(s -> email, token); //�̸��� ����

   free(temp_recordbuf);
}

int writeRecord(FILE *fp, const STUDENT *s, int rrn){

	char* recordbuf; 
   recordbuf = (char*)malloc(RECORD_SIZE);

	pack(recordbuf, s); // recordbuf �Ҵ� �ϰ� -> pack�� ���� 

	//fp = fopen(f_name, "a");
   
   fseek(fp, 0, SEEK_SET);

   fseek(fp, (rrn *RECORD_SIZE) + HEADER_SIZE, SEEK_SET);

	int success = fwrite(recordbuf, RECORD_SIZE, 1, fp);

   
   if(success != 1 ){
      return 0;
   }
   else return 1;
	
	free(recordbuf);
}

void pack(char *recordbuf, const STUDENT *s){
	sprintf(recordbuf, "%s#%s#%s#%s#%s", s->id, s->name, s->dept, s->addr, s->email);
}

int appendRecord(FILE *fp, char *id, char *name, char *dept, char *addr, char *email){

   strcpy(s->id, id);
   strcpy(s->name, name);
   strcpy(s->dept, dept);
   strcpy(s->addr, addr);
   strcpy(s->email, email);

   HEADER h;

	/*if((fp = fopen(f_name, "r")) == NULL){
      fprintf(stderr, "fopen error\n");
      exit(1);
   }*/

   fseek(fp, 0, SEEK_SET);
   memset(&h, 0 , sizeof(HEADER_SIZE));

   if(fread(&h, HEADER_SIZE, 1, fp) <0){
      fprintf(stderr,"fread error\n exit(1)\n");
      exit(1);
   }
   
   if(h.header_record == 0){
      h.header_record++;
      fseek(fp,0,SEEK_SET);
      fwrite(&h,HEADER_SIZE,1,fp);

      if(writeRecord(fp, s, h.header_record -1) > 0) 
      return 1;
      else 
      return 0;
   } // ó�� append �� ��  
   else{
      h.header_record++;
      fseek(fp,0,SEEK_SET);
      fwrite(&h,HEADER_SIZE,1,fp);

      if(writeRecord(fp,s, h.header_record -1) > 0) 
      return 1;
      else 
      return 0;
   } // �̹� record�� �־� �� ������ append�� �� 
	
}
void searchRecord(FILE *fp, enum FIELD f, char *keyval){

   HEADER h;

   /*
   if((fp = fopen(f_name, "r")) == NULL){
      fprintf(stderr, " fopen error\n");
      exit(1);
   }*/

   fseek(fp, 0, SEEK_SET);
	memset(&h, 0, sizeof(HEADER_SIZE));

	if(fread(&h, HEADER_SIZE, 1, fp) < 0){
      fprintf(stderr,"fread error\n exit(1)");
      exit(1);
   } 

	int rrn = 0; // �ݺ��� ���ư� �� ���� rrn �� �ϳ��� ���� -> rrn �� ��ġ �̵� 
   
   fseek(fp,0,SEEK_END);
   int record_length=ftell(fp); //ftell�� �̿��Ͽ� length ���� ��ġ ȣ��   
   fseek(fp,0,SEEK_SET); // ó�� ��ġ�� �̵�

	/* while ���� �̿��Ͽ� ftell(fp)�� ��ġ�� record_length ���� ��� ���� == record�� �� ��ġ���� while�� ���� */
   while(ftell(fp)<record_length){
      readRecord(fp, s, rrn);
      switch(f){
         case ID:
            if(!strcmp(keyval, s->id))
               printRecord(s);
            break;

         case NAME:
            if(!strcmp(keyval, s->name))
               printRecord(s);
            break;

         case DEPT:
            if(!strcmp(keyval, s->dept))
               printRecord(s);
            break;

         case ADDR:
            if(!strcmp(keyval, s->addr))
               printRecord(s);
            break;

         case EMAIL:
            if(!strcmp(keyval, s->email))
               printRecord(s);
            break;
      }
      rrn++;
   }
   return;
}
enum FIELD getFieldID(char *fieldname){
	
	if(!strcmp(fieldname, "ID"))
		enumfile = ID;
	
	else if(!strcmp(fieldname, "NAME"))
		enumfile = NAME;
	
	else if(!strcmp(fieldname, "DEPT"))
		enumfile = DEPT;
	
	else if(!strcmp(fieldname, "ADDR"))
		enumfile = ADDR;
	
	else if(!strcmp(fieldname, "EMAIL"))
		enumfile = EMAIL;
	
	else{
      fprintf(stderr,"rewrite field_name\n exit(1)\n");
      exit(1);
   }
	
	return enumfile;
} // enum FIELD�� NAME = 1 ����


int main(int argc, char *argv[])
{
   FILE* fp;

    if(access(argv[2], F_OK) == 0 ){
         if((fp = fopen(argv[2], "r+")) < 0 ){
            fprintf(stderr, "open Error\n exit(1)\n");
            exit(1);
         }
         
      }// if file in this programm -> access �Լ��� ����
      
      else{
         if((fp = fopen(argv[2], "a+")) < 0) {
			fprintf(stderr, "open error\n exit(1)\n");
			exit(1);
		   }
      }// file not in this programm

   s = (STUDENT*)malloc(RECORD_SIZE);
   f_name = (char*)malloc(BUFFER_SIZE);

   strcpy(f_name, argv[2]);
   
   if(!strcmp(argv[1], "-a")){
      if(argc != 8){
         fprintf(stderr, "Usage : %s -a record_file_name | field_value_1 | field_value_2 | field_value_3 | field_value_4 | field_value_5 \n exit(1)\n", argv[0]);
         exit(1);
      }
      strcpy(s->id,argv[3]);
		strcpy(s->name,argv[4]);
		strcpy(s->dept,argv[5]);
		strcpy(s->addr,argv[6]);
		strcpy(s->email,argv[7]);

      appendRecord(fp, s->id, s->name, s->dept, s->addr, s->email);
   }

   if(!strcmp(argv[1], "-s")){
      if(argc != 4){
         fprintf(stderr, "Usage : %s -s record_file_name | field_name=field_value\n exit(1)\n", argv[0]);
         exit(1);
      }
      char* field_name; // ID NAME DEPT ADDR EMAIL
      char same_enum; // User enter field_name
      char* keyval; // User enter field_value

      field_name = (char*)malloc(FIELD_SECTOR);
      keyval = (char*)malloc(KEY_VAL);
      
      sscanf(argv[3], "%[^=] %c %[^\n]", field_name, &same_enum, keyval); // %[^\n] -> ���� ���� 
      enumfile = getFieldID(field_name);
      //printf("%s\n",fieldname);
      //printf("%s\n",keyval);
      //printf("%d\n",enumfile);
      searchRecord(fp, enumfile, keyval);

      free(field_name);
      free(keyval);
   }
	free(s);
	free(f_name);
	fclose(fp);
   
	return 0;
}

void printRecord(const STUDENT *s)
{
	printf("%s | %s | %s | %s | %s\n", s->id, s->name, s->dept, s->addr, s->email);
}
