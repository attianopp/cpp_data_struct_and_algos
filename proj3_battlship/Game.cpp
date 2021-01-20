#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);

private:
	int n_rows;
	int n_cols;
	int n_shipCount;
	vector<char> n_shipChars; //re-sizeable array of ships
	vector<string> n_shipNames; // names of corresponding ships
	vector<int> n_shipLengths;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{	
	n_rows = nRows;
	n_cols = nCols;
	n_shipCount = 0;
}

int GameImpl::rows() const
{
    return n_rows;  // This compiles but may not be correct
}

int GameImpl::cols() const
{
    return n_cols;  // This compiles but may not be correct
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
	n_shipCount++;
	n_shipLengths.push_back(length);
	n_shipNames.push_back(name);
	n_shipChars.push_back(symbol);
    return true;  // This compiles but may not be correct
}

int GameImpl::nShips() const
{
    return n_shipCount;  // This compiles but may not be correct
}

int GameImpl::shipLength(int shipId) const
{
    return n_shipLengths[shipId];  // This compiles but may not be correct
}

char GameImpl::shipSymbol(int shipId) const
{
    return n_shipChars[shipId];  // This compiles but may not be correct
}

string GameImpl::shipName(int shipId) const
{
    return n_shipNames[shipId];  // This compiles but may not be correct
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
	if (!p1->placeShips(b1) || !p2->placeShips(b2)) {
		cerr << "returning nullptr because couldnt place all ships" << endl;
		return nullptr;
	}
	
	Board* board1 = &b1;
	Board* board2 = &b2;
	Player* player1 = p1;
	Player* player2 = p2;

	while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {
		
		//output before attack
		
		cout << player1->name() << "'s turn. Board for " << player2->name() << ':' << endl;
		if (player1->isHuman()) {
			board2->display(true);
		}
		else {
			board2->display(false);
		}
		
		//attacking
		bool shothit = false;
		bool shipdestroyed = false;
		int shipid = 0;
		Point Point1 = player1->recommendAttack();
		bool validshot = board2->attack(Point1, shothit, shipdestroyed, shipid);
		player1->recordAttackResult(Point1, validshot, shothit, shipdestroyed, shipid);
		player2->recordAttackByOpponent(Point1);
		
		//output after attack
		
		if (!validshot) {
		cout << player1->name() << " wasted a shot at (" << Point1.r << ',' << Point1.c << ")." << endl;
		}
		else {
			if (!shothit) {
				cout << player1->name() << " attacked (" << Point1.r << ',' << Point1.c << ") and missed, resulting in:" << endl;
			}
			else if (shothit && !shipdestroyed) {
				cout << player1->name() << " attacked (" << Point1.r << ',' << Point1.c << ") and hit something, resulting in:" << endl;
			}
			else if (shothit && shipdestroyed) {
				cout << player1->name() << " attacked (" << Point1.r << ',' << Point1.c << ") and destroyed the " << shipName(shipid) << ", resulting in:" << endl;
			}
			if (player1->isHuman()) {
				board2->display(true);
			}
			else {
				board2->display(false);
			}
		}
		//reversing player and board roles
		Board* boardtemp = board1;
		board1 = board2;
		board2 = boardtemp;

		Player* playertemp = player1;
		player1 = player2;
		player2 = playertemp;

		if (shouldPause && !b1.allShipsDestroyed() && !b2.allShipsDestroyed()) {
			waitForEnter();
		}
	}
	if (b1.allShipsDestroyed()) { // if player1 lost
		cout << p2->name() << " wins!" << endl;
		if (p1->isHuman()) { 
			cout << "Here's where " << p2->name() << "'s ships were:" << endl;
			b2.display(false); }
		return p2;
	}
	if (b2.allShipsDestroyed()) { // if player2 lost
		cout << p1->name() << " wins!" << endl;
		if (p2->isHuman()) { 
			cout << "Here's where " << p1->name() << "'s ships were:" << endl;
			b1.display(false); }
		return p1;
	}
	else {
		cerr << "ERROR: game ran but neither person destroyed all ships" << endl;
		cerr << "inexplicable error" << endl;
		return nullptr;
	}
	
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

