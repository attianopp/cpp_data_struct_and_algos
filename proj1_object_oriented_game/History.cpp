#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols): m_rows(nRows),m_cols(nCols)
{
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			m_displayGrid[r - 1][c - 1] = 0;
	m_arena = nullptr;
}

bool History::record(int r, int c)
{
	bool result;
	if (r < 1 || r > m_rows || c < 1 || c > m_cols) {
		result = false;
	}
	else {
		result = true;
		int temp = m_displayGrid[r-1][c-1];
		temp++;
		m_displayGrid[r-1][c-1] = temp;
	}
	return result;
}

void History::display() const
{
	//char displayGrid[MAXROWS][MAXCOLS];
	int r, c;
	clearScreen();
	for (r = 1; r <= m_rows; r++)
	{ 
		for (c = 1; c <= m_cols; c++) {
			if (m_displayGrid[r - 1][c - 1] == 0) {
				cout << '.';
			}
			else if (m_displayGrid[r - 1][c - 1] > 0 && m_displayGrid[r - 1][c - 1] < 26) {
				int displaynum = m_displayGrid[r - 1][c - 1];
				char z = 'Z';
				char output = z - (26 - displaynum); // corresponds to the capital alphabet letter in question, 26 is the number of letters in the alphabet
				cout << output;
			}
			else if (m_displayGrid[r - 1][c - 1] >=26) {
				cout << "Z";
			}
		}
		cout << endl;
	}
	cout << endl;
	
}
