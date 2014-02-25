#ifndef CELL_H
#define CELL_H

// the basic component of the sudoku puzzle
// each cell will contain a value (0 for empty)
// as well as all possiblilities for that cell

class Cell
{

friend class Puzzle;

public:
	Cell();
	//Cell(int initValue); // constructor
	void setValue(int newValue);
	int getValue(); 
	int getCandidates(); // returns the number
	//void set(); // candidates[i] = value
	void setCandidate(int i, bool b) // the compiler is being difficult
	{
		if(candidates[i] == b) // already equal, should not happen
			throw "the hell?";
		else
		{
			candidates[i] = b;
			if(b)
				numCandidates++;
			else
				numCandidates--;
		}
	}

private:
	int value;
	bool candidates[10]; // actually 9, but we use 1-9 for simplicity
	int numCandidates;
	int square; // which 3x3 square 

}; // Cell header



#endif
