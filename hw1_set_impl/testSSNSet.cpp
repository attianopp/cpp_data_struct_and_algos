#include "SSNSet.h"
#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main()
{
	SSNSet s;
	assert(s.size() == 0);
	unsigned long j = 10029923;
	unsigned long i = 1009234315;
	unsigned long k = 10029923;
	assert(s.add(j));
	assert(s.add(i));
	assert(!s.add(k));
	s.print();

}