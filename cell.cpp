// cell.cpp, where we will define Cell class methods

#include "cell.h"
#include <iostream>

Cell::Cell()
{
	// nothing!	
}

/*Cell::Cell(int initValue)
{

	value = initValue; // assume correct input

	if(value == 0) // a space
	{

		candidates[0] = false; // can never be 0!
		for(int i = 1; i < 10; i++)
		{
			candidates[i] = true;
		} // fill with candidates

	} // if
	else // a definite number has no possible candidates!
	{
		for(int i = 0; i < 10; i++)
		{
			candidates[i] = false;
		} // for
	} // else


} // constructor */



void Cell::setValue(int newValue) // initial setting only!!
{

	if(newValue < 0 || newValue > 9)
	{
		std::cout << newValue << std::endl;
		throw "ERROR: number not in range"; // should never happen
	}
	value = newValue;

	if(value == 0) // a space
	{

		candidates[0] = false; // can never be 0!
		for(int i = 1; i < 10; i++)
		{
			candidates[i] = true;
		} // fill with candidates

		numCandidates = 9;

	} // if
	else // a definite number has no possible candidates!
	{
		for(int i = 0; i < 10; i++)
		{
			if(i == value)
				candidates[i] = true;
			else
				candidates[i] = false;
		} // for

		numCandidates = 1;

	} // else

} // setValue



int Cell::getValue()
{
	return value;
} // getValue



int Cell::getCandidates()
{
	return numCandidates;
}


