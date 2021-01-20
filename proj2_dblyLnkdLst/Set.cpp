#include "Set.h"

Set::Set() //initializing set to have a dummy node to simplifiy implementation
{
	head = new node;
	head->next = head;
	head->prev = head;
	msize = 0;
}

Set::~Set()
{
	for (node* p = head->next; p != head;) { //iterating through the list and deleting each node individually
		node* temp = p->next; //need to store pointer to p.next since p gets deleted
		delete p;
		p = temp;
	}
	delete head; //exits loop once p=head so still need to delete head
}

Set::Set(const Set & other):msize(other.msize)
{
	//initializing variables and head
	head = new node;
	int count = 0;
	ItemType tempdata;
	node* tempnext;
	node* tempNode = head;

	//using count to determine the length of the loop so I have more control over where p points at the end of the loop
	for (node*p = other.head->next; count < other.msize; count++) {
		//saving the relevant data from the other list while p still points to it
		tempdata = p->data;
		tempnext = p->next;

		//creating new p node copying over data and attaching it to this list
		p = new node;
		p->data = tempdata;

		//tempnode points to last node in new list
		tempNode->next = p;
		head->prev = p; //p is always at the end of the list so head->prev must point to p and p->next must point to head
		p->next = head;
		p->prev = tempNode;
		
		tempNode = p;//setting tempnode to p in the new list
		p = tempnext; //moving p back to the old lists last value
	}
	if (other.msize == 0) { //for loop doesnt execute if msize = 0 which implies an empty list, make head a dummy node
		head->next = head;
		head->prev = head;
	}
}

Set & Set::operator=(const Set & rhs)
{
	if (this != &rhs) { //if the left hand side doesnt equal the right handside dont execute since its a waste of operation
		Set temp(rhs);
		swap(temp); //creating a copy and copying over rhs to lefthand side
	}
	return *this;
}

bool Set::empty() const
{
	return msize==0;
}

int Set::size() const
{
	return msize;
}

bool Set::insert(const ItemType & value)
{
	if (!this->contains(value)) { //if the set doesnt contain the value create a new node that contains the value
		node* endOfList=head->prev; //end of current list
		node* p = new node;
		p->data = value;
		
		//setting end of list to point to p and p to point back to the old end of the list
		p->prev =endOfList;
		endOfList->next = p;
		
		head->prev = p; //p inserted is always at the end of the list
		p->next = head;
		msize++;
		return true;
	}
	else {
		return false; 
	}
}

bool Set::erase(const ItemType & value)
{
	if ((msize - 1) < 0) { //must be a value to delete in the first place
		return false;
	}
	if (this->contains(value)) { //if the value is in the set, proceed to erase it
		node* ptrToValue;
		for (ptrToValue = head->next; ptrToValue != head && ptrToValue->data != value; ptrToValue = ptrToValue->next) {
			//exits loop either with a ptrTovalue pointing to head or to the node that contains value
			//but the if statement checks to make sure it contains the value, so it must point to the node that contains the value
		}
		if (ptrToValue == head) { //just a sanity check to make sure contains did its job properly
			return false;
		}
		//connecting node to the left of ptrToValue to the node on the right of ptrToValue
		//and connecting node on the right of ptrTovalue to the node on the left of ptrToValue
		ptrToValue->prev->next = ptrToValue->next;
		ptrToValue->next->prev = ptrToValue->prev;

		delete ptrToValue;
		msize--;
		return true;
	}
	return false;
}

