all: a4

a4: Assignment4.c
	gcc -o a4 Assignment4.c

clean:
	rm -rf a4
