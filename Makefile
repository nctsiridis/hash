all:
	mkdir -p build
	cc -Wall -Wextra hash.c -o build/program

run: all build/program
	./build/program
