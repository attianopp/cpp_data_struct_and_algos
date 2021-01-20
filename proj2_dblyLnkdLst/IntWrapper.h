
#ifndef INTWRAPPER_INCLUDED
#define INTWRAPPER_INCLUDED
#include <iostream>
// IntWrapper simply stores and retrieves an int.  However, every
// constructor, the destructor, and the assignment operator report
// when they're called.

class IntWrapper
{
public:
	IntWrapper();
	IntWrapper(int i);
	IntWrapper(const IntWrapper& other);
	~IntWrapper();
	IntWrapper& operator=(const IntWrapper& rhs);
	int value() const;
	friend std::ostream& operator<<(std::ostream& os, const IntWrapper& obj);
private:
	int m_value;
};

bool operator==(const IntWrapper& lhs, const IntWrapper& rhs);
bool operator!=(const IntWrapper& lhs, const IntWrapper& rhs);
bool operator< (const IntWrapper& lhs, const IntWrapper& rhs);
bool operator<=(const IntWrapper& lhs, const IntWrapper& rhs);
bool operator> (const IntWrapper& lhs, const IntWrapper& rhs);
bool operator>=(const IntWrapper& lhs, const IntWrapper& rhs);

#endif // INTWRAPPER_INCLUDED