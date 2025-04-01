all: a3

a3: Assignment4.c
	gcc -o a3 Assignment4.c

clean:
	rm -rf a3
