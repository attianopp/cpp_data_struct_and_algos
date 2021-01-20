#include "SSNSet.h"
#include "Set.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet()
{
}

bool SSNSet::add(ItemType ssn)
{
	bool result = set.insert(ssn);
	return result;
}

int SSNSet::size() const
{
	return set.size();
}

void SSNSet::print() const
{
	ItemType x;
	for (int i = 0; i < set.size(); i++) {
		set.get(i, x);
		cout << x << endl;
	}
}
