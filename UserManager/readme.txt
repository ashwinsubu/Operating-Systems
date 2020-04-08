#Ashwin Subramaniam
#Shell on Shell
#Project 2 Phase 1

__Compile ShellShell__
gcc UM.c -o UM

__Run ShellShell__
./UM

__Example inputs__
cmd> 1 login
cmd> 2 login
cmd> 1 cmd ls
cmd> 1 pipe ls;grep hel; grep hel2

__Example output___
cmd> 1 login
cmd> 1 cmd echo 123

Shsh processing 1 cmd ls 

Shsh process (1, 339) has been created by 338  

#output of command will be written in 1.dir/1.out

cmd> 1 logout
Shsh process (1, 339) terminates

cmd> 0 #exits from the application