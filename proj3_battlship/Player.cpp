#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <stack>

using namespace std;


//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
	HumanPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual bool isHuman() const { return true; }
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
private:
	Point m_lastCellAttacked;
};

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.

class MediocrePlayer : public Player
{
  public:
	 MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
	struct ship
	{
		Direction direc;
		int id;
		Point topOrLeft;

	};
  private:
    Point m_lastCellAttacked; //for good player
	Point m_lastCellHit;
	
	//for attack functions
	//current state takes value 1 or 2
	// last_state only takes states 1 or 2
	int current_state;
	int last_state;
	stack<Point> state2_attack_points;
	bool last_shot_hit;
	bool destroyed_ship;
	//for attack functions 
	// to check if points returned by recommend attack are already checked
	vector<Point> attack_points_checked;
	//helper function for checking if the point passed in is in the vector of points already checked
	bool not_checked(Point p);
	

	// for placeships function
	bool placeAllShips(int shipsLeft, Board& b);


};

//*********************************************************************
//  GoodPlayer
//*********************************************************************


class GoodPlayer : public Player
{
public:
	GoodPlayer(string nm, const Game& g);
	virtual bool placeShips(Board& b);
	virtual Point recommendAttack();
	virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
		bool shipDestroyed, int shipId);
	virtual void recordAttackByOpponent(Point p);
private:
	// private members for attack
	Point m_lastCellAttacked;
	Point m_lastCellHit;
	Point m_SecondHit;
	Point m_firstHit;
	// 3 states for good player: 1->hunt, 2->hit so look for second hit outwards
	// 3->found second hit so either attack vertically or horizontally until destroyed
	//or until state 3 stack runs out, then exhaust stack 2 until something is destroyed
	//or stack 2 runs out
	//if state 1 hunt cant find a parity point, find a random point that hasnt been hit
	int current_state;
	int last_state;
	stack<Point> state2_attack_points;
	stack<Point> state3_upOrDown;
	vector<Point> attack_points_checked;
	bool not_checked(Point p); // helper function to easily check if point was already attacked
	bool last_shot_hit;
	bool destroyed_ship;
	vector<vector<char> > attackBoard; // so good player only attacks every other row
	int largest_ship_size;
	//to take advantage of parity

	//for place ships function
	vector<vector<char> > shipsPlacedBoard;
	//recursive helper function for placing all ships
	bool placeAllShips(int shipsLeft, Board& b);
};

//*********************************************************************
//  createPlayer
//*********************************************************************


Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}

HumanPlayer::HumanPlayer(string nm, const Game & g)
	: Player(nm, g), m_lastCellAttacked(0, 0)
{
}

bool HumanPlayer::placeShips(Board & b)
{
	cout << name() << " must place " << game().nShips() << " ships." << endl;
	
	for (int i = 0; i < game().nShips(); i++) {
		b.display(false);
		int length = game().shipLength(i);
		string name = game().shipName(i);
		char direc;

		while (true) { // waiting for correct input for direction
			cout << "Enter h or v for direction of " << name << " (length " << length << "): ";
			cin >> direc;
			if ((!cin)
				|| (direc != 'h' && direc != 'v')) //superflous?
			{
				cin.clear();
				cin.ignore(numeric_limits <streamsize> ::max(), '\n');
				cout << "Direction must be h or v.\n";
			}
			else {
				cin.ignore(numeric_limits <streamsize> ::max(), '\n');
				break;
			}
		}

		// direction must be equal to h or v at this point
		if (direc == 'h') {
			int r, c;

			while (true) { // waiting for correct input for cell
				cout << "Enter row and column of leftmost cell (e.g, 3 5): ";
				cin >> r >> c;
				if ((!cin)) //superflous?
				{
					cin.clear();
					cin.ignore(numeric_limits <streamsize> ::max(), '\n');
					cout << "You must enter two integers." << endl;
				}
				else if (!b.placeShip(Point(r, c),i,HORIZONTAL)) { //if the ship cant be placed there
					cin.clear();
					cin.ignore(numeric_limits <streamsize> ::max(), '\n');
					cout << "The ship can not be placed there." << endl;
				}
				else {
					cin.ignore(numeric_limits <streamsize> ::max(), '\n');
					break;
				}
			}

			//b.placeShip(Point(r, c), i, HORIZONTAL);
		}
		if (direc == 'v') {
			int r, c;

			while (true) { // waiting for correct input for cell
				cout << "Enter row and column of leftmost cell (e.g, 3 5): ";
				cin >> r >> c;
				if ((!cin)) //superflous?
				{
					cin.clear();
					cin.ignore(numeric_limits <streamsize> ::max(), '\n');
					cout << "You must enter two integers." << endl;
				}
				else if (!b.placeShip(Point(r, c), i, VERTICAL)) { //if the ship cant be placed there
					cin.clear();
					cin.ignore(numeric_limits <streamsize> ::max(), '\n');
					cout << "The ship can not be placed there." << endl;
				}
				else {
					cin.ignore(numeric_limits <streamsize> ::max(), '\n');
					break;
				}
			}

			//b.placeShip(Point(r, c), i, VERTICAL);
		}
	}
	// the code got to this point only after placing all ships
	return true;
}

