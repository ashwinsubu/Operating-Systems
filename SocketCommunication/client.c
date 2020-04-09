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

void error(char *msg)
{
    perror(msg);
    exit(0);
}
int MAX_BUF_SIZE = 512;
int main(int argc, char *argv[])
{
    int sockfd, portno, ret;
    struct sockaddr_in serv_addr;
    struct hostent *server;
 
	 
	
    if (argc < 4) {
      fprintf(stderr,"usage %s hostname port\n", argv[1]);
      exit(0);
    }
	char *userid = argv[1];
	//printf("\nargc= %d, arg0 %s, arg1=%s, arg2= %s, arg3= %s\n",argc, argv[0],argv[1],argv[2],argv[3]);
	//printf("\nuserid= %s\n",userid);
    portno = atoi(argv[3]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    server = gethostbyname(argv[2]);
    if (server == NULL) error ("ERROR, no such host");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
      error("ERROR connecting");
	
	//write userid to server
	//printf("\n userid = %s, sizeof(userid)= %d", userid, (int)sizeof(userid));
	if(write(sockfd, userid, (int)sizeof(userid)) < 1){
		printf("\n Error while writing userid to server...");
	}
	
	
	while(1){
		if(ret >1){
			printf ("ucmd> ");
			char buffer[512]={0};
			bzero(buffer, MAX_BUF_SIZE);
			fgets(buffer, (int)sizeof(buffer), stdin);
			buffer[strlen(buffer)-1]='\0';
			//printf("\ninput = tttt%stttt\n",buffer);
			if(strcmp(buffer,"logout") == 0){
				//printf("\nthe userid is = %s\n", userid);
				strncat(buffer, " ", 1);
				strncat(buffer, userid, strlen(userid));
				//printf("\nthe logout buffer is %s\n", buffer);
				printf("User %s socket %d has been closed, process terminating\n", userid, sockfd);
				write(sockfd, buffer, (int)sizeof(buffer));
				exit(1);
				break;
			}
			
			//printf("\nthe buff is %s\n", buffer);
			//printf("\n buffer size is= %d\n", (int)sizeof(buffer));
			ret = write(sockfd, buffer, (int)sizeof(buffer));
			if (ret < 0) error ("ERROR writing to socket");
			//printf("\n write ret = %d\n", ret);
		}
		char read_buffer[512];
		
		ret = read(sockfd, read_buffer, MAX_BUF_SIZE-1);
		if (ret < 0) error ("\nERROR reading from socket");
		if(ret == 0) printf("\nPeer disconnected...\n");
		//printf("\n read ret = %d\n", ret);
		printf ("\n====Server Response===== \n%s\n",read_buffer);
		//printf("\n sizeof(buf) = %d, and strlen(buf)= %d\n", sizeof(read_buffer), strlen(read_buffer));
	}
    return 0;
}
