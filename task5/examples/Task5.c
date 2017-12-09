#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <math.h>
#include <sys/ipc.h>
#include <ctime>
#include <sys/stat.h>
#include <sys/msg.h>
#include <string.h>
#define size 255

using namespace std;

int main(int argc, char** argv, char** envp)
{
	time_t time;
	char buf[256];
	int fd1;
	if ((fd1 = open("copy.txt", O_RDWR)) < 0)
	{
		printf("open file1 error\n");
		exit(-1);
	}
	int fd2;
	if ((fd2 = open("paste.txt", O_RDWR | O_CREAT, 0666)) < 0)
        {
                printf("open file2 error\n");
                exit(-1);
        }
	int get = 0; int put = 0;
	time = clock();
	while((get = read(fd1, buf, size)) > 0)
	{
		if (get == -1)
		{
			printf("reading error\n");
			exit(-1);
		}
		put = write(fd2, buf, get);
		if (put != get)
		{
			printf("writing error\n");
			exit(-1);
		}
	}
	time = clock() - time;
	printf("%ld - standart copy time\n", time);
	close(fd2);

	struct stat inf;
	int st;
	if ((st = fstat(fd1, &inf)) == -1)
	{
		printf("stat error\n");
		exit(-1);
	}


	int fd3;
	if ((fd3 = open("copy.txt", O_RDWR | O_CREAT, 0666)) < 0)
	{
			printf("opening file1(mmap) error\n");
			exit(-1);
	}
	time = clock();
	char* map = (char*)mmap(NULL, inf.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd1, 0);
	if (map == MAP_FAILED)
	{
		printf("mmap error\n");
		exit(-1);
	}
	close(fd1);
	
	
	get = write(fd3, map, inf.st_size);
	if (get != inf.st_size)
	{
		printf("writing with mmap error\n");
		exit(-1);
	}
	
	time = clock() - time;
	printf("%ld - mapping copy time\n", time);
	close(fd3);
	munmap((void*)map, inf.st_size);	

	return 0;
}
