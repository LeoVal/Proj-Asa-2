all: clean link

compile:
	gcc -g -Wall -Wextra -c main.c

link: compile
	gcc -o main main.o

clean:
	rm -f main main.o
