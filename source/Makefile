# Makefile

all: main

main: main.o multigraph.o
	g++ -std=c++17 -Wall -g -fsanitize=address -pedantic main.o multigraph.o -o main.out

main.o: main.cpp colors.hpp multigraph.hpp
	g++ -std=c++17 -Wall -g -fsanitize=address -pedantic -c main.cpp

multigraph.o: multigraph.cpp multigraph.hpp colors.hpp
	g++ -std=c++17 -Wall -g -fsanitize=address -pedantic -c multigraph.cpp

clean:
	rm -f *.o *.out