Point HumanPlayer::recommendAttack()
{
	cout << "Enter the row and column to attack (e.g, 3 5): ";
	int r, c;
	while (!getLineWithTwoIntegers(r, c)) {
		cout << "You must enter two integers." << endl;
		cout << "Enter the row and column to attack (e.g, 3 5): ";
	}
	return Point(r,c);
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	//implemented fine
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
	//implemented fine
}


MediocrePlayer::MediocrePlayer(string nm, const Game & g)
: Player(nm, g), m_lastCellAttacked(-1, -1), m_lastCellHit(0, 0), current_state(1), last_state(1), last_shot_hit(false), destroyed_ship(false)
{
}
bool MediocrePlayer::placeShips(Board & b)
{

	for (int numTries = 0; numTries < 50; numTries++) {
		b.block(); //step 1

		//step 2 of spec here
		bool could_place_all = placeAllShips(game().nShips(), b);
		
		b.unblock(); //step 3
		//step 4
		if (could_place_all) {
			return true;
		}
	}
	return false;
	
	// Clustering ships is bad strategy
	/*
	for (int k = 0; k < game().nShips(); k++)
		if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
			return false;
	return true;*/
}
Point MediocrePlayer::recommendAttack()
{
	if (current_state==1) {
		if (last_state == 2) {
			while (!state2_attack_points.empty()) {
				state2_attack_points.pop();
			}
		}
		Point p = game().randomPoint();
		if (!not_checked(p)) {
			while (!not_checked(p)) {
				p = game().randomPoint();
			}
		}
		attack_points_checked.push_back(p);
		return p;
	}
	else { //current_state should == 2

		//error testing
	//	cerr << "current state: " << current_state << endl;
	//	cerr << "last state: " << current_state << endl;
	//	assert(current_state == 2);


		if (last_state == 1) { //add all the points to attack 
		//	cerr << "should get here after first hit" << endl;
			Point p = m_lastCellHit;
			if (p.r == -1 || p.c == -1) {
				cerr << "ERROR :m_lastCellHit is not updated to a new cell since consturction" << endl;
				cerr << "making point p equal last cell attacked but erroneous" << endl;
				p = m_lastCellAttacked;
			}
			// add all valid points in each  direction to stack
			// four for loops, i starts at 1 goes to i <5
			// N = r - 1, S = r + 1, E = c + 1, W = c - 1
			for (int i = 4; i >=1; i--) { //north
			//	cerr << "adding north " << i << endl;
				if (game().isValid(Point(p.r - i, p.c))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r - i, p.c));
				}
		
			//	cerr << "adding south " << i << endl;
				if (game().isValid(Point(p.r + i, p.c))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r + i, p.c));
				}
			
			//	cerr << "adding east " << i << endl;
				if (game().isValid(Point(p.r, p.c + i))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r, p.c + i));
				}
			
			//	cerr << "adding west " << i << endl;
				if (game().isValid(Point(p.r, p.c - i))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r, p.c - i));
				}
			}
			if (!state2_attack_points.empty()) { // shouldnt be empty just error checking
			//	cerr << "returning a value from the stack like normal for state 2" << endl;
				Point next = state2_attack_points.top();
				state2_attack_points.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = state2_attack_points.top();
						state2_attack_points.pop();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else {
				cerr << "why is the stack empty after trying to add all the points after a hit" << endl;
				cerr << "this error message should never show when running the game" << endl;
				Point next = game().randomPoint();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = game().randomPoint();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}

		}
		if (last_state == 2) {// point a point off and return that point
			if (!state2_attack_points.empty()) {
				Point next = state2_attack_points.top();
				state2_attack_points.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = state2_attack_points.top();
						state2_attack_points.pop();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else { //error or length of ships are 6 or more
				cerr << "stack is empty for some reason after staying in state_2" << endl;
				cerr << "returning a randompoint because I'm forced too" << endl;
				current_state = 1;
				last_state = 2;
				Point next = game().randomPoint();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = game().randomPoint();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
		}
	}
	cerr << "ERROR: Somehow didnt enter logic paths for states" << endl;
	cerr << "code should have never gotten to this point, returning a random point" << endl;
	Point next = game().randomPoint();
	if (!not_checked(next)) {
		while (!not_checked(next)) {
			next = game().randomPoint();
		}
	}
	attack_points_checked.push_back(next);
	return next;
}
void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	m_lastCellAttacked = p;
	if (validShot) {

		if (shotHit) {
			last_shot_hit = true;
			m_lastCellHit = p;
			if (shipDestroyed) {
				destroyed_ship = true;
				if (current_state == 1) {
					last_state = 1;
					current_state = 1;
				}
				else if (current_state == 2) {
					last_state = 2;
					current_state = 1;
				}
			}
			else { //attack hits a ship but doesnt destroy it
				destroyed_ship = false;
				if (current_state == 1) {
					last_state = 1;
					current_state = 2;
				}
				else if (current_state == 2) {
					last_state = 2;
					current_state = 2;
				}
			}
		}
		else {
			if (current_state == 1) {
				last_state = 1;
				current_state = 1;
			}
			else if (current_state == 2) {
				last_state = 2;
				current_state = 2;
			}
			last_shot_hit = false;
		}
	}
	else {
		cerr << "INVALID SHOT FOR MEDIOCRE PLAYER"<<name()<<": look into why" << endl;
	}
	//cerr << "record attack for"<< name() <<" :" << endl;
	//cerr << "	current state: " << current_state << endl;
	//cerr << "	last state: " << last_state << endl;

}
void MediocrePlayer::recordAttackByOpponent(Point p)
{
	//implemented fine
}
bool MediocrePlayer::not_checked(Point p)
{
	for (int i = 0; i < static_cast<int>(attack_points_checked.size()); i++) {
		if (attack_points_checked[i].r == p.r && attack_points_checked[i].c == p.c) {
			return false;
		}
	}
	return true;
}
bool MediocrePlayer::placeAllShips(int shipsLeft, Board& b)
{
	if (shipsLeft == 0) { //successfully placed all ships recursively
		return true;
	}
	for (int i = 0; i < game().rows(); i++) { //row in question
		for (int j = 0; j < game().cols(); j++) { //col in question
			Point p(i,j);
			if (b.placeShip(p, game().nShips()-shipsLeft, HORIZONTAL)) { //trying horizontal placement
				bool placed = placeAllShips((shipsLeft-1), b);
				if (placed) {
					return true;
				}
				else {
					b.unplaceShip(p, game().nShips() - shipsLeft, HORIZONTAL);
				}
			}
			else if (b.placeShip(p, game().nShips() - shipsLeft, VERTICAL)) { //trying vertical placement
				bool placed = placeAllShips((shipsLeft - 1), b); //place next ship
				if (placed) {
					return true;
				}
				else {
					b.unplaceShip(p, game().nShips() - shipsLeft, VERTICAL);
				}
			}
		}
	}
	return false;
}

