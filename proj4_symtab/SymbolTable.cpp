// SymbolTable.cpp

// This is a correct but inefficient implementation of
// the SymbolTable functionality.

#include "SymbolTable.h"
#include <string>
#include <vector>
#include <functional>
#include <forward_list>
#include <array>
using namespace std;

const int NUM_BUCKETS = 3779; // prime number makes more even distrobutions
  // This class does the real work of the implementation.

class SymbolTableImpl
{
  public:
	SymbolTableImpl();
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
  private:
	  struct symbol {
		  int m_line;
		  string m_id;
		  int m_scope;
	  };
	
	int current_scope;
    vector<forward_list<symbol>::iterator> m_ids;
	unsigned int hashFunction(const string& hashMe) const;
	forward_list<symbol> m_buckets[NUM_BUCKETS];
};

SymbolTableImpl::SymbolTableImpl()
{
	current_scope = 0;//default construct everything else
}

void SymbolTableImpl::enterScope()
{
    //increment current scope
	current_scope++;
 
}

bool SymbolTableImpl::exitScope()
{
	//if exiting scope means going before starting scope there is an error
	if ((current_scope -1) < 0) {
		return false;
	}
	if (current_scope == 0) {
		return false;
	}
      // Remove ids back to the last scope entry.
	forward_list<symbol>::iterator to_delete;
	
    while (!m_ids.empty() &&  (*m_ids.back()).m_scope == current_scope)
    {
		to_delete = m_ids.back(); //symbol to delete
		unsigned int hash_to_delete = hashFunction(to_delete->m_id); //hash id for symbol to delete 
		m_buckets[hash_to_delete].pop_front();

        m_ids.pop_back();
    
    }
	
	//decrement the scope
	current_scope--;
    return true;
}

bool SymbolTableImpl::declare(const string& id, int lineNum)
{
	//error check input
    if (id.empty())
        return false;
	if (lineNum < 0) {
		return false;
	}

      // Check the bucket id hashes to for another declaration in the same scope.
      // return false if found

	string check_id = id;
	unsigned int hashed_check_id = hashFunction(check_id);

	forward_list<symbol>::iterator p = m_buckets[hashed_check_id].begin();
	for (; p != m_buckets[hashed_check_id].end();p++) {
		if ((*p).m_id == check_id && (*p).m_scope == current_scope) {
			return false;
		}
	}

      // Save the declaration

	//create a symbol to add with all the info 
	symbol to_add;
	to_add.m_id = id;
	to_add.m_line = lineNum;
	to_add.m_scope = current_scope;
	
	//add the symbol to the hash table in the correct bucket
	m_buckets[hashed_check_id].push_front(to_add);
	//record an iterator for the id for easy deletion once exiting scope
    m_ids.push_back(m_buckets[hashed_check_id].begin());
    return true;
}

int SymbolTableImpl::find(const string& id) const
{
	//error check input
    if (id.empty())
        return -1;

	// Search the bucket id would be in for the most recent declaration still
    // available.
	unsigned int hashed_id = hashFunction(id);
	forward_list<symbol>::const_iterator p = m_buckets[hashed_id].begin();
	
	for (; p != m_buckets[hashed_id].end(); p++) {
		if ((*p).m_id == id) {
			return p->m_line;
		}

	}
	//if it wasnt found in the bucket it would be in, its not in the hash table 
	//so return -1
    return -1;
}

unsigned int SymbolTableImpl::hashFunction(const string & hashMe) const
{
	std::hash<std::string> str_hash;
	unsigned int hashValue = str_hash(hashMe);

	unsigned int bucketNum = hashValue % NUM_BUCKETS;
	return bucketNum;
}

//*********** SymbolTable functions **************

// For the most part, these functions simply delegate to SymbolTableImpl's
// functions.

SymbolTable::SymbolTable()
{
    m_impl = new SymbolTableImpl;
}

SymbolTable::~SymbolTable()
{
    delete m_impl;
}

void SymbolTable::enterScope()
{
    m_impl->enterScope();
}

bool SymbolTable::exitScope()
{
    return m_impl->exitScope();
}

bool SymbolTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int SymbolTable::find(const string& id) const
{
    return m_impl->find(id);
}



