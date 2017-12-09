#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <math.h>
#include <sys/ipc.h>
#include <ctime>

using namespace std;

struct sq
{
	double a1;
	double a2;
	double b1;
	double b2;
	long long int m;
	long long int n;
	int* arr;
};

int function(double x, double y)
{
	
	if (y > cos(x))
		return -1;
	else
		return 1;
}

void* calc(void* arg)
{
	struct sq A = *((struct sq*)arg);
	int* ar = A.arr;
	long long int s = A.n * A.m;
	for (long long int i = 0; i < A.n; i++)
	{
		ar[s+i] = function(A.a1+((double)rand()/RAND_MAX)*(A.a2 - A.a1), A.b1 + ((double)rand()/RAND_MAX)*(A.b2- A.b1));
	}
	return NULL;
}

int main(int argc, char** argv, char** envp)
{
	srand(time(NULL));
	int shmid;
	key_t key;
	long long int n;
	long long int m;
	int st;
	struct sq arg;
	printf("Points: ");
	cin >> n;
	printf("Threads: ");
	cin >> m;

	arg.a1 = -1.57;
        arg.a2 = 1.57;
        arg.b1 = 0;
        arg.b2 = 1;

	int* arr;
	int status;
	char path[] = "Task3.c";
	pthread_t th[100];
	pid_t pid = fork();
	if (pid == -1)
	{
		printf("fork error\n");
		return 0;
	}
	else
	{
		sq* str = (sq*)malloc(m*sizeof(sq));
		if(pid == 0)
		{	
			if ((key = ftok(path, 0)) < 0)
			{
				printf("ftok error\n");
				free(str);
				exit(-1);
			}
			if((shmid = shmget(key, n*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0)
			{
				if (errno != EEXIST)
				{
					printf("can't create shared memory\n");
					free(str);
					exit(-1);
				}
				else
				{
					if((shmid = shmget(key, n*sizeof(int), 0)) < 0)
					{
						st = shmctl(shmid, IPC_RMID, NULL);
						printf("can't find shared memory\n");
						free(str);
						exit(-1);
					}
				}
			}
			int check;
			if ((arr = (int*)shmat(shmid, NULL, 0)) == (int*)(-1))
			{
				printf("Can't attach shared memory\n");
				free(str);
				exit(-1);
			}
			arg.arr = arr;
			arg.n = (long long int)(n/m);
			for(long long int i = 0; i < m; i++)
			{
				str[i] = arg;
			}
			for (long long int i = 0; i < m; i++)
			{
				(str[i]).m = (long long int)i;
				check = pthread_create(&th[i], NULL, calc, (void*)&str[i]);
				if(check != 0)
				{
					printf("thread create error\n");
					free(str);
					exit(-1);
				}
			}
			for (long long int i = 0; i < m; i++)
			{
				pthread_join(th[i], NULL);
			}
			free(str);
			exit (0);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (status == -1)
			{
				cout << "child error\n";
				return -1;
			}
			
			if ((key = ftok(path, 0)) < 0)
            {
				printf("ftok error\n");
                return -1;
            }
            if((shmid = shmget(key, n*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0)
            {
                if (errno != EEXIST)
				{
					printf("can't create shared memory\n");
					return -1;
				}
				else
				{
					if((shmid = shmget(key, n*sizeof(int), 0)) < 0)
					{
						st = shmctl(shmid, IPC_RMID, NULL);
						printf("can't find shared memory\n");
						return -1;
					}
				}
			}

			if ((arr = (int*)shmat(shmid, NULL, 0)) == (int*)(-1))
			{
					printf("error");
					st = shmctl(shmid, IPC_RMID, NULL);
					return -1;
			}
			double plus = 0;
			double minus = 0;
			for (long long int i = 0; i < n; i++)
			{
				if(arr[i] == 1)
				{
					plus++;
				}
				else
				{
					if(arr[i] == -1)
					{
						minus++;
					}
				}
				
			}
			double result = plus/(plus+minus)*(arg.b2 - arg.b1)*(arg.a2 - arg.a1);
			printf("integral: %lf\n", result);
			st = shmctl(shmid, IPC_RMID, NULL);
			if (st == 0)
			printf("memory cleand\n");
		}
	}
	return 0;
}
