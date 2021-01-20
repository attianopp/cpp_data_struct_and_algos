#ifndef SET_INCLUDED
#define SET_INCLUDED

#include <string>

typedef std::string ItemType;
//typedef unsigned long ItemType;

class Set
{
public:
	// Housekeeping functions
	~Set();
	Set(const Set& other);
	Set& operator=(const Set& rhs);
	Set();         // Create an empty set.

	bool empty() const;  // Return true if the set is empty, otherwise false.

	int size() const;    // Return the number of items in the set.

	bool insert(const ItemType& value);
	// Insert value into the set if it is not already present.  Return
	// true if the value was actually inserted.  Leave the set unchanged
	// and return false if the value was not inserted (perhaps because it
	// is already in the set or because the set has a fixed capacity and
	// is full).

	bool erase(const ItemType& value);
	// Remove the value from the set if present.  Return true if the
	// value was removed; otherwise, leave the set unchanged and
	// return false.

	bool contains(const ItemType& value) const;
	// Return true if the value is in the set, otherwise false.

	bool get(int i, ItemType& value) const;
	// If 0 <= i < size(), copy into value the item in the set that is
	// greater than exactly i items in the set and return true.  Otherwise,
	// leave value unchanged and return false.

	void swap(Set& other);
	// Exchange the contents of this set with the other one.

private:

	//invariant: node is a private object in a doubly-linked list holding a single variable ,data, of ItemType type 
	//next and prev are the pointers to the next and previous nodes in the list
	//first node item is dummy node head
	
	struct node {
		ItemType data;
		node* next;
		node* prev;
	};
	
	node* head;// static pointer to keep track of the start of the list
	int msize;
};

void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);


#endif // !SET_INCLUDED