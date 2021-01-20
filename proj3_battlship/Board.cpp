#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
	int shipDestroyedCount;
	vector<vector<char> > matrix;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
	shipDestroyedCount = 0;// This compiles, but may not be correct
	matrix.resize(g.rows(), vector<char>(g.cols(), '.'));

}

void BoardImpl::clear()
{
	for (int i = 0; i < static_cast<int>(matrix.size()); i++) {
		for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) {
			matrix[i][j] = '.';
		}
	}

	// OR matrix.resize(m_game.cols(), vector<char>(m_game.rows(), '.'));
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
				matrix[r][c]='#'; // TODO:  Replace this with code to block cell (r,c)
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
			if (matrix[r][c] == '#') { // TODO:  Replace this with code to unblock cell (r,c) if blocked
				matrix[r][c] = '.';
			}
		}
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
	bool isinvalid = false;
	
	//checking if shipid & point are valid
	if (shipId < 0 || shipId >= m_game.nShips() || !m_game.isValid(topOrLeft)
	){
		isinvalid=true;
		return false;
	}
	if (dir != HORIZONTAL && dir != VERTICAL) {
		isinvalid = true;
		return false;
	}
	//cerr << "accessing ship vector 1: " << endl;
	int ship_length = m_game.shipLength(shipId);
	//cerr << "accessing ship vector 2: " << endl;
	string ship_name = m_game.shipName(shipId);
	//cerr << "accessing ship vector 3: " << endl;
	char ship_symb = m_game.shipSymbol(shipId);
	//cerr << "successful access " << endl;
	int p_row = topOrLeft.r;
	int p_col = topOrLeft.c;



	//checking to make sure ship can be placed in specified direction
	//for specified length without overlap or going outside bounds
	if (dir == HORIZONTAL) {
		if ((p_col + ship_length) > m_game.cols()) {
			isinvalid = true;
			//cerr << " length + col > #colums" << endl;
			return false;
		}
		for (int i = 0; i < ship_length; i++) {
			if (matrix[p_row][p_col+i] != '.') {
				//cerr << "cant place cuz its not just water" << endl;
				isinvalid = true;
				return false;
			}
		}
	}
	else if (dir == VERTICAL) {
		if ((p_row + ship_length) > m_game.rows()) {
			isinvalid = true;
			//cerr << " length + row > #rows" << endl;
			return false;
		}
		for (int i = 0; i < ship_length; i++) {
			if (matrix[p_row+i][p_col] != '.') {
				//cerr << "cant place cuz its not just water (VERTICAL)" << endl;
				isinvalid = true;
				return false;
			}
		}
	}

	//check that ship hasnt already been placed by iterating through
	//entire matrix and checking to see if any point equals
	//the ship symbol
	for (int i = 0; i < static_cast<int>(matrix.size()); i++) {
		for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) {
			if (matrix[i][j] == ship_symb) {
				isinvalid = true;
				//cerr << "ship already placed" << endl;
				return false;
			}
		}
	}

	//if any of the error checking found an error
	if (isinvalid) {
		//cerr << "checking is invalid for some reason" << endl;
		return false;
	}

	// non-erroneous input so proceed to place ship at specified location
	if (dir == HORIZONTAL) {
		for (int i = 0; i < ship_length; i++) {
			matrix[p_row][p_col + i] = ship_symb;
		}
		//cerr << "				placed ship" << shipId << " successfully (HORIZONTAL)" << endl;

		return true;
	}
	else if (dir == VERTICAL) {
		for (int i = 0; i < ship_length; i++) {
			matrix[p_row + i][p_col] = ship_symb;
		}
		//cerr << "				placed ship" << shipId << " successfully (VERTICAL)" << endl;
		return true;
	}
	return !isinvalid; //return if its valid or not
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
	bool isinvalid = false;

	//checking if shipid & point are valid
	if (shipId < 0 || shipId >= m_game.nShips() || !m_game.isValid(topOrLeft)
		) {
		//cerr << "invalid input for shipid or point" << endl;
		isinvalid = true;
		return false;
	}
	if (dir != HORIZONTAL && dir != VERTICAL) {
		//cerr << "invalid input for direction" << endl;
		isinvalid = true;
		return false;
	}

	int ship_length = m_game.shipLength(shipId);
	string ship_name = m_game.shipName(shipId);
	char ship_symb = m_game.shipSymbol(shipId);
	int p_row = topOrLeft.r;
	int p_col = topOrLeft.c;
	


	//checking if the board contains the entire ship at specified location
	if (dir == HORIZONTAL) {

		if ((p_col + ship_length) >= m_game.cols()) {
			//couldnt possibly fit at location
			return false;
		}
		for (int i = 0; i < ship_length; i++) {
			if ( matrix[p_row][p_col + i] != ship_symb) {
				//(p_col+i<m_game.cols()) &&
				//cerr << "cant find ship at horizontal" << endl;
				isinvalid = true;
				return false;
			}
			//else {
				//cerr << "		went over the bounds of row for some reason" << endl;
			//	return false;
		//	}
		}
	}
	else if (dir == VERTICAL) {
		if ((p_row + ship_length) >= m_game.rows()) {
			//couldnt possibly fit at location
			return false;
		}
		for (int i = 0; i < ship_length; i++) {
			if ( matrix[p_row + i][p_col] != ship_symb) {
				//(p_row + i<m_game.rows()) &&
				//cerr << "cant find ship at vertical" << endl;
				isinvalid = true;
				return false;
			}
			//else {
				//cerr << "		went over the bounds of row for some reason" << endl;
			//	return false;
			//}
		}
	}

	//if any of the error checking found an error
	if (isinvalid) {
		//cerr << "somehow checking my bool" << endl;
		return false;
	}

	//non-erroneous input so proceed to remove ship
	if (dir == HORIZONTAL) {
		for (int i = 0; i < ship_length; i++) {
			matrix[p_row][p_col + i] = '.';
		}
		//cerr << "unplaced ship: " << shipId << " successfully (HORIZONTAL)" << endl;
		return true;
	}
	else if (dir == VERTICAL) {
		for (int i = 0; i < ship_length; i++) {
			matrix[p_row + i][p_col] = '.';
		}
		//cerr << "unplaced ship: " << shipId << " successfully (VERTICAL)" << endl;
		return true;
	}

	return !isinvalid;
}

