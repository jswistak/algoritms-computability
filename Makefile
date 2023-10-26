# Makefile

all: main

main: main.cpp
	g++ -std=c++11 -Wall -Werror -pedantic main.cpp -o main


clean:
	rm -f main