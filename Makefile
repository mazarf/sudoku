# mifarran; Farran, Mazar

a.out: main.cpp cell.o puzzle.o
	g++ -Wall -g main.cpp cell.o puzzle.o

cell.o: cell.cpp cell.h
	g++ -Wall -c cell.cpp

puzzle.o: puzzle.cpp puzzle.h cell.h
	g++ -Wall -c puzzle.cpp

clean:
	rm -f cell.o puzzle.o a.out
