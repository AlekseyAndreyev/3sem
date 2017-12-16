#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>


int main()
{
	int pid1,pid2,pid3;
	pid1 = fork();
	if (pid1 == 0)
	{
		FILE *f = fopen("pid.txt", "w");
		fprintf(f,"%d", getpid());
		fclose(f);
		int status;
		umask(0);
		setsid();
		chdir("/");
		close(STDIN_FILENO);
		//close(STDOUT_FILENO);
		close(STDERR_FILENO);
		pid2 = fork();
		if (pid2 == 0)
		{
			execlp("cp", "cp", "-r", "home/a/dz/test", "home/a/dz/dir", NULL);
			exit(0);
		}
		else
		{
			if (pid2 < 0)
			{
				printf("fork error");
				exit(-1);
			}
			wait(&status);
		}
		while(1)
		{
			pid2 = fork();
			if (pid2 == 0)
			{
				printf("====================================================\n");
				execlp("diff", "diff", "-r","home/a/dz/dir/test", "home/a/dz/test", '>', "home/a/dz/changes.diff", NULL);
				execlp("diff", "diff", "-r","home/a/dz/dir/test", "home/a/dz/test", NULL);
				exit(0);
			}
			else
			{
				if (pid2 < 0)
				{
					printf("fork error");
					exit(-1);
				}
				wait(&status);	
			}
			pid3 = fork();
			if (pid3 == 0)
			{
				execlp("cp", "cp", "-r","home/a/dz/test", "home/a/dz/dir", NULL);
				exit(0);
			}
			else
			{
				if(pid3 < 0)
				{
					printf("fork error");
					exit(-1);
				}
				wait(&status);
			}
			sleep(30);
		}
	}
	else
	{
		if (pid1 < 0)
		{
			printf("fork error");
		}
		return 0;
	}

	return 0;
}
