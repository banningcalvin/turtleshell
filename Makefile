# choose your compiler
CC=gcc
#CC=gcc -Wall

mysh: sh.o get_path.o alias.o history.o main.c 
	$(CC) -g -lpthread main.c sh.o get_path.o alias.o history.o -o mysh

sh.o: sh.c sh.h
	$(CC) -g -c sh.c

get_path.o: get_path.c get_path.h
	$(CC) -g -c get_path.c

alias.o: alias.c alias.h
	$(CC) -g -c alias.c

history.o: history.c history.h
	$(CC) -g -c history.c

clean:
	rm -rf *.o mysh *~