GoodPlayer::GoodPlayer(string nm, const Game & g)
: Player(nm, g), m_lastCellAttacked(-1, -1), m_lastCellHit(0,0), m_SecondHit(-1,-1), m_firstHit(-1,-1)
{
	largest_ship_size = 5;
	current_state = 1;
	last_state = 1;
	last_shot_hit = false;
	destroyed_ship = false;
	//initializing both boards for attack and placeships
	attackBoard.resize(g.rows(), vector<char>(g.cols(), '.'));
	shipsPlacedBoard.resize(g.rows(), vector<char>(g.cols(), '.'));
	//making attack board observe parity
	for (int i = 0; i < static_cast<int>(attackBoard.size()); i++) {
		for (int j = 0; j < static_cast<int>(attackBoard[i].size()); j++) {
			if (i % 2 == 0) { //if row is even and col is even then observe parity
				if (j % 2 == 0) {
					attackBoard[i][j] = 'p';
				}
			}
			if (i % 2 != 0) { //not odd so must be even
				if (j % 2 != 0) { //if both row and column are odd observe parity
					attackBoard[i][j] = 'p';
				}
			}
		}
	}

}

//TODO
bool GoodPlayer::placeShips(Board & b)
{
	//place randomly, make sure ships arent touching
	// if not possible to place not touching, just place

	//belongs in an else statement
	for (int numTries = 0; numTries < 50; numTries++) {
		b.block(); //step 1

				   //step 2 of spec here
		bool could_place_all = placeAllShips(game().nShips(), b);

		b.unblock(); //step 3
					 //step 4
		if (could_place_all) {
			return true;
		}
	}
	return false;
}

