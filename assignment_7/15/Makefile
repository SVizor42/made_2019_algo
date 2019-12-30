CC=g++ -std=c++0x

all: solution

solution: main.o DSU.o travel.o solver.o
	$(CC) -o solution main.o DSU.o travel.o solver.o

main.o: main.cpp
	$(CC) -c main.cpp

DSU.o: DSU.cpp
	$(CC) -c DSU.cpp

travel.o: travel.cpp
	$(CC) -c travel.cpp

solver.o: solver.cpp
	$(CC) -c solver.cpp

run: solution
	./solution

clean:
	rm -rf *.o solution
