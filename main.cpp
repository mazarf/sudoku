// Mazar Farran - Sudoku Solver
// resubmit

#include <iostream>

#include <cstdlib> // for isprint()
#include <sstream> // for formating hex
#include <iomanip> // all this just for a special case!

#include <list>
#include <stack> // STL containers used in solving algorithm
#include <utility> // pair

#include "puzzle.h"

using namespace std;

// I/O functions
void getInput(char* temp);
void printError(const char *expected, const char *saw);
void printError(const char *expected, const char saw);
bool isValue(int c);

// solving functions
void solve(Puzzle puzzle);
pair<Cell*, int>* findDecidableCell(Puzzle& puzzle);
//void deduce(Cell* c, Puzzle& p, int row, int col, int *value);
bool guess(Puzzle *puzzle, Puzzle *alternative);
bool hiddenSingles(Puzzle& puzzle); // hidden singles deduction


int main()
{

    char temp[100]; // stores a valid puzzle string
    getInput(temp);

    //for(int i = 0; i < 81; i++)
    //{
	//cout << temp[i];
    //}

    //cout << endl;



	Puzzle p(temp);
	//p.printPuzzle();
	//cout << endl;

	p.validate();

	solve(p);

    return 0;	
} // main


void getInput(char* temp)
{

    const char *VALUE = "<value>";
    const char *NEW_LINE = "\\n";
    const char *END_OF_FILE = "<eof>";

    // read in array one char at a time
    // if something unexpected, give error and terminate

    int count = 0;   
 
    while(1)
    {

	if(count < 81) // still filling the sudoku
	{ 

	    if(cin.peek() >= '1' && cin.peek() <= '9') // digit
	    {
			// store number in place
			temp[count] = cin.get();
	    }
	    else if(cin.peek() == '.')
	    {
			// store space in place
			temp[count] = cin.get();
	    }
	    else // nothing else is valid yet
	    {
	
			if(cin.peek() == -1) // early EOF
			    printError(VALUE, END_OF_FILE);
			else if(cin.peek() == '\n')
			    printError(VALUE, NEW_LINE);
			else // default case
			{
			    char invalid = cin.get();
			    printError(VALUE, invalid);
			}

			exit(0);

	    } // if

	} // if count < 81
	else if(count == 81) // expecting a newline
	{
	    if(cin.peek() == '\n')
	    {
			cin.get(); // skip and move on
			count++; // we'll miss a count by continuing
			continue;
	    }
		else if(cin.peek() == -1) // early EOF
			printError(NEW_LINE, END_OF_FILE);
		//else if(isValue(cin.peek()))
		//	printError(NEW_LINE, VALUE);
	    else // default
	    {
			char invalid = cin.get();
			printError(NEW_LINE, invalid);
	    } // if

	    // since we didn't continue, we must exit
	    exit(0);
	} // if count == 81
	else // looking for EOF
	{
	    if(cin.peek() == -1)
			break; // we're done here
	    //else if(isValue(cin.peek()))
		//	printError(END_OF_FILE, VALUE); 
	    else if(cin.peek() == '\n')
			printError(END_OF_FILE, NEW_LINE);
	    else
	    {
			char invalid = cin.get();
			printError(END_OF_FILE, invalid);
	    }
	    exit(0);

	} // end big if

	count++;
    } // while(1)
}


// simple function that prints a standard error to cout
void printError(const char *expected, const char *saw)
{
    cout << "ERROR: expected " << expected << " saw " << saw << "\n";
} // printError



// overloaded to handle single chars
void printError(const char *expected, const char sw)
{
	unsigned char saw = sw;
    if(!isprint(saw)) // not a printable character
    {
	char hexString[8];  // stores '\xHH', where HH is the ascii
	stringstream s;	    // value in hex
	s << "\\x" << hex << setfill('0')
	    << setw(2) << (int)saw;
	s >> hexString;

	// print it 
	cout << "ERROR: expected " << expected << " saw " 
	    << hexString << "\n";
    }
    else // normal character, just print
    {
	cout << "ERROR: expected " << expected << " saw "
	    << saw << "\n";
    } // if

} // printError for chars



// tests if character is 1-9 or .
bool isValue(int c)
{
    if((c >= '1' && c <= '9') || (c == '.'))
		return true;

    return false;
} // isValue



/*void validate()
{

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			for(int k = 0; k < 9; k++)
			{
				for(int m = 0; m < 9; m++)
				{

					if((i == k || j == m 
						|| grid[i][j].square == grid[k][m].square)
						&& grid[i][j].getValue() == grid[k][m].getValue())
					{
						cout << "No Solution" << endl;
					} // if same row col, square and have same value


				} // for m
			} // for k
		} // for j
	} // for i

} // validate */


