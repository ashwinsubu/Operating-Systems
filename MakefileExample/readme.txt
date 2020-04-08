#Ashwin Subramaniam
A short project that demonstrates makefile

____Run as____
make hello
./test

#Compiles main.c, square.c and quadratic.c

__Removing object files___
make clean

__Valid inputs___
command> 1 #quadratic solution for three inputs
command> 2 #finds two numbers that are relative prime and the root of their square sum is an integer
command> 0 #quit program
___Example Input___

command> 1
input a b c (ax^2+bx+c=0):  1 
-5
6
__output__
(1, -5, 6) solutions = 3.000000, 2.000000 
2 integer solutions

command2> 2
5
6
__output__
-- 3, 4, sqrt = 5  