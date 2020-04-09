#Ashwin Subramaniam
#Client Server Socket Communication cum UserManager

____Compile the following programs_____

gcc -pthread -o UM UM.c

gcc -o client client.c

gcc -o admin admin.c  


_______Run the program as below________

./UM port# <T>

./admin <UM-PID>

./client <userid> hostname port#


______List of user commands_______
cmd <any linux command>
pipe <any linux command>;<any linux command>;<any linux command>

_____List of admin commands_______
terminate	#terminates all users, kills their process threads and logs them out, exits UM(server) and exits admin program.
infor		#Prints UM information including pid, port#, server socket#
listuser	#Lists all loggedin users user ID, pid, socket#
sleep		#Makes UM process sleep for <T> seconds. Client processes continue. 


Notes:
> Each terminal/command prompt window is a client, and the UM(server) is also another window.
> You can find hostname by typing 'hostname' in command prompt/terminal.
> port# must be same for client and UM(server)
> T is the time to make the UM sleep.
> The UM-PID will be found as soon as UM starts.
> Both phase 1 and 2 are combined in same UM.
> admin program can signal commands to the UM(server).
> Each user will have his own working space. So once logged in, his working directory will change to userid.dir
> Read the problem statement and report files for detailed description about the commands and syntax.