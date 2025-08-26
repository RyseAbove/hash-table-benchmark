CC = g++
CCFLAGS = -Wall -Wextra -Wpedantic -pedantic-errors -Wno-unused-parameter -std=c++17 -g -O2 -fmax-errors=1 -D_GLIBCXX_DEBUG -fsanitize=undefined -fsanitize=address

all: a.out generator

a.out: main.cc hash.h
	$(CC) $(CCFLAGS) main.cc

generator: generator.cc
	$(CC) $(CCFLAGS) generator.cc -o generator

clean:
	rm -f a.out core *.o
