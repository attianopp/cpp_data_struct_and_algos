#include <iostream>
#include <string>
#include <queue>
#include <cassert>
using namespace std;

bool pathExists(char maze[][10], int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

/*
int main() {


	char maze[10][10] = {
		{ 'X','X','X','X','X','X','X','X','X','X' },
		{ 'X','.','.','.','.','.','.','.','.','X' },
		{ 'X','X','.','X','.','X','X','X','X','X' },
		{ 'X','.','.','X','.','X','.','.','.','X' },
		{ 'X','.','.','X','.','.','.','X','.','X' },
		{ 'X','X','X','X','.','X','X','X','.','X' },
		{ 'X','.','X','.','.','.','.','X','X','X' },
		{ 'X','.','X','X','X','.','X','X','.','X' },
		{ 'X','.','.','.','X','.','.','.','.','X' },
		{ 'X','X','X','X','X','X','X','X','X','X' }
	};

	if (pathExists(maze, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << maze[i][j] << ' ';
		}
		cout << endl;
	}
}
*/
bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = '^';
	Coord top(0, 0);
	Coord end(er, ec);

	while (!coordQueue.empty()) {
		top = coordQueue.front();
		int topR = top.r();
		int topC = top.c();
		coordQueue.pop();

		if (top.c() == end.c() && top.r() == end.r()) { //found the end
			return true;
		}

		if ((topR - 1) >= 0 && (topR - 1) < 10 && maze[topR - 1][topC] != '^' && maze[topR - 1][topC] != 'X') { //north
			coordQueue.push(Coord(topR - 1, topC));
			maze[topR - 1][topC] = '^';
		}
		if ((topC + 1) >= 0 && (topC + 1) < 10 && maze[topR][topC + 1] != '^' && maze[topR][topC + 1] != 'X') { //east
			coordQueue.push(Coord(topR, topC + 1));
			maze[topR][topC + 1] = '^';
		}
		if ((topR + 1) >= 0 && (topR + 1) < 10 && maze[topR + 1][topC] != '^' && maze[topR + 1][topC] != 'X') { //south
			coordQueue.push(Coord(topR + 1, topC));
			maze[topR + 1][topC] = '^';
		}
		if ((topC - 1) >= 0 && (topC - 1) < 10 && maze[topR][topC - 1] != '^' && maze[topR][topC - 1] != 'X') { //west
			coordQueue.push(Coord(topR, topC - 1));
			maze[topR][topC - 1] = '^';
		}
	}

	return false;
}
