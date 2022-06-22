#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<math.h>

#define BUFFER_SIZE 999

int main(int argc, char *argv[]){
	int fd;
	int length;
	char buffer[BUFFER_SIZE];
	off_t fsize; 

	int offset = atoi(argv[1]); 
	int read_byte = atoi(argv[2]); 

	if(argc != 4){ 
		fprintf(stderr, "Usage : %s <offset> <read_byte> <filename>\nexit(1)\n",argv[0]);
		exit(1);
	}

	if((fd = open(argv[3], O_RDWR)) < 0){ 
		fprintf(stderr, "Open Error for %s\n exit(1)\n", argv[3]);
		exit(1);
	}
	
	if((fsize = lseek(fd, 0, SEEK_SET)) <0){ 
		fprintf(stderr,"lseek Error\n Programm exit(1)");
		exit(1);
	} 

	if(read_byte < 0){ 

	if((lseek(fd, offset+read_byte, SEEK_CUR)) < 0){
		fprintf(stderr, "lseek Error\nexit(1)\n");
		exit(1);
		}
	}
	else{

	if((lseek(fd, offset+1, SEEK_CUR)) < 0){ 
		fprintf(stderr, "lseek Error\n exit(1)\n");
		exit(1);
		}
	}

	read_byte = abs(read_byte);


	if((length = read(fd, buffer, read_byte)) < 0){ 
		fprintf(stderr, "Read error\n Program exit(1)\n");
		exit(1);
	}
	
	if(length > 0)	{

		if((write(1, buffer, length))<0){ 
			fprintf(stderr,"Write Error\nProgram exit(1)\n");
			exit(1);
		}
		printf("\n");
	}

	if(length == 0){  
		if((write(1, buffer, fsize))<0){
			fprintf(stderr,"Write Error\nProgram exit(1)");
			exit(1);
		} 
		printf("\n");
	}
	close(fd);
	return 0;
}