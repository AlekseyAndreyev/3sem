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
	if ((file = open("paste.txt", O_WRONLY | O_CREAT, 0666)) < 0)
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

	int rcv;
	int wr;
	int ch = -1;
	st str;
	str.type = 5;
	while ((rcv = msgrcv(msg, &str, idf, 0, 0)) > 0)
	{
		if(str.type !=5)
			ch = str.type;

		if (ch >= 0)
		{
			//write(1, str.buf, ch);
			if ((wr = write(file, str.buf, ch)) < 0)
			{
				printf("write error");
				close(file);
				msgctl(msg, IPC_RMID, NULL);
				exit(-1);
			}
			msgctl(msg, IPC_RMID, NULL);
		}
		else
		{
			//write(1, str.buf, rcv);
			if ((wr = write(file, str.buf, rcv)) < 0)
			{
				printf("write error");
				close(file);
				msgctl(msg, IPC_RMID, NULL);
				exit(-1);
			}
			else
			{
				rcv = 0;
			}
		}
	}
	
	return 0;
}
