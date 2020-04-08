#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
//const int ppid = getppid();
int size=0;
int MAX_USER=10;
struct User{
	char *loginid;
	int pid;
	int fd[2];
}users[20];

//UM.exe
main() {
	
	initUserStruct();
	const int ppid = getpid();
do{
 char str1[200];
 char str2[200];
	
	printf("\nucmd> ");
	gets(str1);	
strcpy(str2,str1);	
	if(strcmp(str1, "0") == 0){
		for(int i=0;i<size;i++){
			//kill(users[i].pid,SIGKILL);
		}
		exit(1);
		
	}
	char *p = strtok (str1, " ");
	char *array[100];
	int i=0;
	while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, " ");
    }
	
	//printf("\n %s\n", array[0]);
	char *id = array[0];
	char *command = array[1];
	//printf("\n*command = %s, and len(command) = %d\n", command, strlen(command));
	int isExist = isUserExist(id, users);
	
	if(compare_string(command, "login") == 0){
		//printf("\n in main method id : %s\n", id);
		
		if(isExist == 0){
			addUser(id);
		} else {
			printf("\n Alert: User already logged in....\n");
		}
	} else if(compare_string(command, "logout") == 0){
		if(!isExist){
			printf("\nAlert: Cannot logout- User hasnt logged in\n");
			continue;
		} else {
			int i = getUserIndex(id, users);
			printf("Shsh process (%s, %d) terminates", users[i].loginid, users[i].pid);
			users[i].loginid = " ";
			users[i].pid = -1;
			close(users[i].fd);
			users[i].fd[0] = -1;
			users[i].fd[1] = -1;
		}
	} else {
		int commandOver = 0;
		if(isExist){
			int userIndex = getUserIndex(id, users);
			/////////
			//printf("\nexecuting a command for user: %s\n",users[userIndex].loginid);
			//printf("\npassing the command str2: %s\n",str2);
			printf("\nShsh processing %s\n", str2);
			pipe(users[userIndex].fd);
			
			/*int userpid = users[userIndex].pid;
			if(users[userIndex].pid == -1){
				userpid = fork();
				if(userpid != 0) {
					users[userIndex].pid = userpid;
					printf("\nShsh process (%s, %d) has been created by %d\n", users[userIndex].loginid, users[userIndex].pid, ppid);
				}
			}*/
			int userpid = fork();
			if(userpid != 0){
				if(users[userIndex].pid == -1)
				users[userIndex].pid = userpid;
				printf("\nShsh process (%s, %d) has been created by %d\n", users[userIndex].loginid, users[userIndex].pid, ppid);
			}
			
			if(userpid == 0){
			///
			
			dup2(users[userIndex].fd[1], 1);
			close(users[userIndex].fd[0]);
			///
			
			if(compare_string("cmd",command) == 0){
				int startLen = strlen(id) + 1 +strlen(command) + 1;
				int commandLen = strlen(str2);
				int len = commandLen - startLen+1;
				char syscommand[len];
				memcpy(syscommand, &str2[startLen], len);	
				syscommand[len]='\0';
				//printf("\nsyscommand = %s\n", syscommand);
				//got syscommand ls...
				char dirname[15]="";
				strncat(dirname, users[userIndex].loginid, strlen(users[userIndex].loginid));
				strncat(dirname, ".dir",4);
				//printf("\ndirname is:: %s\n", dirname);
				chdir(dirname);
				char filename[25]="";
				strncat(filename, syscommand, strlen(syscommand));
				strncat(filename, ">>", 2);
				strncat(filename, users[userIndex].loginid, strlen(users[userIndex].loginid));
				strncat(filename, ".out",4);
				//printf("\nfilename is:: %s\n", filename);
				
				system(filename);
				chdir("..");
				printf("\noutput written to file\n");
			} else if(compare_string("pipe",command) == 0){
				printf("\nIn pipe command \n");
				int startLen = strlen(id) + 1 + 4 + 1;
				int commandLen = strlen(str2);
				int len = commandLen - startLen+1;
				char syscommand[len];
				memcpy(syscommand, &str2[startLen], len);	
				syscommand[len]='\0';
				//printf("\nsyscommand = %s\n", syscommand);
				 
				 int noOfCommands = getNoOfCommands(syscommand)+1;
				 //printf("\nNo of commands is: %d\n",noOfCommands);
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
				
				///////
				char dirname[15]="";
				strncat(dirname, users[userIndex].loginid, strlen(users[userIndex].loginid));
				strncat(dirname, ".dir",4);
				chdir(dirname);
				char filename[25]="";
				strncat(filename, users[userIndex].loginid, strlen(users[userIndex].loginid));
				strncat(filename, ".out",4);
				printf("\nfilename is:: %s\n", filename);
				//////
				char pp[40]="";
				strncat(pp, "cat>>", 5);
				strncat(pp, filename,strlen(filename));
				array[i++] = pp;
				printf("\nPrinting separated pipe cmds ::\n");
				for (i = 0; i < noOfCommands+1; i++)
					printf("%s\n", array[i]);
				
				pipeline(array, noOfCommands+1);
				chdir("..");
			}
			exit(1);
		  } else {
			  wait(NULL); 
		  }
		  
		}	//isExist ends here
	
	}

}while(1);
}

