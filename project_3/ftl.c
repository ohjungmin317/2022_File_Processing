#define PRINT_FOR_DEBUG

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"

AddrMapTbl addrmaptbl; // maptable �߰�
extern FILE *devicefp;

//SpareData spare[PAGES_PER_BLOCK];

int reserved_empty_blk = DATABLKS_PER_DEVICE;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);
void print_block(int pbn);
void print_addrmaptbl_info();

/*
typedef struct page {
    char sector[SECTOR_SIZE];
    char str_spare[SPARE_SIZE];
    int spare;
} page;
*/

//
// flash memory�� ó�� ����� �� �ʿ��� �ʱ�ȭ �۾�, ���� ��� address mapping table�� ����
// �ʱ�ȭ ���� �۾��� �����Ѵ�
//
void ftl_open()
{
	int i;
	 // initialize the address mapping table
   for(i = 0; i < DATABLKS_PER_DEVICE; i++)
   {
      addrmaptbl.pbn[i] = -1;
   }

   addrmaptbl.pbn[DATABLKS_PER_DEVICE] = reserved_empty_blk; // �ʱⰪ ����


	//
	// �߰������� �ʿ��� �۾��� ������ �����ϸ� �ǰ� ������ ���ص� ������
	//

	return;
}

//
// file system�� ���� FTL�� �����ϴ� write interface
// 'sectorbuf'�� ����Ű�� �޸��� ũ��� 'SECTOR_SIZE'�̸�, ȣ���ϴ� �ʿ��� �̸� �޸𸮸� �Ҵ�޾ƾ� ��
//
void ftl_write(int lsn, char *sectorbuf)
{  char *page_buffer;
   page_buffer = (char*)malloc(PAGE_SIZE);
   memset(page_buffer, 0xFF, PAGE_SIZE);

   SpareData *spare_buffer;
   spare_buffer = (SpareData*)malloc(SPARE_SIZE);
   memset(spare_buffer, 0xFF, SPARE_SIZE);

   int lbn;
	int offset;
	int pbn;
	int ppn;
   
	lbn = lsn / PAGES_PER_BLOCK;
	offset = lsn % PAGES_PER_BLOCK;
   /*
	for(i=0;i<DATABLKS_PER_DEVICE;i++){
		if(addrmaptbl.pbn[i]==lbn)
			break;
	}*/

   //pbn = addrmaptbl.pbn[lbn];
   //ppn = (pbn * PAGES_PER_BLOCK) +  offset;


  if(addrmaptbl.pbn[lbn] != -1) { 
      pbn = addrmaptbl.pbn[lbn];
      ppn = (pbn * PAGES_PER_BLOCK) + offset;
      if(dd_read((pbn * PAGES_PER_BLOCK) + offset, page_buffer) == 1) {
         memcpy(spare_buffer, &page_buffer[SECTOR_SIZE], SPARE_SIZE);
         memcpy(page_buffer, sectorbuf, SECTOR_SIZE);

         /*
         if(spare_buffer->lsn == -1)
         dd_write(ppn,sectorbuf);
         */
        
         //overwrite 
         if(spare_buffer->lsn >= 0) {
            dd_write(reserved_empty_blk * PAGES_PER_BLOCK + offset, page_buffer);
            for(int i = 0; i < PAGES_PER_BLOCK; i++) {
               if(i == offset) 
               continue;
                  dd_read((pbn * PAGES_PER_BLOCK) + i, page_buffer);
                  dd_write((reserved_empty_blk * PAGES_PER_BLOCK) + i, page_buffer);
            }
            addrmaptbl.pbn[lbn] = reserved_empty_blk;
            reserved_empty_blk = pbn;
            addrmaptbl.pbn[DATABLKS_PER_DEVICE] = reserved_empty_blk;
            dd_erase(reserved_empty_blk);
         }
         else { 
            memcpy(&page_buffer[SECTOR_SIZE], &lsn, sizeof(int)); 
            dd_write(ppn, page_buffer);
         }
      }
   }
   else { 
      int n_block;
      int n_next_pbn[BLOCKS_PER_DEVICE];
      for(n_block = 0; n_block < BLOCKS_PER_DEVICE; n_block++) {
         for(int i = 0; i < BLOCKS_PER_DEVICE; i++) {
            if(n_block == addrmaptbl.pbn[i]) {
               n_next_pbn[n_block] = TRUE;
               break;
            }
            else n_next_pbn[n_block] = FALSE;
         }
      }

      for(n_block = 0; n_block < BLOCKS_PER_DEVICE; n_block++) {
         if(n_next_pbn[n_block] == 0) {
            pbn = n_block;
            break;
         }
      } 

      ppn = (pbn * PAGES_PER_BLOCK);

      addrmaptbl.pbn[lbn] = pbn;

      memcpy(page_buffer, sectorbuf, SECTOR_SIZE);
      memcpy(spare_buffer, &lsn, sizeof(int));
      memcpy(&page_buffer[SECTOR_SIZE], spare_buffer, SPARE_SIZE);
      dd_write((pbn * PAGES_PER_BLOCK) + offset, page_buffer);
   }

   free(page_buffer);
   free(spare_buffer);



#ifdef PRINT_FOR_DEBUG
   print_addrmaptbl_info();
#endif



   return;

  
}

//
// file system�� ���� FTL�� �����ϴ� read interface
// 'sectorbuf'�� ����Ű�� �޸��� ũ��� 'SECTOR_SIZE'�̸�, ȣ���ϴ� �ʿ��� �̸� �޸𸮸� �Ҵ�޾ƾ� ��
// 

void ftl_read(int lsn, char *sectorbuf)
{
   int lbn;
   int offset;
   char *page_buffer;
   
   lbn = lsn / PAGES_PER_BLOCK;
   offset =  lsn % PAGES_PER_BLOCK;

   //addrmaptbl.pbn[lbn] = pbn
   //(addrmaptbl.pbn[lbn] * PAGES_PER_BLOCK) + offset = ppn
   
   page_buffer = (char*)malloc(PAGE_SIZE);
   

   if(addrmaptbl.pbn[lbn]  == -1 ) {
      fprintf(stderr,"Read Error\n");
      return;
   }
   
   if(dd_read((addrmaptbl.pbn[lbn] * PAGES_PER_BLOCK) + offset, page_buffer) == 1) {
      memcpy(sectorbuf, page_buffer, SECTOR_SIZE);
   }   
   free(page_buffer);

   
	#ifdef PRINT_FOR_DEBUG			// �ʿ� �� ������ block mapping table�� ����� �� �� ����
	print_addrmaptbl_info();
	#endif
   


	return;
}

//
// for debugging
//

void print_block(int pbn)
{
	char *pagebuf;
	SpareData *sdata;
	int i;
	
	pagebuf = (char *)malloc(PAGE_SIZE);
	sdata = (SpareData *)malloc(SPARE_SIZE);

	printf("Physical Block Number: %d\n", pbn);

	for(i = pbn*PAGES_PER_BLOCK; i < (pbn+1)*PAGES_PER_BLOCK; i++)
	{
		dd_read(i, pagebuf);
		memcpy(sdata, pagebuf+SECTOR_SIZE, SPARE_SIZE);
		printf("\t   %5d-[%7d]\n", i, sdata->lsn);
	}

	free(pagebuf);
	free(sdata);

	return;
}

//
// for debugging
//
void print_addrmaptbl_info()
{
	int i;

	printf("Address Mapping Table: \n");
	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
	{
		if(addrmaptbl.pbn[i]>= 0)
		{
			printf("[%d %d]\n", i, addrmaptbl.pbn[i]);
		}
	}
}
