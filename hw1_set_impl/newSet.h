#pragma once
#ifndef NEWSET_INCLUDED
#define NEWSET_INCLUDED

#include <string>


const int DEFAULT_MAX_ITEMS = 200;
typedef std::string ItemType;
//typedef unsigned long ItemType;

class Set
{
public:
	Set(const int size = DEFAULT_MAX_ITEMS);         // Create an empty set.

	~Set();

	Set(const Set& other);

	Set& operator=(const Set& rhs);

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

	//invariant: set is an ItemType array capable of storing DEFAULTMAXITEMS, msize is the size of the array and msize>=0
	ItemType* set;
	int msize;
	int maxsize;
};

#endif // !NEWSET_INCLUDED