int getUserIndex(char *id, struct User *users){
	int index=-1;
	for(int i=0;i<MAX_USER;i++){
		if(compare_string(users[i].loginid, " ") != 0 &&  compare_string(id, users[i].loginid) == 0){
		//	printf("\nMATCH\n");
			index = i;
			break;
		}
	}
	return index;
}

addUser(char *id){
	//if(ppid == getppid()){
//	printf("\n in addUser method id : %s\n", id);
	users[size].loginid = malloc(sizeof(char) * (strlen(id) + 1));
	strcpy(users[size].loginid, id);
	
	
	//printf("\n in addUser method user.loginid : %s\n", users[size].loginid);
	
	//create directory for user[size]
	char dirname[15]="";
	strncat(dirname, users[size].loginid, strlen(users[size].loginid));
	strncat(dirname, ".dir",4);
	int mk = mkdir(dirname, 0777);
	//printf("\n isDirectoryMAde? : %d\n", mk);
	
		//pipe(users[size].fd);
		//int processID= fork();
		//users[size].pid = processID;
		//printf("\nUser %s created with PID= %d\n", users[size].loginid, users[size].pid);
	
	size++;
	//}
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
	for (int i=0; i<20; i++) {
		users[i].loginid = " ";
		users[i].pid = -1;
		users[i].fd[0] = -1;
		users[i].fd[1] = -1;
	}
}

int isUserExist(char *id, struct User *users){
	//printf("\n in isUserExist method id : %s\n", id);
	int flag =0;
	/*for(int i=0;i<20;i++){
		printf("users[%d].loginid= %s\n",i, users[i].loginid);
	}*/
	for(int i=0;i<20;i++){
//printf("users[%d].loginid= %s\n",i, users[i].loginid);
		if(compare_string(users[i].loginid, " ") != 0 &&  compare_string(id, users[i].loginid) == 0){
		//	printf("\nMATCH\n");
			flag = 1;
			break;
		}
	}
	return flag;
}

void substring(char s[], char sub[], int p, int l) {
   int c = 0;
   
   while (c < l) {
      sub[c] = s[p+c-1];
      c++;
   }
   sub[c] = '\0';
}


void executeCommands(int userIndex,struct User users,char *str1,char *array){
	printf("\nexecuteCommands command is: %s\n", str1);
	printf("\nexecute Commands array is: %s\n", array);
	printf("\nexecute Commands array[1] is: %s\n", array[0]);
	printf("\nexecute Commands array[2] is: %s\n", array[1]);
	char *command = *(array+1);
	
	char subcommand[4];
	printf("\nexecute Commands array[2] is: %s\n", *(array+2));
	memcpy(subcommand, &str1[0], 3);
	subcommand[4] = '\0';
	
	int len=0;
	if(compare_string("cmd",command) == 0){
		int startLen = strlen(*array) + 1;
		int commandLen = strlen(str1);
		len = commandLen - startLen+1;
		char syscommand[len];
	    memcpy(syscommand, &str1[startLen], len);	
		syscommand[len]='\0';	
		system(syscommand);
	}
	if(strcmp("pip",subcommand) == 0){
	 char sub[1000];
	 substring(str1, sub, 5, strlen(str1)-4);
	 int noOfCommands = getNoOfCommands(sub)+1;
	 printf("\nNo of commands is: %d\n",noOfCommands);
     printf("sub == %s\n", sub);
	 printf("str2 == %s\n", str1);
	 char *p = strtok (sub, ";");
     char **array[100];
	int i=0;
	while (p != NULL)
    {
        array[i++] = p;
        p = strtok(NULL, ";");
    }
	printf("\nPrinting separated pipe cmds ::\n");
    for (i = 0; i < noOfCommands; i++)
        printf("%s\n", array[i]);
	
	//pipeline(array, noOfCommands);
	}
}


void pipeline(char ***str, int len)
{
	printf("\npipeline str:  %s\n", *str);
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