// COMMENT OUT CERR STATEMENTS AFTER YOU KNOW ITS WORKING FOR TIME < 5 seconds

Point GoodPlayer::recommendAttack()
{ 
	//3 states
	// one attack diagonal patter
	// second hit a ship and branches out
	// thrid found either horizontal or vertical orientation
	if (current_state == 1) {
		if (last_state == 2) {
			while (!state2_attack_points.empty()) {
				state2_attack_points.pop();
			}
		}
		if (last_state == 3) { //emptying old places to shoot
			while (!state2_attack_points.empty()) {
				state2_attack_points.pop();
			}
			while (!state3_upOrDown.empty()) {
				state3_upOrDown.pop();
			}
		}

		for (int i = 0; i < static_cast<int>(attackBoard.size()); i++) {
			for (int j = 0; j < static_cast<int>(attackBoard[i].size()); j++) {
				Point p(i, j);
				if (not_checked(p) && attackBoard[p.r][p.c] == 'p') {
					attack_points_checked.push_back(p);
					return p;
				}
			}
		}
		//checked every parity point first

		Point next = game().randomPoint();
		if (!not_checked(next)) {
			while (!not_checked(next)) {
				next = game().randomPoint();
			}
		}
		attack_points_checked.push_back(next);
		return next;

		//old code for this case
		/*
		Point p = game().randomPoint();
		int count = 0;
		if (!not_checked(p) || attackBoard[p.r][p.c]!='p') {
			while (!not_checked(p) || attackBoard[p.r][p.c] != 'p') {
				p = game().randomPoint();
				count++;
				if (count > 65) { // takes too long so we gotta stop early?
					if (not_checked(p)) { // even if we break dont use a point we already checked
						break;
					}
				}
			}
		}
		attack_points_checked.push_back(p);
		return p;*/
		
	}
	else if (current_state == 2){ 

		   //error testing
		//cerr << "current state: " << current_state << endl;
		//cerr << "last state: " << current_state << endl;
		assert(current_state == 2);


		if (last_state == 1) { //add all the points to attack 
			//cerr << "should get here after first hit" << endl;
			Point p = m_lastCellHit;
			m_firstHit = m_lastCellHit;
			if (p.r == -1 || p.c == -1) {
				cerr << "ERROR :m_lastCellHit is not updated to a new cell since consturction" << endl;
				cerr << "making point p equal last cell attacked but erroneous" << endl;
				p = m_lastCellAttacked;
			}
			// add all valid points in each  direction to stack
			// four for loops, i starts at 1 goes to i <5
			// N = r - 1, S = r + 1, E = c + 1, W = c - 1
			for (int i = (largest_ship_size-1); i >= 1; i--) { //north
			//	cerr << "adding north " << i << endl;
				if (game().isValid(Point(p.r - i, p.c))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r - i, p.c));
				}

			//	cerr << "adding south " << i << endl;
				if (game().isValid(Point(p.r + i, p.c))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r + i, p.c));
				}

			//	cerr << "adding east " << i << endl;
				if (game().isValid(Point(p.r, p.c + i))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r, p.c + i));
				}

			//	cerr << "adding west " << i << endl;
				if (game().isValid(Point(p.r, p.c - i))) {
			//		cerr << "	successfully added" << endl;
					state2_attack_points.push(Point(p.r, p.c - i));
				}
			}
			if (!state2_attack_points.empty()) { // shouldnt be empty just error checking
				//cerr << "returning a value from the stack like normal for state 2" << endl;
				Point next = state2_attack_points.top();
				state2_attack_points.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						if (!state2_attack_points.empty()) {
							next = state2_attack_points.top();
							state2_attack_points.pop();
						}
						else {
							last_state = 2;
							current_state = 1;
							next = game().randomPoint();
						}
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else {
				cerr << "why is the stack empty after trying to add all the points after a hit" << endl;
				cerr << "this error message should never show when running the game" << endl;
				Point next = game().randomPoint();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = game().randomPoint();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}

		}
		if (last_state == 2 || last_state == 3) {// point a point off and return that point
			if (!state2_attack_points.empty()) {
				Point next = state2_attack_points.top();
				state2_attack_points.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						if (!state2_attack_points.empty()) {
							next = state2_attack_points.top();
							state2_attack_points.pop();
						}
						else {
							last_state = 2;
							current_state = 1;
							next = game().randomPoint();
						}
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else { //error or length of ships are 6 or more or last state

				cerr << "stack is empty for some reason after staying in state_2" << endl;
				cerr << "returning a randompoint because I'm forced too" << endl;
				last_state = current_state;
				current_state = 1;
				Point next = game().randomPoint();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = game().randomPoint();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
		}
	
	}
	else {//current state should be 3
	//	cerr << "current state: " << current_state << endl;
	//	cerr << "last state: " << current_state << endl;
	//	assert(current_state == 3);

		// can only go to point 3 after state 2 or staying in state 3
		// can go back to state 2 if state 3 doesnt lead to desctruction of a ship
		if (last_state == 2) { // just got a second hit
							   // start hunting north/south or east/west depending on last input
			Point second = m_SecondHit;
			Point first = m_firstHit;
			if (second.r == -1 || second.c == -1 || first.r == -1 || first.c == -1) {
				cerr << "ERROR : first and second hit have not been updated to a new cell since consturction" << endl;
				cerr << "making point p equal last cell attacked but erroneous" << endl;
				first = second = m_lastCellAttacked; 
				//? or do I just want to go to state 2 and return from the stack and if that doesnt work return random point
			}
			// else second hit and first hit are updated
			// check if last and second hit are vertical or horizontal
			if (first.r == (second.r - 1) || first.r == (second.r + 1)) { // ship was placed vertically
				for (int i = (largest_ship_size - 1); i >= 1; i--) { //add
	//				cerr << "adding north " << i << endl;
					if (game().isValid(Point(first.r - i, first.c))) {
	//					cerr << "	successfully added" << endl;
						state3_upOrDown.push(Point(first.r - i, first.c));
					}

	//				cerr << "adding south " << i << endl;
					if (game().isValid(Point(first.r + i, first.c))) {
		//				cerr << "	successfully added" << endl;
						state3_upOrDown.push(Point(first.r + i, first.c));
					}
				}
			}
			else if (first.c == (second.c - 1) || first.c == (second.c + 1)) { //placed horizontal EAST O WEST
				for (int i = (largest_ship_size - 1); i >= 1; i--) {
		//			cerr << "adding east " << i << endl;
					if (game().isValid(Point(first.r, first.c + i))) {
					//	cerr << "	successfully added" << endl;
						state3_upOrDown.push(Point(first.r, first.c + i));
					}

		//			cerr << "adding west " << i << endl;
					if (game().isValid(Point(first.r, first.c - i))) {
			//			cerr << "	successfully added" << endl;
						state3_upOrDown.push(Point(first.r, first.c - i));
					}
				}
			}
			//need to take point off top of stack3 or stack 2 if stack 3 empty
			if (!state3_upOrDown.empty()) {
				Point next = state3_upOrDown.top();
				state3_upOrDown.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						if (!state3_upOrDown.empty()) {
							next = state3_upOrDown.top();
							state3_upOrDown.pop();
						}
						else if (!state2_attack_points.empty()) {
							next = state2_attack_points.top();
							state2_attack_points.pop();
						}
						else {
							last_state = 3;
							current_state = 1;
							next = game().randomPoint();
						}
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else if (!state2_attack_points.empty()) {

			//	cerr << "stack 3 empty for some reason so moving to stack 2 and state 2" << endl;
				last_state = current_state;
				current_state = 2;

				Point next = state2_attack_points.top();
				state2_attack_points.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						if (!state2_attack_points.empty()) {
							next = state2_attack_points.top();
							state2_attack_points.pop();
						}
						else {
							last_state = 3;
							current_state = 1;
							next = game().randomPoint();
						}
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else {
				cerr << "both stack 3 and stack 2 are empty for some reason in state 3" << endl;
				cerr << "returning a randompoint because I'm forced too" << endl;
				last_state = current_state;
				current_state = 1;
				Point next = game().randomPoint();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = game().randomPoint();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
		}
		else if (last_state == 3) { //hunting north/south or east/west
									// or if empty stack go to state two hunt 
									// if stack two empty return random unchecked point and go to state 1
			if (!state3_upOrDown.empty()) {
				Point next = state3_upOrDown.top();
				state3_upOrDown.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						if (!state3_upOrDown.empty()) {
							next = state3_upOrDown.top();
							state3_upOrDown.pop();
						}
						else if (!state2_attack_points.empty()) {
							next = state2_attack_points.top();
							state2_attack_points.pop();
							last_state = 3;
							current_state = 2;
						}
						else {
							last_state = 3;
							current_state = 1;
							next = game().randomPoint();
						}
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else if (!state2_attack_points.empty()) {

			//	cerr << "stack 3 empty for some reason so moving to stack 2 and state 2" << endl;
				last_state = 3;
				current_state = 2;

				Point next = state2_attack_points.top();
				state2_attack_points.pop();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						if (!state2_attack_points.empty()) {
							next = state2_attack_points.top();
							state2_attack_points.pop();
						}
						else {
							last_state = current_state;
							current_state = 1;
							next = game().randomPoint();
						}
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
			else {
				cerr << "both stack 3 and stack 2 are empty for some reason in state 3" << endl;
				cerr << "returning a randompoint because I'm forced too" << endl;
				last_state = 3;
				current_state = 1;
				Point next = game().randomPoint();
				if (!not_checked(next)) {
					while (!not_checked(next)) {
						next = game().randomPoint();
					}
				}
				attack_points_checked.push_back(next);
				return next;
			}
		}
		else { //state 1? error just return a random non checked point
			last_state = current_state;
			current_state = 1;
			cerr << "ERROR: current state 3 last state was 1" << endl;
			cerr << "how did we get here? returning random unchecked point" << endl;
			Point next = game().randomPoint();
			if (!not_checked(next)) {
				while (!not_checked(next)) {
					next = game().randomPoint();
				}
			}
			attack_points_checked.push_back(next);
			return next;
		}

	}
	cerr << "ERROR: Somehow didnt enter logic paths for states" << endl;
	cerr << "code should have never gotten to this point, returning a random point" << endl;
	last_state = current_state;
	current_state = 1;
	Point next = game().randomPoint();
	if (!not_checked(next)) {
		while (!not_checked(next)) {
			next = game().randomPoint();
		}
	}
	attack_points_checked.push_back(next);
	return next;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
	//attacks in a diagonal pattern
	// good player records last two shots hit
	// then only attacks vertically or horizontal, need state 3

	m_lastCellAttacked = p;
	if (validShot) {

		if (shotHit) {
			last_shot_hit = true;
			m_lastCellHit = p;
			if (current_state == 2) { //if already hit once
				m_SecondHit = p;
			}
			else if (current_state == 1) {
				Point reset(-1, -1);
				m_SecondHit = reset;
			}
			if (shipDestroyed) {
				destroyed_ship = true;
				if (current_state == 1) {
					last_state = 1;
					current_state = 1;
				}
				else if (current_state == 2) {
					last_state = 2;
					current_state = 1;
				}
				else if (current_state == 3) {
					last_state = 3;
					current_state = 1;
				}

				// 3rd parity if patrol boat destroyed
				if (game().shipLength(shipId) == 2) {
					for (int i = 0; i < static_cast<int>(attackBoard.size()); i++) {
						for (int j = 0; j < static_cast<int>(attackBoard[i].size()); j++) {
							attackBoard[i][j] = '.';
							if (i % 3 == 0) { //if row is even and col is even then observe parity
								if (j % 3 == 0) {
									attackBoard[i][j] = 'p';
								}
							}
							if (i % 3 == 1) { //not odd so must be even
								if (j % 3 == 1) { //if both row and column are odd observe parity
									attackBoard[i][j] = 'p';
								}
							}
							if (i % 3 == 2) { //not odd so must be even
								if (j % 3 == 2) { //if both row and column are odd observe parity
									attackBoard[i][j] = 'p';
								}
							}
						}
					}
				}

				// accounting for largest ship size destroyed
				if (largest_ship_size == 4 && game().shipLength(shipId) == 4) {
					//already destroyed aircraft carrier
					//now destroyed battleship next largest shp
					largest_ship_size = 3;
				}
				if (game().shipLength(shipId) == 5) { // destroyed the aircraft carrier
													  //next largest ship is size 4
					largest_ship_size = 4;
				}

			}
			else { //attack hits a ship but doesnt destroy it
				destroyed_ship = false;
				if (current_state == 1) {
					last_state = 1;
					current_state = 2;
				}
				else if (current_state == 2) {
					last_state = 2;
					current_state = 3;
				}
				else if (current_state == 3) {
					last_state = 3;
					current_state = 3;
				}
			}
		}
		else {
			if (current_state == 1) {
				last_state = 1;
				current_state = 1;
			}
			else if (current_state == 2) {
				last_state = 2;
				current_state = 2;
			}
			else if (current_state == 3) {
				last_state = 3;
				current_state = 3;
			}
			last_shot_hit = false;
		}
	}
	else {
		cerr << "INVALID SHOT FOR MEDIOCRE PLAYER" << name() << ": look into why" << endl;
	}
	//cerr << "record attack for" << name() << " :" << endl;
	//cerr << "	current state: " << current_state << endl;
	//cerr << "	last state: " << last_state << endl;
}

void GoodPlayer::recordAttackByOpponent(Point p)
{
	//no clue how to implement
	//how would good player change their attack pattern?
	// in reality no one changes their attack pattern based on how close they are to losing
}

bool GoodPlayer::not_checked(Point p)
{
	for (int i = 0; i < static_cast<int>(attack_points_checked.size()); i++) {
		if (attack_points_checked[i].r == p.r && attack_points_checked[i].c == p.c) {
			return false;
		}
	}
	return true;
}

bool GoodPlayer::placeAllShips(int shipsLeft, Board & b)
{
	if (shipsLeft == 0) { //successfully placed all ships recursively
		return true;
	}
	for (int i = 0; i < game().rows(); i++) { //row in question
		for (int j = 0; j < game().cols(); j++) { //col in question
			Point p(i, j);
			if (b.placeShip(p, game().nShips() - shipsLeft, HORIZONTAL)) { //trying horizontal placement
				bool placed = placeAllShips((shipsLeft - 1), b);
				if (placed) {
					return true;
				}
				else {
					b.unplaceShip(p, game().nShips() - shipsLeft, HORIZONTAL);
				}
			}
			else if (b.placeShip(p, game().nShips() - shipsLeft, VERTICAL)) { //trying vertical placement
				bool placed = placeAllShips((shipsLeft - 1), b); //place next ship
				if (placed) {
					return true;
				}
				else {
					b.unplaceShip(p, game().nShips() - shipsLeft, VERTICAL);
				}
			}
		}
	}
	return false;
}
