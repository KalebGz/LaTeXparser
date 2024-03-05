CC = gcc
CFLAGS = -g3 -std=c11 -pedantic -Wall

#	structure 
#target: dependencies
#	commands



all: proj1 # all: executable1 executable2


string.o: string.c string.h proj1.h
	gcc ${CFLAGS} -c string.c

stack.o: stack.c string.o stack.h proj1.h state_machine.h
	gcc ${CFLAGS} -c stack.c	

macro.o: macro.c string.h stack.h
	gcc ${CFLAGS} -c macro.c

state_machine.o: state_machine.c string.h stack.h
	gcc ${CFLAGS} -c state_machine.c

proj1.o:  proj1.c  string.h stack.h state_machine.h macro.h
	gcc ${CFLAGS} -c proj1.c

proj1: proj1.o string.o stack.o macro.o state_machine.o
	${CC} ${CFLAGS} $^ -o $@


# all: proj1 

# proj1.o:  proj1.c  string.h
# 	gcc ${CFLAGS} -c proj1.c

# string.o: string.c string.h proj1.h
# 	gcc ${CFLAGS} -c string.c

# stack.o: string.c string.h proj1.h
# 	gcc ${CFLAGS} -c string.c       

# proj1: proj1.o string.o
# 	${CC} ${CFLAGS} $^ -o $@



# gcc -o proj1 proj1.o
# $^ = name of target
# $@ = name of all dependencies


# rempves executable and object files
# Run make clean
clean:
	rm -f proj1 *.o
	

# # run make proj1_app to run command
# proj1_app: proj1
# 	./proj1 "proj1 data"
# # can use 