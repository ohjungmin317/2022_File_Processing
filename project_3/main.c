﻿#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blkmap.h"

FILE *devicefp;

int main( )
{
	// 아래 변수는 테스트할 때 필요하면 사용하기 바람
	char sectorbuf[SECTOR_SIZE], *blockbuf;
	int i, lsn;

	// flash memory의 파일명은 'flashmemory'을 가정함
	devicefp = fopen("flashmemory", "w+b");	

	if(devicefp == NULL)
	{
		printf("file open error\n");
		exit(1);
	}

	//
	// initialize flash memory where each byte are set to '0xFF'
	// 
	blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
	}

	ftl_open();

	//
	// ftl_write() 및 ftl_read() 테스트를 위한 코드를 자유자재로 만드세요.
	// 채점할 때 이 부분에 테스트 코드를 심어서 합니다. Flash memory에 대한 데이터의 읽기 및 쓰기가
	// 올바르게 동작하는지를 테스트하고, 필요하면 다른 부분도 검사를 합니다.
	//
	/*
	print_addrmaptbl_info();
	strcpy(sectorbuf,"abcdef");
	ftl_write(9,sectorbuf);
	strcpy(sectorbuf,"bcd");
	ftl_write(10,sectorbuf);
	strcpy(sectorbuf,"cdeff");
	ftl_write(100,sectorbuf);
	ftl_read(9,sectorbuf);
	printf("sectorbuf : %s\n",sectorbuf);
	ftl_read(10,sectorbuf);
	printf("sectorbuf :%s\n",sectorbuf);
	ftl_read(100,sectorbuf);
	printf("sectorbuf : %s\n",sectorbuf);
	print_block(0);
	print_block(1);
	print_block(2);
	print_addrmaptbl_info();
	*/

	/*
	char* tmp = "hoonse jung";
	char* wb = (char*)malloc(PAGE_SIZE);
	char* rb = (char*)malloc(PAGE_SIZE);
	strcpy(wb,tmp);
	ftl_write(0, wb);
	ftl_write(4, wb);
	ftl_write(10, wb);
	ftl_write(4, wb);
	ftl_write(16, wb);
	printf("write : %s ---------\n", wb);
	ftl_read(16, wb);
	printf("read : %s ---------\n", rb);
	ftl_write(16, wb);
	print_addrmaptbl_info();
	print_block(0);
	print_block(1);
	print_block(2);
	print_block(3);
	print_block(4);
	print_addrmaptbl_info();


	free(wb);
	free(rb);
	*/

   
   print_addrmaptbl_info();
   strcpy(sectorbuf,"ab");
   ftl_write(9,sectorbuf);
   strcpy(sectorbuf,"fg");
   ftl_write(9,sectorbuf);
   strcpy(sectorbuf,"bc");
   ftl_write(10,sectorbuf);
   strcpy(sectorbuf,"modi");
   ftl_write(10,sectorbuf);
  // strcpy(sectorbuf,"cde");
  // ftl_write(100,sectorbuf);
 //strcpy(sectorbuf,"hi");
   //ftl_write(50,sectorbuf);
   //strcpy(sectorbuf,"ttt");
   //ftl_write(50,sectorbuf);
   //strcpy(sectorbuf,"abcd");
   //ftl_write(160,sectorbuf);
   ftl_read(9,sectorbuf);
   printf("sectorbuf : %s\n",sectorbuf);
   ftl_read(10,sectorbuf);
   printf("sectorbuf : %s\n",sectorbuf);
   ftl_write(50,sectorbuf);
   ftl_write(150,sectorbuf);
   ftl_write(200,sectorbuf);
   /*
   print_block(0);
   print_block(1022);
   print_addrmaptbl_info();
   for(int t=0;t<1200;t++){
      ftl_write(160,sectorbuf);
   }
   ftl_read(160,sectorbuf);
   printf("sectorbuf : %s\n",sectorbuf);
   */

  // ftl_read(100,sectorbuf);
   //printf("sectorbuf : %s\n",sectorbuf);
   //ftl_read(50,sectorbuf);
   //printf("sectorbuf : %s\n",sectorbuf);
   //ftl_read(160,sectorbuf);
   //printf("sectorbuf : %s\n",sectorbuf);
   print_addrmaptbl_info();
   //ftl_read(160,sectorbuf);
   //printf("sectorbuf : %s\n",sectorbuf);




	free(blockbuf);
	fclose(devicefp);

	return 0;
}