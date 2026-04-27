.PHONY:clean
all:Vector.o
	g++ Vector.o -o vector_test -L. -lVectorTest

Vector.o: Vector.cpp Vector.h
	g++ -c Vector.cpp -o Vector.o

clean:
	rm -f Vector.o vector_test
