#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;


void test() {
	

														   //swap
	Set s5;//empty
	Set s6; //empty

	s5.insert("fo");//insert values
	s5.insert("ro");
	s5.insert("xo");
	s5.insert("fg");

	assert(s5.size() == 4); //size is 4
	s6.insert("fg");//insert
	s6.insert("hp");
	s6.insert("ge5");

	assert(s6.size() == 3); //size is 3
	s5.swap(s6); //swap the 2 Sets
	assert(s6.size() == 4); //size swaped
	assert(s5.size() == 3); //swap the sizes

							//case if the one or both of the sets are empty

	Set s7; //empty set
	assert(s7.empty());
	//Set S6 has 4 values
	assert(s6.size() == 4); //size checked
	s6.swap(s7); //swap the 2 sets
	assert(s7.size() == 4); //the sizes swapped
	assert(s6.size() == 0); //the size


							//copy constructor
	Set s8;
	s8.insert("g32");//insert values
	s8.insert("xbox");
	s8.insert("hp");
	s8.insert("dw");
	s8.insert("rew");
	assert(s8.size() == 5); //the size of the s8 set
	Set s9 = s8; //call the copy constructor
	assert(s9.size() == 5); //size is copied
	assert(s9.contains("g32") && s9.contains("xbox") && s9.contains("hp") && s9.contains("dw") && s9.contains("rew")); //now the new set s9 contains all the values of s8

																													   //for an empty Set;
	Set s10;
	assert(s10.empty());
	Set s12 = s10;
	assert(s12.empty());






	//assignment operator
	Set s11; //empty
	Set s13; //empty

	s11.insert("val"); //insert values
	s11.insert("wow");
	s11.insert("hello");
	s11.insert("to");
	assert(s11.size() == 4); //size is 4
	assert(s13.size() == 0); //empty list
	s13 = s11; //call the assignment operator
	assert(s11.size() == 4); //size of the set is 4
	assert(s13.size() == 4); //the size changes
	assert(s13.contains("val") && s13.contains("wow") && s13.contains("hello") && s13.contains("to"));//the set now contains

																									  //the case if both of the objects are empty
	Set s14; //empty set
	Set s15; //empty set
	assert(s15.size() == 0); //check the size
	assert(s14.size() == 0);  //size
	s14 = s15; //call the assignement operator
	assert(s14.empty()); //check if the set is still empty
	assert(s14.size() == 0); //check the set size

							 //unite function
	Set s16; //empty
	Set s17; //empty
	s16.insert("h"); //insert distinct values into s16
	s16.insert("e");
	s16.insert("l");
	s16.insert("o");
	s16.insert("m");
	s16.insert("i");
	s16.insert("k");
	s16.insert("e");

	s17.insert("h"); //insert different values in s17
	s17.insert("f");
	s17.insert("s");
	s17.insert("o");
	s17.insert("i");
	s17.insert("g");
	s17.insert("34");
	s17.insert("s");
	s17.insert("t");
	s17.insert("O");

	Set s18; //some empty set
	unite(s16, s17, s18);
	assert(s18.size() == 13);//s18 contains non-duplicated values from s16 and s17
	assert(s18.contains("h") && s18.contains("e") && s18.contains("l") && s18.contains("o") && s18.contains("m") && s18.contains("i") && s18.contains("k") && s18.contains("e") && s18.contains("f") && s18.contains("s") && s18.contains("34") && s18.contains("s") && s18.contains("t") && s18.contains("O")); //s18 now has all values that are in s16, or s17 but not duplicated

																																																																												 //use unite with two empty sets
	Set s19; //empty set
	Set s20;// empty set
	Set s21;
	unite(s19, s20, s21); //unite 2 empty sets
	assert(s21.empty());  //set s21 from two empty sets must be an emoty set
	assert(s21.size() == 0);

	//unite w one empty set
	unite(s16, s19, s20); //unite
	assert(s20.size() == s16.size()); //s20 must now have the same size as s20
	assert(s20.contains("h") && s20.contains("e") && s20.contains("l") && s20.contains("o") && s20.contains("m") && s20.contains("i") && s20.contains("k")); //s20 now has to contain all the values from  s16




																																							 //also consider the cases of aliasing

																																							 //the case if unite (x,x,y); or when the first two arguments are the same
	unite(s16, s16, s19);
	assert(s19.contains("h") && s19.contains("e") && s19.contains("l") && s19.contains("o") && s19.contains("m") && s19.contains("i") && s19.contains("k")); //s19 must contain all values of s16

																																							 //the  case when unite (x,y,y);
	unite(s16, s17, s17); //unite called, this should modify s17 so that it has all the non-duplicated values from s16 and s17
	assert(s17.size() == 13); //size
	assert(s17.contains("h") && s17.contains("e") && s17.contains("l") && s17.contains("o") && s17.contains("m") && s17.contains("i") && s17.contains("k") && s17.contains("e") && s17.contains("f") && s17.contains("s") && s17.contains("34") && s17.contains("s") && s17.contains("t") && s17.contains("O")); //s17 must now contain all the values of s16 and s17
																																																																												 //the case unite (x,y,x)
	Set s22; //empty
	s22.insert("j"); //insert values
	s22.insert("");
	s22.insert("z");
	s22.insert("u");

	unite(s22, s17, s22); //call unite
	assert(s22.size() == 17);
	assert(s22.contains("h") && s22.contains("e") && s22.contains("l") && s22.contains("o") && s22.contains("m") && s22.contains("i") && s22.contains("k") && s22.contains("e") && s22.contains("f") && s22.contains("s") && s22.contains("34") && s22.contains("s") && s22.contains("t") && s22.contains("O") && s22.contains("j") && s22.contains("") && s22.contains("z") && s22.contains("u"));  //s22 must have the values of s17+s22 not-duplicated

																																																																																																	 //the case unite (x,x,x)

	unite(s17, s17, s17);
	assert(s17.size() == 13); //the size must be 15 since nothing has changed
	assert(s17.contains("h") && s17.contains("e") && s17.contains("l") && s17.contains("o") && s17.contains("m") && s17.contains("i") && s17.contains("k") && s17.contains("e") && s17.contains("f") && s17.contains("s") && s17.contains("34") && s17.contains("s") && s17.contains("t") && s17.contains("O")); //s17 must contain the same valuess



																																																																												 //subtract function
																																																																												 //check the regular case
	Set s23;
	Set s24;
	s23.insert("i"); //insert values
	s23.insert("k");
	s23.insert("s");
	s23.insert("w");
	s23.insert("a");

	s24.insert("s");
	s24.insert("d");
	s24.insert("h");
	s24.insert("t");
	s24.insert("a");
	s24.insert("");

	subtract(s23, s24, s17); //call unite
	assert(s17.size() == 3); //subtracted size
	assert(s17.contains("i") && s17.contains("k") && s17.contains("w")); //contains values in s23 but not in s24s

																		 //if (x,y,z), if x is empty
	Set s25;
	subtract(s25, s24, s17);
	assert(s17.size() == 0 && s17.empty());

	//case (x,x,y)

	subtract(s23, s23, s17); //subtract is called
	assert(s17.empty() && s17.size() == 0); //the size of the set is empty

											//case (x,y,x)
	subtract(s23, s24, s23); //subtact is called with the first and last args the same
	assert(s23.size() == 3); //the size of s23 must be 3 now, since its been modified

							 //case (x,y,y)

	Set s26;
	Set s27;
	s26.insert("ss");
	s26.insert("koko");
	s26.insert("d");
	s26.insert("large");
	s26.insert("woh");
	s26.insert("gg");
	s26.insert("jj");


	s27.insert("jj");
	s27.insert("woh");
	s27.insert("s");
	s27.insert("d");
	s27.insert("h");
	s27.insert("t");
	s27.insert("a");
	s27.insert("d");
	s27.insert("");

	subtract(s26, s27, s27);
	assert(s27.size() == 4);
	assert(s27.contains("ss") && s27.contains("koko") && s27.contains("large") && s27.contains("gg")); //now s27 is modified and contains the subtracted values.

																									   //case (x,x,x)
	subtract(s26, s26, s26); //subtract a set from itself and give it to itself
	assert(!s26.contains("ss") && !s26.contains("koko")); //s26 contains nothing
	assert(s26.size() == 0 && s26.empty()); //s26 is an empty set now


											//Test cases to throughly check the function.

											//testing the default constuctor
											//for a an empty Set
	ItemType testString;
	Set m1;
	Set s1;
	ItemType x;
	assert(!m1.contains("Y")); //does not contain anything
	assert(!m1.erase("hey")); //nothing to be removed
	assert(!m1.get(0, testString)); //there is no value to be given back
	assert(m1.empty()); //the set m1 is empty so it has to be true
	assert(m1.size() == 0); //the size of the empty list has to be zero

							//for non-empty Set

	assert(s1.insert("ABC"));        // insert "ABC"
	assert(s1.insert("BCD"));        // insert "BCD"
	assert(s1.insert("CDE"));        // insert "CDE"
	assert(s1.size() == 3);           // test size
	assert(!s1.empty());             // test empty

	assert(s1.contains("ABC"));      // contains "ABC"
	assert(!s1.empty()); //s1 has value
	assert(s1.contains("ABC")); //contains ABC
	assert(s1.contains("BCD")); //contains BCD
	assert(s1.contains("CDE"));//contains CDE
	assert(s1.size() == 3); //size of the set
	assert(s1.get(0, testString)); //get ABC
	assert(testString == "ABC"); //ABC is the smallest
	assert(s1.get(1, testString)); // second value
	assert(testString == "BCD"); //BCD is the second
	assert(s1.get(2, testString));
	assert(testString == "CDE"); //CDE is the largest
	assert(!s1.get(3, testString)); //out of bounds
	assert(s1.erase("ABC")); //erase ABC
	assert(!s1.erase("ABC")); //Already erased
	assert(s1.size() == 2);  //size of the set
	assert(!s1.get(2, testString)); //out of bounds now
	assert(!s1.contains("ABC") && s1.contains("BCD") && s1.contains("CDE"));



	//    assert(s1.get(0, x) && x=="joey");// "joey" is smallest
	//    assert(s1.erase("roti"));         // remove "roti"
	//    assert(s1.size() == 2);           // test size
	//    assert(!s1.contains("roti"));     //doesn’t contain "roti"
	//    assert(s1.contains("pati"));      // contains "pati"
	//    assert(s1.contains("joey"));      // contains "joey"
	//    assert(s1.get(0, x) && x=="joey");// "joey" is smallest
	//    assert(s1.get(1, x) && x=="pati");// "pati" is largest
	//    assert(!s1.get(2, x));            // 2 is out of bounds


	//default constructor was tested to check its functionality, it is supposed to create an empty list

	//insert function

	assert(m1.insert("mi")); //it has to take the value that we insert to it (and not a duplicate)
	assert(m1.contains("mi")); //the set does contain the value
	assert(m1.size() == 1); //the size must be one has one value
	assert(m1.get(0, testString)); //we should be able to get the value back
	assert(testString == "mi"); //correct value was given back
	assert(!m1.erase("zp")); //the value was not inserted in the set
	assert(m1.erase("mi")); //now we can remove the item since we have an item
	assert(m1.empty()); //the set is now empty
						//multiple inserts
	assert(m1.insert("yo")); //values are inserted in the Set and in the linked list
	assert(m1.insert("mo"));
	assert(m1.insert("to"));
	assert(m1.insert("lo"));
	assert(m1.insert("so"));
	assert(m1.insert("ko"));

	assert(!m1.insert("mo")); //if a value already exists in the set we cannot insert it again
	assert(!m1.insert("lo"));

	assert(m1.size() == 6); //the size of set is 6 since we inserted 6 items
	assert(!m1.contains("qo")); //the value was not inserted
	assert(m1.contains("yo") && m1.contains("mo") && m1.contains("to") && m1.contains("lo") && m1.contains("so") && m1.contains("ko"));;// the set must contain all of these values since they were all inserted
	assert(!m1.empty()); //the list is not empty
	assert(m1.erase("mo")); //erase an item from the set
	for (int i = 0; i < m1.size(); i++)
	{
		assert(m1.get(i, testString)); //we must be able to get all the values from the set back
	}

	//empty function and size
	Set s3;
	assert(s3.empty()); //the set is empty
	assert(s3.size() == 0); //the size of an empty set is 0
	assert(s3.insert("go"));
	assert(!s3.empty()); //has value now
	assert(s3.size() == 1); //has one value
	assert(s3.insert("uo"));
	assert(s3.size() == 2); //has 2 values
	assert(!s3.empty()); // has values now
	assert(s3.erase("go")); //erase value
	assert(s3.size() == 1); //the size is 1 and contains 1 value
	assert(s3.erase("uo")); //erase
	assert(s3.size() == 0); //the set is empty so the size is 0
	assert(s3.empty()); //the set is empty now

	Set s4;
	//erase function and contains
	assert(!s4.get(0, testString)); //there is no value to get

	s4.insert("ABC");  //insert ABC
	s4.insert("BCD");//insert BCD
	s4.insert("CDE");//insert CDE
	s4.insert("DEF");//insert DEF
	s4.insert("EFG");//insert EFG
	s4.insert("FGH");//insert FGH

	assert(s4.get(0, testString) && testString == "ABC"); //get the first element, ABC is the smallest
	assert(s4.get(1, testString) && testString == "BCD"); //get the second element, BCD
	assert(s4.get(2, testString) && testString == "CDE");  //get the third element, CDE
	assert(s4.get(3, testString) && testString == "DEF"); //get the fourth element, DEF
	assert(s4.get(4, testString) && testString == "EFG"); //get EFG fifth element
	assert(s4.get(5, testString) && testString == "FGH");  //get 6th element, FGH
	assert(!s4.erase("yo")); //does not contain any value
	assert(!s4.contains("do"));  //this value is not contained
	assert(s4.insert("do")); //insert do
	assert(s4.contains("do")); //contains the value do
	assert(s4.insert("234"));   //insert 234
	assert(s4.contains("234"));   //contains
	assert(s4.insert("ABC"));  //insert ABC
	assert(s4.contains("ABC")); //contains ABC
	assert(s4.insert("BCD"));   //insert BCD
	assert(s4.contains("BCD")); //contains BCD
	assert(s4.insert("CDE"));   //insert CDE
	assert(s4.contains("CDE"));
	assert(s4.size() == 5); //size is 5
	assert(s4.erase("do"));  //erase do
	assert(s4.erase("234"));    //erase 234
	assert(s4.erase("ABC"));    //ABC
	assert(s4.erase("BCD"));    //BCD
	assert(s4.erase("CDE"));    //CDE
	assert(s4.empty());  // the set is empty now, we deleted all the values.

						 //get function



}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}

