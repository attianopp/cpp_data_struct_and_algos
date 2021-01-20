#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

Set::Set() :msize(0)
{
	//let the array default constructor be called since its called anyway
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
	if(Set::contains(value) == false){
		if ((msize + 1) > DEFAULT_MAX_ITEMS) {
			return false;
		}
		msize++;
		set[msize-1] = value;
		return true;
	}
	else {
		return false;
	}
}

bool Set::erase(const ItemType & value)
{
	if (Set::contains(value)==true) {
		int index = 0;
		for (int i = 0; i < msize; i++) {
			if (set[i] == value) {
				index=i;
			}
		}
		for (int j = index; j < (msize-1); j++) {
			set[j] = set[j + 1];
		}
		if ((msize-1) < 0) {
			return false;
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
	if (i>=0 && i < msize) {
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
	int tempsize = other.msize;
	ItemType temp;
	if (tempsize > msize){
		for (int i = 0; i < tempsize; i++) {
			temp = set[i];
			set[i] = other.set[i];
			other.set[i] = temp;
		}
	}
	else if (msize > tempsize) {
		for (int i = 0; i < msize; i++) {
			temp = set[i];
			set[i] = other.set[i];
			other.set[i] = temp;
		}
	}
	other.msize = msize;
	msize = tempsize;
}
