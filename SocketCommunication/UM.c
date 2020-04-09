/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#define MAX 512
#define MAX_USERS 20

int usersize=-1;
struct User{
	char *userid;
	int sockfd;
	int isLoggedIn;
	int pid;
	pthread_t tid;
}user[MAX_USERS];
int TIME=5;
int PID=0;

int random_filename;

void *socketThread(int* pclient);
void slp();
void terminate();
void listuser();
void infor();

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int MAX_BUF_SIZE = 512;
char buffer[512];
int portno;



int main (int argc, char *argv[])
{
	PID= getpid();
	
	printf("\n========= PID ======== %d, use this pid to start admin.exe\n", getpid());


		if(signal(SIGQUIT, terminate) == SIG_ERR){
			printf("\n  Error during sigquit\n");
		}
		if(signal(SIGRTMIN, listuser) == SIG_ERR){
			printf("\n  Error during sigrtmin\n");
		}
		if(signal(SIGRTMAX, infor) == SIG_ERR){
			printf("\n  Error during sigrtmax\n");
		}
		if(signal(SIGINT, slp) == SIG_ERR){
			printf("\n  Error during sigint\n");
		}
	 int sockfd, newsockfd, clilen;
     
     struct sockaddr_in serv_addr, cli_addr;
     int ret;

     if (argc < 2)
     { fprintf (stderr, "ERROR, no port provided\n");
       exit(1);
     }
	 if (argc < 3)
     { fprintf (stderr, "ERROR, no TIME provided\n");
       exit(1);
     }
	 initUserStruct();
	 TIME= atoi(argv[2]);
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
       error("ERROR opening socket");
     bzero ((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
       error("ERROR binding");

     listen(sockfd,10);
     
	 
	 pthread_t tid[10];
	 int i=0;
	 clilen = sizeof(cli_addr);
	 
	
	 //accepting new clients
	while(newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)){
		
		 
		usersize++;
		user[usersize].sockfd = newsockfd;
		
		
		//printf("\n newsockfd = %d\n", newsockfd);
		int *pclient =  malloc(sizeof(int));
		*pclient = newsockfd;
	if (newsockfd < 0)
       error ("ERROR accepting");
	else if(newsockfd == 0){
		continue;
	}
     else {
		 //printf("Accept client socket %d, %d\n", newsockfd, (int)cli_addr.sin_port);
		 printf("\nUM Server socket ready\n");
	 }
     bzero(buffer,MAX_BUF_SIZE);
	 if(newsockfd > 0){
		//printf("\n before pthreadcreate tid[i] = %d\n", tid[i]);
		
		 if(pthread_create(&tid[i++], NULL, socketThread, pclient) != 0){	//creating new thread for each client
			printf("\nFAILED CONNECT\n");
		 }
		 
		// printf("\n after pthreadcreate tid[i+1] = %d\n", tid[i]);
		 //pthread_join(tid[i],NULL);
		 //printf("\n i = %d\n", i);
	 }
	
	// pthread_join(tid[i++], NULL);
	 newsockfd = 0;
	// printf("\nEND OF while LOOP\n");
	}
     return 0; 
}

void *socketThread(int* arg) {
	
  int pid = fork();
  //if(pid > 0){
	//user[usersize].pid=pid;
//  }
  if (pid > 0) {
	int ns = *arg;
	user[usersize].pid=pid;
	free(arg);
	//printf("\nHI\n");
	//get userid from client and store
	if(!user[usersize].isLoggedIn){
		//printf("\n...storing userid...\n");
		user[usersize].isLoggedIn = 1;
		char buf[100];
		if(read(ns, buf, 100-1)>1){
			//printf("\n userid buf == %s\n", buf);
			user[usersize].userid = (char *)malloc(strlen(buf)+1);
			strcpy(user[usersize].userid, buf);
			//printf("\nCopied userID in struct = %s, for usersize = %d\n", user[usersize].userid, usersize);
			
			user[usersize].isLoggedIn = 1;
			//printf("\nisUserLoggedIn? = %d\n", user[usersize].isLoggedIn );
			//printf("\npthread_self = %d\n", pthread_self());
			
			user[usersize].tid = pthread_self();
			//printf("user.tid = %d\n",user[usersize].tid);
			
			char dirname[15]="";
			strncat(dirname, user[usersize].userid, strlen(user[usersize].userid));
			strncat(dirname, ".dir",4);
			//printf("\ndirname = %s\n", dirname);
			int mk = mkdir(dirname, 0777);
			//chdir(dirname);
			printf("\nNew user %s logs in, thread created, socket %d with port %d is used\n", user[usersize].userid, user[usersize].sockfd, portno);		
		}

	}
		
	
	// printf("\n newsockfd = %d\n", ns);
	int ret;
	 while(ret = read(ns, buffer, MAX_BUF_SIZE-1)){
	//	 printf("\n buffer size is= %d", (int)sizeof(buffer));
		if (ret < 0) error ("ERROR reading from socket");
		//printf("\n +++ read ret = %d +++\n", ret);
		if(ret > 1) {
		
		int currUserID=0;
		int j;
		//printf("\n current req pid= %d, n threadID= %d\n",getpid(),pthread_self());
		for(j=0;j<MAX_USERS;j++){
			
			if(pthread_equal(user[j].tid, pthread_self())){
				currUserID = j;
				//printf("THread ID matched for userid= %s\n", user[j].userid);
				break;
			}
		} 
		char dirname[15]="";
			strncat(dirname, user[currUserID].userid, strlen(user[currUserID].userid));
			strncat(dirname, ".dir",4);
			//printf("\ndirname = %s\n", dirname);
			chdir(dirname);
	
		 shsh(buffer);
		 
	
		
		//read system command's output from temp.out
		char *buf = malloc(sizeof(char) * MAX);
		char *rez = malloc(sizeof(char) * MAX);
		bzero(buffer, MAX_BUF_SIZE);
		bzero(buf,MAX_BUF_SIZE);
		bzero(rez,MAX_BUF_SIZE);
		 FILE *f1;
		 if((f1=fopen("temp.out", "r" )) == NULL){
				printf("Error\n");
				exit(2);
		 }
		 system("cp temp.out temp1.out");
		 int i = 0;
		while((buf[i] = fgetc(f1)) != EOF){
			rez[i] = buf[i];
			i++;
			if(i + 1 >= MAX){
				break;
			}
		}
		
		
		chdir("..");

		rez[i] = '\0';
		buf[i] = '\0';
		///////
		 strcpy(buffer,rez);
		 free(rez);
		 free(buf);
		 //printf ("\nHere is the message: %s\n", buffer);
		 //printf ("\n sizeof(buffer)= %d, and strlen(buffer)= %d\n", sizeof(buffer), strlen(buffer));
		 ret = write(ns, buffer, (int)sizeof(buffer));
		 //printf("\n +++ write ret = %d +++\n", ret);
		 bzero(buffer, MAX_BUF_SIZE);
		 if (ret < 0) error ("ERROR writing to socket");
		 if(ret == 0) printf("\nret = 0\n");
		 ret=-1;
		}
	 }
	 
   } else {
		wait(NULL);
   }
}

void shsh(char str1[]){
////////////////////////////

	//printf("\n srt1 is= %s\n", str1);
char subcommand[4];
	char str2[512];
	strcpy(str2, str1);
	//printf("\n str2 is= %s\n", str2);
	
	//////-------------------
	char *p = strtok(str1, " ");
	char *array[100];
	int i=0;
	while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, " ");
    }
	char *command = array[0];
	//printf("\n*command = %s, and len(command) = %d\n", command, strlen(command));
	
	if(compare_string("cmd",command) == 0){
			int startLen = strlen(command) + 1;
			int commandLen = strlen(str2);
			int len = commandLen - startLen;
			char syscommand[len];
			memcpy(syscommand, &str2[startLen], len);
			
			int compPipe = compare_string("pipe",command);
			//printf("\ncompare pipe and given command return = %d\n", compPipe);
			
			syscommand[len]='\0';
			//printf("\nsyscommand = %s and len(syscommand) = %d\n", syscommand, strlen(syscommand));
			
			char filename[100]="";
			strncat(filename, syscommand, strlen(syscommand));
			strncat(filename, ">temp.out", 9);
			//printf("\nFinal command = %s\n", filename);
			system(filename);
	} else if(compare_string("pipe",command) == 0){
			//printf("\nIn pipe command \n");
			int startLen = 5;
			int commandLen = strlen(str2);
			int len = commandLen - startLen+1;
			char syscommand[len];
			memcpy(syscommand, &str2[startLen], len);	
			syscommand[len]='\0';
			//printf("\nsyscommand = %s\n", syscommand);
				
			 int noOfCommands = getNoOfCommands(syscommand)+1;
			// printf("\nNo of commands is: %d\n",noOfCommands);
			 //printf("syscommand == %s\n", syscommand);
			 //printf("str2 == %s\n", str2);
			 char *p = strtok (syscommand, ";");
			 char **array[100];
			int i=0;
			while (p != NULL)
			{
				array[i++] = p;
				p = strtok(NULL, ";");
			}
			char pp[13]="cat>temp.out";
			array[i++] = pp;
			//printf("\nThe split commands are....\n");
			//for (i = 0; i < noOfCommands+1; i++)
			//	printf("%s\n", array[i]);
			
			
				
			pipeline(array, noOfCommands+1);
	} else if(compare_string("logout",command) == 0){
		//printf("\nIN logout condition....\n");
				char userid[20];
				strcpy(userid, array[1]);
				//printf("\n userid to logout is= %s\n", userid);
				//printf("userid in struct to logout is= %s, and usersize (or i=) = %d\n", user[usersize].userid, usersize);
				
				for(int i=0;i<MAX_USERS;i++){
					
					if(strcmp(userid,user[i].userid)==0 || compare_string(userid, user[i].userid)==0){
						//printf("\nUser id matched == %s, i= %d\n", user[i].userid, i);
						user[i].isLoggedIn = 0;
						//printf("\n user.tid == %d\n", user[i].tid);
						//printf("\n pthread_self == %d\n", pthread_self());
						//printf("\n getpid() == %d\n", getpid());
						//printf("\n userpid == %d\n", user[i].pid);
						
						if(pthread_equal(user[i].tid, pthread_self())){
							printf("User %s logs out, socket %d has been closed, thread terminating", user[i].userid, user[i].sockfd);
							pthread_cancel(user[i].tid);
						}
						if(user[i].pid >0)						
						kill(user[i].pid, SIGTERM);
					}
				}
	} else {
		char pp[30]="echo Invalid Command>temp.out";
		system(pp);
	}
	//////-------------------
	
	
