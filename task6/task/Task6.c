#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
 

pid_t ppid;
int fd1, fd2;
char sym;
int pexit;
int nsym;
 
void cldhandler(int nsig) {
	close(fd2);
	pexit=1;
}
 

void handler (int nsig) {
 	pid_t pid0;
 	pid0 = getpid();
 	if ((nsig == SIGUSR1) && (pid0 == ppid)) {
 			int k = 1,i;
			for(i=0;i<nsym;i++)
				k = k<<1;
			char c = k;
 			sym += c;
 	} 
}
 	
 
int main() {
 	signal (SIGCHLD, cldhandler);
 	signal(SIGUSR1, handler);
 	signal(SIGUSR2, handler);
  	ppid = getpid();
	
	
	
	char mask = 1;

 	fd2 = open("paste.txt", O_CREAT|O_RDWR|O_TRUNC, 0666);
 	if (fd2 < 0) {
 		printf("open error\n");
 		exit(-1);
 	}
 	fd1 = open("copy.txt", O_RDONLY);
 	if (fd1 < 0) {
 		printf("open error\n");
 		exit(-1);
 	}

	sigset_t set1, set2;
  	sigemptyset(&set1);
  	sigemptyset(&set2);
 	sigaddset(&set1, SIGUSR1);
 	sigaddset(&set1, SIGUSR2); 	
 
	pid_t pid;
 	pid = fork();
 	sigprocmask(SIG_SETMASK, &set1, NULL);
 	if (pid > 0) {
 		sym = 0;
		nsym = 0;
		while (pexit == 0) {
			sigsuspend(&set2);
			sigprocmask(SIG_SETMASK, &set1, NULL);
			nsym++;
			if (nsym == 8) {
				write(fd2, (void*)&sym, 1);
				nsym = 0;
				sym = 0;
			}
			kill(pid, SIGUSR1);
		}
		close(fd2);
 	} else {
		char c;
 		int n;
		int i=0;
 		while ((n = read(fd1, (void*)&c, 1)) > 0) {
 			for(i = 0; i < 8; i++) {
 				sigprocmask(SIG_SETMASK, &set1, NULL);
 				if ((c & mask) == 1) {
 					kill(ppid, SIGUSR1);
 				} else {
 					kill(ppid, SIGUSR2);
 				}
 				c = c >> 1;
 				sigsuspend(&set2);
 			}
 			
 		}
 		close(fd1);
 	} 
	exit(-1);
 	return 0;
}