void solve(Puzzle puzzle)
{

	list<Puzzle> solutions;
	stack<Puzzle> alternatives;
	alternatives.push(puzzle);

	bool simplificationFound = false;

	while(!alternatives.empty())
	{
		puzzle = alternatives.top();
		alternatives.pop(); // try the next possibility

		try
		{

		while(pair<Cell*, int> *p = findDecidableCell(puzzle))
			puzzle.decide(*p);

		} catch(const char* c) // contradiction found already!
		{
			continue;
		}


		simplificationFound = true;
		while(!puzzle.solved() && simplificationFound)
		{
			// simplification strategies here 
			simplificationFound = false;
			simplificationFound = puzzle.hiddenSingles();


			// guessing part
			if(!simplificationFound)
			{
				Puzzle alternative;
				if((simplificationFound = guess(&puzzle, &alternative)))
				{
					alternatives.push(alternative);
				} // if
			} // if we need to guess


			if(simplificationFound)
			{
				try
				{
					while(pair<Cell*, int> *p = findDecidableCell(puzzle))
						puzzle.decide(*p);
				} catch(const char* c) // contradiction found
				{
					break;
				}
			} // if we can still deduce

		} // while no solution or contradiction

		if(puzzle.solved())
			solutions.push_front(puzzle);

	} // while still more alternatives

	if(!solutions.empty())
		while(!solutions.empty()) // print all solutions
		{
			solutions.front().printPuzzle();
			cout << endl;
			solutions.pop_front();
		} // print all solutions
	else // if no solutions
		cout << "No solutions." << endl;

} // solve



// naked singles deduction. returns a cell that can be decided 
pair<Cell*, int>* findDecidableCell(Puzzle& puzzle)
{

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(puzzle.grid[i][j].getValue() == 0)
			{
				int value = 0;
				puzzle.deduce(i, j, &value);
				if(value == -1) // contra
					throw " ";
				if(puzzle.grid[i][j].getCandidates() == 1)
					return new pair<Cell*, int>(&puzzle.grid[i][j], value);
			}
		}
	}

	// no simplification found
	return NULL;

}

/*void deduce(Cell* c, Puzzle& p, int row, int col, int *value)
{
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{
			if(i == row || j == col || p.grid[i][j].square == p.grid[row][col].square)
			{
				if(p.grid[row][col].candidates[p.grid[i][j].getValue()] == true)
				{
					p.grid[row][col].candidates[p.grid[i][j].getValue()] = false;
					p.grid[row][col].numCandidates--;
				} // nested if

				if(numCandidates == 1)

			} // if

		} // for j

	} // for i
} // deduce */

// UNTESTED
bool guess(Puzzle *puzzle, Puzzle *alternative)
{

	int row = 0, col = 0, smallest = 10;

	bool isTwo = false, foundOne = false;

	// find cell with fewest candidates, stop if candidates == 2
	// (for efficiency's sake)

	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < 9; j++)
		{

			if(puzzle->grid[i][j].getCandidates() == 2)
			{
				row = i;
				col = j;
				isTwo = true;
				foundOne = true;
				break;
			}
			if(puzzle->grid[i][j].getCandidates() != 1 
				&& puzzle->grid[i][j].getCandidates() < smallest)
			{
				smallest = puzzle->grid[i][j].getCandidates();
				foundOne = true;
				row = i;
				col = j;
			} // if

		} // for j

		if(isTwo)
			break; // needed to break out of double for loop;

	} // for i

	// now we should have a suitable candidate to guess with

	if(foundOne)
	{
		int choice = puzzle->guessDecision(row, col); // choose guess

		//cout << "Choice" << choice << endl;

		*alternative = *puzzle; // preserve the puzzle's state as an alternative
		alternative->grid[row][col].setCandidate(choice, false); // mark the candidate

		//alternative.printPuzzle();

		puzzle->grid[row][col].setValue(choice);

		//cout << endl;

		//alternative.printPuzzle();

		//cout << endl;

		//puzzle.printPuzzle();
		//if(puzzle.grid[0][0].getValue() == 8)
		//	cout << "bad" << endl;

		// make alternative without choice as a candidate
		// mark candidate on current puzzle

		return true;

	} // if
	else // if no candidate found. cannot make further guesses
		return false;

} // guess


/*bool hiddenSingles(Puzzle& p)
{

	for(int r1 = 0; r1 < 9; r1++)
	{

		for(int c1 = 0; c1 < 9; c1++)
		{ // for every cell compare to every other cell

			// copy candidates etc of cell of interest
			int numCandidates = p.grid[r1][c1].getCandidates();
			bool candidates[10];
			for(int i = 0; i < 10; i++)
				candidates[i] = p.grid[r1][c1].candidates[i];

			for(int r2 = 0; r2 < 9; r2++)
			{
				for(int c2 = 0; c2 < 9; c2++)
				{
					if(r1 == r2 || c1 == c2 ||
						 p.grid[r1][c1].square == p.grid[r2][c2].square)
					{
						for(int i = 0; i < 10; i++)
						{
							if(p.grid[r1][c1].candidates[i] == p.grid[r2][c2].candidates[i])
							{
								candidates[i] = false;
								numCandidates--;

							} // if
						} // for i
					}
				} // for c2
			} // for r2

			// after comparing r1,c1 to every cell

		} // for c1
	} // for r1

	return false;
} // hiddenSingles() */
