#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <signal.h>
#define MAX_BUF_SIZE 512

void error(char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    if (argc < 1) {
      fprintf(stderr,"Enter pid", argv[0]);
      exit(0);
    }
	int pid = atoi(argv[1]);
	while(1){
	//printf("\n pid =%d \n", pid);
	printf("\ncmd> ");
	char buffer[MAX_BUF_SIZE];
	fgets(buffer, MAX_BUF_SIZE, stdin);
	int ret;
//	printf("\nbuffer= %s\n", buffer);
	
	int ln = strlen (buffer);
	if ((ln > 0) && (buffer[ln-1] == '\n')) { 
	//	printf("\nInside if ...\n");
		buffer[ln-1] = '\0';
	}
	
	if(strcmp("terminate",buffer) == 0){
		printf("termination..\n");
		ret = kill(pid, SIGQUIT);
		exit(1);
	} else if(strcmp(buffer,"sleep") ==0){
		printf("sleeping...\n");
		kill(pid, SIGINT);
	} else if(strcmp("infor",buffer) == 0){
		printf("infor..\n");
		ret = kill(pid, SIGRTMAX);
	} else if(strcmp("listuser",buffer)==0){
		printf("listuser...\n");
		ret = kill(pid, SIGRTMIN);
	} else {
		printf("\n INvalid command\n");
	}
  }
	//printf("\nret = %d\n",ret);
    return 0;
}
