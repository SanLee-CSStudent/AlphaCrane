CC=g++

run: test
	./main

test: ContainerGrid.o parser.o test.cpp
	$(CC) ContainerGrid.o parser.o test.cpp -o main 
