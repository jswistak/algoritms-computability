# Makefile

exe_path = ../exe

all: main generator

main: main.o multigraph.o
	g++ -std=c++17 -Wall -g -pedantic main.o multigraph.o -o $(exe_path)/main.exe

main.o: main.cpp colors.hpp multigraph.hpp
	g++ -std=c++17 -Wall -g -pedantic -c main.cpp

multigraph.o: multigraph.cpp multigraph.hpp colors.hpp
	g++ -std=c++17 -Wall -g -pedantic -c multigraph.cpp

generator: generator.cpp
	g++ -std=c++17 -Wall -g -pedantic generator.cpp -o $(exe_path)/generator.exe

clean:
	rm -f *.o $(exe_path)/*.exe