# Makefile for MP5
# Heath Gerrald

lab5 : tools.o lab5.o
				gcc -Wall -g tools.o lab5.o -o lab5 -lm

tools.o : tools.c tools.h bst.h
				gcc -Wall -g -c tools.c

lab5.o : lab5.c tools.h bst.h
				gcc -Wall -g -c lab5.c


clean :
				rm -f *.o lab5
