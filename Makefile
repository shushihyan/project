all: complex_sort

complex_sort: main.o complex.o sorting.o
	g++ main.o complex.o sorting.o -o complex_sort

main.o: main.cpp complex.h sorting.h
	g++ -c main.cpp -o main.o

Complex.o: complex.cpp complex.h
	g++ -c complex.cpp -o complex.o

Sorting.o: sorting.cpp sorting.h
	g++ -c sorting.cpp -o sorting.o

clean:
	rm *.o complex_sort
