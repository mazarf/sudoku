// puzzle class, holds an instance of an unfinished puzzle
// it contains a 9x9 array of cells
// it should be able to determine if it is solved

#ifndef PUZZLE_H
#define PUZZLE_H

#include "cell.h"
#include <utility>

using namespace std;

class Puzzle
{

public:

	Puzzle(); // constructor
	Puzzle(const char *temp); // for initial puzzle
	void deduce(int row, int col, int *value);
	void decide(pair<Cell*, int> p); // marks a cell's value
	int guessDecision(int row, int col); // chooses the 1st candidate
	bool solved(); // returns true if the puzzle has been solved
	void printPuzzle(); // prints the puzzle
	void validate(); // makes sure the puzzle actually is solvable
	bool hiddenSingles();
	Cell grid[9][9];

private:
	void assignSquare(int row, int col); // used by constructor to assign cells a square
	//Cell grid[9][9];

}; // Puzzle

#endif
