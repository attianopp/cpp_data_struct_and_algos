#include "globals.h"
#include "Arena.h"
#include <random>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	int rnew = r;
	int cnew = c;
	switch (dir)
	{
	case NORTH:  if (r <= 1)        return false; else rnew--; break;
	case EAST:   if (c >= a.cols()) return false; else cnew++; break;
	case SOUTH:  if (r >= a.rows()) return false; else rnew++; break;
	case WEST:   if (c <= 1)        return false; else cnew--; break;
	}
	r = rnew;
	c = cnew;
	return true;
}
