#Ashwin Subramaniam
#Shell on Shell
#Project 2 Phase 1

__Compile ShellShell__
gcc shsh.c -o shsh

__Run ShellShell__
./shsh

__Example inputs__
cmd>cmd ls
cmd> pipe ls; grep hel; grep hello2;cat>myout.txt
cmd> exit

__Example output___
cmd>cmd echo 123

Shsh processing cmd echo 123 
123

cmd> exit

Shsh processing exit

Shsh process <x> terminates


___Notes___
I've built a shell on shell, which takes user commands and pass to shell for
processing.
If you're executing a command that pipes output using '|' operator use the 'pipe' command.
Example pipe ehco 123; grep 1
The ';' acts like the | operator. I parse the input command and call the system(with each command).
I fork processes and use file descriptors to pipe commands between one process and the other. 
If you're passing a normal command use cmd. It directly passes the command after 'cmd', to system()