///////////////////////////
}

void pipeline(char ***str, int len)
{
	//printf("\npipeline str:  %s\n", *str);
	int fd[2];
	pid_t pid;
	int fdd = 0;
	int i=0;
	while (i<len) {
		pipe(fd);
		pid = fork();
		if (pid == 0) {
			printf("\nShsh forked for %s, with in-pipe %d and out-pipe %d\n", *(str+i), fd[0], fd[1]);
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

int getNoOfCommands(char sub[]){
	int i=0,count=0;
	while(sub[i] != '\0'){
		if(sub[i++] == ';'){
			count++;
		}
	}
	return count;
}

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}

int compare_string(char *first, char *second)
{
   while(*first==*second)
   {
      if ( *first == '\0' || *second == '\0' )
         break;
 
      first++;
      second++;
   }
   if( *first == '\0' && *second == '\0' ){
      //printf("\nin compare_String and Match, 1st= %s and 2nd= %s\n", first, second);
	  return 0;
   }
   else
      return -1;
}

initUserStruct(){
    for (int i=0; i<MAX_USERS; i++) {
		user[i].userid = " ";
		user[i].sockfd = -1;
		user[i].isLoggedIn = 0;
		user[i].tid=-1;
		user[i].pid=-1;
	}
}

void terminate()
{
  //printf("\nTerminating... \n");
  int noOfActiveUsers = 0;
	for(int i=0;i<MAX_USERS;i++){
		if(user[i].isLoggedIn){
			noOfActiveUsers++;
		}
	}
	//printf("no of %d active users\n",noOfActiveUsers);
	for(int j=0;j<MAX_USERS;j++){
		if(user[j].isLoggedIn)	{
			printf("\nuser tid=%d, pid=%d", user[j].tid, user[j].pid);
			if(user[j].tid>0){
				pthread_cancel(user[j].tid);
				close(user[j].sockfd);
				kill(user[j].pid-2, SIGTERM);
			}
	   }
	}
	printf("\nUM terminated on Admin request with %d active users\n",noOfActiveUsers);
	fflush(stdout);
  kill(PID, SIGTERM);
}

void slp()
{
	printf("\nUM receives the sleep signal, going to sleep for %d seconds\n",TIME);
	sleep(TIME);
  printf("\n UM Sleep completed.....\n");
  fflush(stdout);
}

void listuser()
{
	printf("\n====List of active users====\n");
	//printf("User ID: %s, isLoggedIn? = %d, userPID= %d\n",user[usersize].userid,user[usersize].isLoggedIn,user[usersize].pid);
	for(int i=0;i<MAX_USERS;i++){
		if(user[i].isLoggedIn){
			printf("User ID: %s and User PID= %d, User Socket No= %d\n",user[i].userid,user[i].pid,user[i].sockfd);
		}
	}
  fflush(stdout);
}

void infor(){
	int noOfActiveUsers = 0;
	printf("\n====UM Information====");
	for(int i=0;i<MAX_USERS;i++){
		if(user[i].isLoggedIn){
			noOfActiveUsers++;
		}
	}
	printf("\nUM pid= %d, Port No= %d, No of Active Users= %d \n", PID, portno, noOfActiveUsers);
}