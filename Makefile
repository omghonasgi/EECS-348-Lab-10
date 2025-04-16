CC = g++
CFLAGS = -c -Wall


all: prog

prog: calculator.o
	$(CC) $(CDFLAGS) calculator.o -o prog

matrix.o: matrix.cpp
	$(CC) $(CFLAGS) calculator.cpp

clean:
	rm -rf *.o prog