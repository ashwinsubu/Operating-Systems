#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

main() {
	 printf("\nShsh process %d has been created by the process %d\n", getpid(), getppid());
  do{
  char str1[200];
 
  printf("cmd> ");
	gets(str1);	  
	char subcommand[4];
	char str2[200];
	strcpy(str2, str1);
	memcpy( subcommand, &str1[0], 3 );
	subcommand[4] = '\0';
	int len = strlen(str2);
	printf("\nShsh processing %s\n", str2);
	if(strcmp("cmd",subcommand) == 0){
		char syscommand[len];
	    memcpy( syscommand, &str2[4], len-3);	
		syscommand[len]='\0';	
		system(syscommand);
	} else if(strcmp("pip",subcommand) == 0){
	 char sub[1000];
	 substring(str2, sub, 5, strlen(str2)-4);
	 int noOfCommands = getNoOfCommands(sub)+1;
	// printf("\nNo of commands is: %d\n",noOfCommands);
      //  printf("sub == %s\n", sub);
		//printf("str2 == %s\n", str2);
	 char *p = strtok (sub, ";");
     char **array[100];
	int i=0;
	while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, ";");
    }
	/*printf("\nPrinting separated pipe cmds ::\n");
    for (i = 0; i < noOfCommands; i++)
        printf("%s\n", array[i]);
	*/
	pipeline(array, noOfCommands);
	} else if(strcmp("exi",subcommand) == 0){
		printf("\nShsh process %d terminates\n", getpid());
		exit(1);
	} else {
		printf("\nInvalid input\n");
	}
  }while(1);
}

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}


void pipeline(char ***str, int len)
{
//	printf("\npipeline str:  %s\n", *str);
	int fd[2];
	pid_t pid;
	int fdd = 0;
	int i=0;
	while (i<len) {
		pipe(fd);
		pid = fork();
		if (pid == 0) {
			printf("\nShsh forked %d for %s, with in-pipe %d and out-pipe %d\n", getpid(), *(str+i), fd[0], fd[1]);
			dup2(fdd, 0);
			if (i+1<len) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			system(*(str+i));
			exit(1);
		} else {
			wait(NULL); 
			close(fd[1]);
			fdd = fd[0];
			i+=1;
		}
	}
}

void displayString(char ***str, int len){
	printf("\ninitial len of str is: %d\n",len);
	printf("\n in dispString::: %s\n", *(str+1));
	int i=0;
	while (i<len) {
		printf("\nstr = %s\n", *(str+i));
		i++;
	}
	printf("\nlen of str is: %d\n",i);
}

int getNoOfCommands( char sub[]){
	int i=0,count=0;
	while(sub[i] != '\0'){
		if(sub[i++] == ';'){
			count++;
		}
	}
	return count;
}