void BoardImpl::display(bool shotsOnly) const
{
	int rows = m_game.rows();
	int cols = m_game.cols();
	if (shotsOnly) {
		cout << "  ";
		for (int p = 0; p < cols; p++) {
			cout << p;
		}
		cout << endl;
		for (int i = 0; i < static_cast<int>(matrix.size()); i++) {
			cout << i << " ";
			for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) { // dont show ships
				// only show: . X o 
				if (matrix[i][j] == '.' || matrix[i][j] == 'o' || matrix[i][j] == 'X') {
					cout << matrix[i][j];
				}
				else {
					cout << '.';
				}
			}
			cout << endl;
		}
	}
	else {
		cout << "  ";
		for (int p = 0; p < cols; p++) {
			cout << p;
		}
		cout << endl;
		for (int i = 0; i < static_cast<int>(matrix.size()); i++) {
			cout << i << " ";
			for (int j = 0; j < static_cast<int>(matrix[i].size()); j++) {
				cout << matrix[i][j];
			}
			cout << endl;
		}
	}
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
	if (!m_game.isValid(p) || matrix[p.r][p.c] == 'X' || matrix[p.r][p.c] == 'o') {
		return false;
   }
	char point = matrix[p.r][p.c];
	int ship = -1;
	//finding out which ship is at point p
	for (int i = 0; i < m_game.nShips(); i++) {
		if (point == m_game.shipSymbol(i)) {
			ship = i;
			break;
		}
	}

	if (ship == -1 && point != '.') {
		//cerr << "ERROR: didnt find a matching shipId for the symbol at point p, and point p isnt X,o, or ." << endl;
		//cerr << "inexplicable error" << endl;
		return false;
	}
	
	if (point == '.') {
		shotHit = false;
		matrix[p.r][p.c] = 'o';
		return true;
	}
	if (ship != -1) { //we did hit a ship with shipId = ship
		shotHit = true;
		matrix[p.r][p.c] = 'X';
		
		//check whether ship fully destroyed
		int length = m_game.shipLength(ship);
		char ship_symb = m_game.shipSymbol(ship);
		bool stillLeft = false;
		//look length units in each direction from point to see if there are more
		//instances of ship symbol occuring, if there is more ship left
		for (int i = 0; i < length; i++) {
			if ((p.r + i) < m_game.rows() && matrix[p.r + i][p.c] == ship_symb) {
				stillLeft = true;
			}
			if ((p.c + i) < m_game.cols() && matrix[p.r][p.c+i] == ship_symb) {
				stillLeft = true;
			}
			if ((p.r - i) >=0 && matrix[p.r - i][p.c] == ship_symb) {
				stillLeft = true;
			}
			if ((p.c - i) >= 0 && matrix[p.r][p.c-i] == ship_symb) {
				stillLeft = true;
			}
		}
		if (stillLeft) { //not fully destroyed
			//so shot hit but ship not destroyed
			shipDestroyed = false;
			return true;
		}
		if (!stillLeft) {
			shipDestroyed = true;
			shipDestroyedCount++;
			shipId = ship;
			return true;
		}
	}
	//if we got this far must be the case that the shot was successful?
	return true;
}

bool BoardImpl::allShipsDestroyed() const
{
	//check entire matrix, if it contains only X,o, and ., all ships are destroyed

	//or alternatively count each ship thats destroyed
	//and if its the same number as the amount of ships in the game
	// then all ships are destroyed 
	if (shipDestroyedCount == m_game.nShips()) {
		return true;
	}
    return false; // This compiles, but may not be correct
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
