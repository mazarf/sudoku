// Mazar Farran

#include "puzzle.h"

#include <iostream>
#include <cstdlib>
using namespace std;
Puzzle::Puzzle()
{

} // Puzzle



Puzzle::Puzzle(const char *temp) // for initial puzzle only
{

	// fill the grid with the inputted values

	for(int rows = 0; rows < 9; rows++)
	{

		for(int cols = 0; cols < 9; cols++)
		{
			if(temp[rows * 9 + cols] == '.')
				grid[rows][cols].setValue(0);
			else
				grid[rows][cols].setValue(temp[rows * 9 + cols] - '0');

			this->assignSquare(rows, cols); 
		} // for j

	} // for i

} // Puzzle



void Puzzle::deduce(int row, int col, int *value)
{

	Cell& currCell = grid[row][col]; // to make it cleaner
	
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(i == row || j == col || grid[i][j].square == currCell.square) // if on same row, column, or square
			{
				if(currCell.candidates[grid[i][j].getValue()])
				{
				  //currCell.candidates[grid[i][j].getValue()] = false;
					//currCell.numCandidates--; // rule out

					currCell.setCandidate(grid[i][j].getValue(), false);

				} // nested if

				// if we've eliminated all we could with this cell
				// at this time, return control to findDecideableCell()
				// (just to save time)

				if(currCell.getCandidates() <= 0) // contradiction
				{
					*value = -1;
					return; // lost cause
				}

				if(currCell.getCandidates() == 1)
				{
					for(int k = 1; k <= 9; k++)
					{
						if(currCell.candidates[k])
						{
							*value = k;
						} // assigns only possible value to cell
					}
				 
				} // if. wait though because there might be a contra!
			}
		} // for j

	} // for i

} // deduce

void Puzzle::decide(pair<Cell*, int> p)
{

	p.first->setValue(p.second);
	

	//cout << p.first->getValue() << " ";
	//cout << grid[0][0].getValue();

	//this->printPuzzle();
	//cout << " " << solved();

	//cout << endl;
	
} // decide



int Puzzle::guessDecision(int row, int col)
{

	// choose first candidate

	int i;
	for(i = 1; i <= 9; i++)
		if(grid[row][col].candidates[i])
			break;

	return i;

}



// check if solved. solved iff no more zeroes and all numCandidates==1
bool Puzzle::solved()
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(grid[i][j].getValue() == 0 || grid[i][j].getCandidates() != 1 )
				return false;
		} // for j
	} // for i

	return true;
} // solved()


// give each cell a unit (the 3x3 square) 1-9
void Puzzle::assignSquare(int row, int col)
{

	if(row >= 0 && row < 3 && col >= 0 && col < 3)
		grid[row][col].square = 1;
	else if(row >= 0 && row < 3 && col >= 3 && col < 6)
		grid[row][col].square = 2;
	else if(row >= 0 && row < 3 && col >= 6 && col < 9)
		grid[row][col].square = 3;
	else if(row >= 3 && row < 6 && col >= 0 && col < 3)
		grid[row][col].square = 4;
	else if(row >= 3 && row < 6 && col >= 3 && col < 6)
		grid[row][col].square = 5;
	else if(row >= 3 && row < 6 && col >= 6 && col < 9)
		grid[row][col].square = 6;
	else if(row >= 6 && row < 9 && col >= 0 && col < 3)
		grid[row][col].square = 7;
	else if(row >= 6 && row < 9 && col >= 3 && col < 6)
		grid[row][col].square = 8;
	else if(row >= 6 && row < 9 && col >= 6 && col < 9)
		grid[row][col].square = 9;
	else // something wrong
		grid[row][col].square = -1;

} // assignUnit



void Puzzle::printPuzzle()
{

	for(int rows = 0; rows < 9; rows++)
	{
		for(int cols = 0; cols < 9; cols++)
		{

			cout << grid[rows][cols].value;
		
		} // for cols

		//cout << endl;
	} // for rows

} // printPuzzle()


void Puzzle::validate()
{
	for(int r1 = 0; r1 < 9; r1++)
	{
		for(int c1 = 0; c1 < 9; c1++)
		{

			if(grid[r1][c1].getValue() != 0)
			{

				for(int r2 = 0; r2 < 9; r2++)
				{
					for(int c2 = 0; c2 < 9; c2++)
					{
						if(r1 == r2 && c1 == c2) break; // same cell!
						if((r1 == r2 || c1 == c2
							|| grid[r1][c1].square == grid[r2][c2].square)
							&& grid[r1][c1].getValue() == grid[r2][c2].getValue())
						{
							cout << "No solutions." << endl;
							exit(0);
						} // if same row, col, square and same value

					} // for c2

				} // for r2

			} // if nonzero

		} // for c1

	} // for r1

} // validate


bool Puzzle:: hiddenSingles()
{

	for(int r1 = 0; r1 < 9; r1++)
	{

		for(int c1 = 0; c1 < 9; c1++)
		{ // for every cell compare to every other cell

			// copy candidates etc of cell of interest
			int numCandidates = grid[r1][c1].getCandidates();
			bool candidates[10];
			for(int i = 0; i < 10; i++)
				candidates[i] = grid[r1][c1].candidates[i];

			for(int r2 = 0; r2 < 9; r2++)
			{
				for(int c2 = 0; c2 < 9; c2++)
				{

					if(r1 == r2 && c1 == c2) break;

					if(r1 == r2 || c1 == c2 ||
						 grid[r1][c1].square == grid[r2][c2].square)
					{
						for(int i = 1; i < 10; i++)
						{
							if(grid[r1][c1].candidates[i] == true &&grid[r1][c1].candidates[i] == grid[r2][c2].candidates[i])
							{
								candidates[i] = false;
								numCandidates--;
							} // if

						} // for i
					}
				} // for c2
			} // for r2

			// after comparing r1,c1 to every cell
			if(numCandidates == 1)
			{
				for(int i = 1; i < 10; i++)
				{
					if(candidates[i])
						grid[r1][c1].setValue(i);
				} // for

			} // if

		} // for c1
	} // for r1

	return false;
} // hiddenSingles() 
