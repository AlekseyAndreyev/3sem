#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(){
	int fd1[2], fd2[2], result;
	char resstring[100];
	char string[100];
	char c='p';
	int status;
	
	if(pipe(fd1)<0){
		printf("Can\'t create pipe\n");
		exit(-1);
	}

        if(pipe(fd2)<0){
                printf("Can\'t create pipe\n");
                exit(-1);
        }

	while(c!='y'){
		printf("From?(c=child, p=parent)\n");
		//scanf("%c",&c);
		cin>>c;
		result = fork();
		if(result<0){
			printf("Can\'t fork\n");
			exit(-1);
		}else if (result > 0){
			if(c=='p'){
				scanf("%s", string);	
				write(fd1[1], string, 20);
			}else{
				read(fd2[0], resstring, 20);
                                printf("Parent: %s\n", resstring);
                        }
		}else{
			if(c=='p'){
				read(fd1[0], resstring, 20);
				printf("Child: %s\n", resstring);
			}else{
			        scanf("%s", string);
                                write(fd2[1], string, 20);
			}
			exit(0);
		}
		wait(&status);
		printf("exit?(y/n)\n");
		//scanf("%c", &c);
		cin>>c;
	}
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
        close(fd2[1]);
	return 0;
}
