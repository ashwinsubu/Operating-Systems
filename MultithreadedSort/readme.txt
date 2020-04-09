#AXS190172
#Ashwin Subramaniam
#OS Project 4

_____Steps to Run________
Go to the src folder (/OSP4/src/)

Compile the ParallelSort as shown below:

javac axs190172/ParallelSort.java

Run the program as below (current working directory should be /OSP4/src):

java axs190172.ParallelSort inputFile.txt -o

or

java axs190172.ParallelSort inputFile.txt -r

________Example Input________

>Input Format:

16 #No Of inputs = N (N should be 2^x where x is an integer greater than 2)
1
2
3
.
.
.
16
8 #Next No Of inputs = N (N should be 2^x where x is an integer greater than 2)
1
2
.
.
8
0 #Terminate program with last line as 0

> Have N random digits in any order
> Paste input file inside src/axs190172/
> Check input.1 and input.2 for sample input file.


_____NOTES_______
If you dont have jdk installed, install in linux/ubuntu command line this way:

apt install default-jdk  