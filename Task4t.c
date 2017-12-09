#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <math.h>
#include <sys/ipc.h>
#include <ctime>
#include <sys/msg.h>
#include <string.h>
#define idf 3

using namespace std;

struct st
{
	long type;
	char buf[idf];
};

int main(int argc, char** argv, char** envp)
{
	int file;
	if ((file = open("copy.txt", O_RDONLY)) < 0)
	{
		printf("file error");
		exit(-1);
	}
	
	size_t key = ftok("Task4t.c", 0);
	if (key < 0)
	{
		printf("ftok error");
		close(file);
		exit(-1);
	}

	int msg = msgget(key, 0666|IPC_CREAT);
	if (msg < 0)
	{
		printf("msgget error");
		close(file);
		exit(-1);
	}

	int rd;
	int snd;
	st str;
	str.type = 5;
	struct stat f;
	fstat(file, &f);
	int n = f.st_size;
	while ((rd = read(file, str.buf, idf)) > 0)
	{	
		n=n-rd;
		if(n==0)
			str.type=rd;
		//write(1, str.buf, rd);
		if ((snd = msgsnd(msg, &str, rd, 0) != 0))
		{
			printf("msgsnd error");
			close(file);
			msgctl(msg, IPC_RMID, NULL);		
		}
		else
		{
			rd = 0;
		}
	}
	return 0;
}