bool Set::contains(const ItemType & value) const
{
	for (node* p = head->next; p != head; p = p->next) { //iterating through the list checking to see if it contains value
		if (p->data == value) {
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType & value) const
{
	int count = 0; //count for the number of values node*p is greater than
	if (i >= 0 && i < msize) {
		for (node*p = head->next; p != head; p = p->next) { //iterate through the list checking each item
			count = 0;//resetting count
			for (node*q = head->next; q != head; q = q->next) { //if there the item being checked is greater than another item in the list increment count
				if (p->data > q->data) {
					count++;
				}
			}
			if (count == i) { //found the item that is greater than i items in the list
				value = p->data;
				return true;
			}
		}
	}
	return false;

}

void Set::swap(Set & other)
{
	//swapping head pointers
	node* temp = head;
	head = other.head;
	other.head = temp;
	
	//swapping sizes
	int tempsize = msize;
	msize = other.msize;
	other.msize = tempsize;
}

//unite is far from optimized could just use a temp to store s1, and then swap temp and result
//then use the same for loop to check s2 with result that appends if a match is found

void unite(const Set & s1, const Set & s2, Set & result)
{
	int size1 = s1.size();
	int size2 = s2.size();
	bool matchfound = false; //need a variable to keep track of whether or not a match was found
	bool resultSetisTheSameSetAsS1orS2 = false;
	int countOfMatches = 0;
	bool resultSameAsS1 = false;
	bool resultSameAsS2 = false;
	
	//need to check if result set is same set as s1 or s2
	
	if (result.size() == size1 || result.size() == size2) { //if it has the same size as either set
		if (result.size() == size1) { //if it has the same size as set 1 check to see if all the constituent elements are the same
			for (int i = 0; i < result.size(); i++) {
				ItemType x; // holds s1 element
				ItemType y; //holds result elemnt
				s1.get(i, x);
				result.get(i, y);
				if (x == y) {
					countOfMatches++;
				}
			}
			if (countOfMatches == result.size()) {
				resultSameAsS1 = true;
				resultSetisTheSameSetAsS1orS2 = true;
			}
		}
		if (result.size() == size2) { //checking to see if set 2 has all the same elements as result set if they have the same size
			for (int i = 0; i < result.size(); i++) {
				ItemType x; // holds s2 element
				ItemType y; //holds result elemnt
				s2.get(i, x);
				result.get(i, y);
				if (x == y) {
					countOfMatches++; //counting how many elements match
				}
			}
			if (countOfMatches == result.size()) { //if all elements match they are the same
				resultSameAsS2 = true;
				resultSetisTheSameSetAsS1orS2 = true;
			}
		}
	}

	if (resultSetisTheSameSetAsS1orS2) {
		if (resultSameAsS1) { //if result is the same as s1 add any elements that are in s2 that might not be in result or s1, to result
			for (int i = 0; i < size2; i++) {
				matchfound = false; //need to reset the matchfound bool for a new comparison
				ItemType x;
				s2.get(i, x); // putting value from second set to be compared in x
				for (int j = 0; j < result.size(); j++) {
					ItemType y;
					result.get(j, y); //putting value from the result set in y to compare to x
					if (y == x) {
						matchfound = true;
					}
				}
				if (!matchfound) { //if a match of  variable x isnt found in the second set then append it to result
					result.insert(x);
				}
			}
		}
		if (resultSameAsS2) {//if result is the same as s2 add any elements that are in s1 that might not be in result or s2, to result
			for (int i = 0; i < size1; i++) {
				matchfound = false; //need to reset the matchfound bool for a new comparison
				ItemType x;
				s1.get(i, x); // putting value from first set to be compared in x
				for (int j = 0; j < result.size(); j++) {
					ItemType y;
					result.get(j, y); //putting value from the result set in y to compare to x
					if (y == x) {
						matchfound = true;
					}
				}
				if (!matchfound) { //if a match of  variable x isnt found in the first set then append it to result
					result.insert(x);
				}
			}
		}
	}

	if (!resultSetisTheSameSetAsS1orS2) { 

		if (!result.empty()) { //if the input result Set isnt empty make it empty
			int resultsize = result.size(); //need to have a static variable since erase reduces original size
			for (int i = 0; i < resultsize;) { //looping through entire result set
				ItemType erased;
				result.get(i, erased);
				result.erase(erased);
				resultsize--;
			}
		}

		for (int i = 0; i < size1; i++) { //iterating through first set and comparing with second set to fill result
			matchfound = false; //need to reset the matchfound bool for a new comparison
			ItemType x;
			s1.get(i, x); // putting value from larger set to be compared in x
			for (int j = 0; j < size2; j++) {
				ItemType y;
				s2.get(j, y); //putting value from second set in y to compare to x
				if (y == x) {
					matchfound = true;
				}
			}
			if (!matchfound) { //if a match of  variable x in the first set isnt found in the second set then append it to result
				result.insert(x);
			}
		}

		for (int i = 0; i < size2; i++) { //iterating through second set and comparing with the now potentially semi-full result set
			matchfound = false; //need to reset the matchfound bool for a new comparison
			ItemType x;
			s2.get(i, x); // putting value from second set to be compared in x
			for (int j = 0; j < result.size(); j++) {
				ItemType y;
				result.get(j, y); //putting value from first set in y to compare to x
				if (y == x) {
					matchfound = true;
				}
			}
			if (!matchfound) { //if a match of  variable x isnt found in the second set then append it to result
				result.insert(x);
			}
		}
	}
}

void subtract(const Set & s1, const Set & s2, Set & result)
{

	Set temp(s1);  //put s1 -> result
	Set temp2(s2); //save s2 info in case of aliasing
	int size2 = s2.size();
	result = temp;
	for (int i = 0; i < size2; i++) { //compare s2 to result and if a match is found erase match from result
		int resultsize = result.size();

		ItemType x; //putting s2 items into x to compare with result and if there is a match delete it
		temp2.get(i, x);
		for (int j = 0; j < resultsize;j++) {
			ItemType y; //putting result items in y to compare with x
			result.get(j, y);
			if (y == x) {
				result.erase(y);
				resultsize--;
				j--;
			}
		}
	}
}
