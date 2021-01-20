
#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include "globals.h"
class Arena;

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_rows;
	int m_cols;
	int m_displayGrid[MAXROWS][MAXCOLS];
	Arena* m_arena;
};

#endif // !HISTORY_INCLUDED
