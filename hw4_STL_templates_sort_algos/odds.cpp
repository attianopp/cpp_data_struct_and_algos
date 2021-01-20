//TAKE OUT THE INCLUDE STATEMENTS
/*
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;
*/
// Remove the odd integers from v.
// It is acceptable if the order of the remaining even integers is not
// the same as in the original vector.
void removeOdds(vector<int>& v)
{
	if (!v.empty()) {
		for (vector<int>::iterator i = v.begin(); i !=v.end(); i++) {
			if ( (*i)%2 != 0) {
				i=v.erase(i);
				i--;
			}
		}
	}
}