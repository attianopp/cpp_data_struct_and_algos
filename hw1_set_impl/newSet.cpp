#include "newSet.h"
#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>


Set::Set(const int size) :msize(0),maxsize(size)
{
	if (maxsize >= 0) {
		set = new ItemType[maxsize];
	}
	else if(maxsize<0) {
		std::cerr << "ERROR: Size input is negative" << std::endl;
		std::exit(1);
	}
	
}

bool Set::empty() const
{
	if (msize == 0) {
		return true;
	}
	else {
		return false;
	}
}

int Set::size() const
{
	return msize;
}

bool Set::insert(const ItemType & value)
{
	if (Set::contains(value) == false) {
		if ((msize + 1) > maxsize) {
			return false;
		}
		msize++;
		set[msize - 1] = value;
		return true;
	}
	else {
		return false;
	}
}

bool Set::erase(const ItemType & value)
{
	if (Set::contains(value) == true) {
		int index = 0;
		for (int i = 0; i < msize; i++) {
			if (set[i] == value) {
				index = i;
			}
		}
		if ((msize - 1) < 0) {
			return false;
		}
		for (int j = index; j < (msize-1); j++) {
			set[j] = set[j + 1];
		}
		msize--;
		return true;
	}
	else {
		return false;
	}
}

bool Set::contains(const ItemType & value) const
{
	for (int i = 0; i < msize; i++) {
		if (set[i] == value) {
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType & value) const
{
	int count = 0;
	if (i >= 0 && i < msize) {
		for (int j = 0; j < msize; j++) {
			count = 0;
			for (int index = 0; index < msize; index++) {
				if (set[j] > set[index]) {
					count++;
				}
			}
			if (count == i) {
				value = set[j];
				return true;
			}
		}
	}
	return false;

}

void Set::swap(Set & other)
{
	ItemType* temp = other.set;
	int tempsize = other.msize;
	int tempmaxsize = other.maxsize;
	other.set = set;
	set = temp;
	other.msize = msize;
	other.maxsize = maxsize;
	msize = tempsize;
	maxsize = tempmaxsize;

}

Set::~Set()
{
	delete [] set;
}

Set::Set(const Set & other)
{
	msize = other.msize;
	maxsize = other.maxsize;
	set = new ItemType[maxsize];
	for (int i = 0; i < other.msize; i++) {
		set[i] = other.set[i];
	}
}

Set & Set::operator=(const Set & rhs)
{
	if (this != &rhs)
	{
		Set temp(rhs);
		Set::swap(temp);
	}
	return *this;
}
