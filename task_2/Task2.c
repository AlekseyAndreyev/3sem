#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

using namespace std;

int main(){
	int fd1[2], fd2[2], result;
	char resstring[100];
	char string[100];
	char c='p';
	int status;
	int size=0;
	
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
				while( (size = read(0, string, sizeof(string)-1)) > 0)
				{
					string[size] = '\0';
					
					write(fd1[1], string, size+1);
					if(strcmp("end\n", string) == 0)
						break;
				}
				//scanf("%s", string);	
				//write(fd1[1], string, 20);
			}else{
				while((size = read(fd2[0], resstring, sizeof(resstring)-1)) > 0)
				{
					if(strcmp("end\n", resstring) == 0)
						break;
					printf("Parent: %s", resstring);
				}
				//read(fd2[0], resstring, 20);
                                //printf("Parent: %s\n", resstring);
                }
		}else{
			if(c=='p'){
				while((size = read(fd1[0], resstring, sizeof(resstring)-1)) > 0)
				{
					if(strcmp("end\n", resstring) == 0)
						break;
					printf("Child: %s", resstring);
				}
				//read(fd1[0], resstring, 20);
				//printf("Child: %s\n", resstring);
			}else{
				while( (size = read(0, string, sizeof(string)-1)) > 0)
				{
					string[size] = '\0';
					
					write(fd2[1], string, size+1);
					if(strcmp("end\n", string) == 0)
						break;
				}
			    //scanf("%s", string);
                //write(fd2[1], string, 20);
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
