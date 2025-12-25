.PHONY: all clean
all: example.exe

clean:
	rm -f example.exe

example.exe: example.c blat.h
	gcc -Wall -Wextra -std=c89 example.c -o example.exe
