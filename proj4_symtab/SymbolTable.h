#ifndef SYMBOLTABLE_INCLUDED
#define SYMBOLTABLE_INCLUDED

#include <string>

class SymbolTableImpl;

class SymbolTable
{
  public:
    SymbolTable();
    ~SymbolTable();
    void enterScope();
    bool exitScope();
    bool declare(const std::string& id, int lineNum);
    int find(const std::string& id) const;
      // We prevent a SymbolTable object from being copied or assigned
    SymbolTable(const SymbolTable&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;

  private:
    SymbolTableImpl* m_impl;
};

#endif // SYMBOLTABLE_INCLUDED


//
//#include "SymbolTable.h"
//#include <string>
//#include <vector>
//#include <functional>
//#include <list>
//#include <array>
//using namespace std;
//
//const int NUM_BUCKETS = 17579;
//// This class does the real work of the implementation.
//
//class SymbolTableImpl
//{
//public:
//	SymbolTableImpl();
//	void enterScope();
//	bool exitScope();
//	bool declare(const string& id, int lineNum);
//	int find(const string& id) const;
//private:
//	struct symbol {
//		int m_line;
//		string m_id;
//		int m_scope;
//	};
//	int current_scope;
//	vector<list<symbol>::iterator> m_ids;
//	//  vector<int> m_lines;
//	unsigned int hashFunction(const string& hashMe) const;
//	//	std::array<std::list<symbol>, NUM_BUCKETS> m_buckets; //an array of lists of NUM_BUCKETS symbols
//	list<symbol> m_buckets[NUM_BUCKETS];
//};
//
//SymbolTableImpl::SymbolTableImpl()
//	:current_scope(0)
//{
//	//default construct everything else
//}
//
//void SymbolTableImpl::enterScope()
//{
//	// Extend the id vector with an empty string that
//	// serves as a scope entry marker.
//	current_scope++;
//	//  m_ids.push_back("");
//	// m_lines.push_back(0);
//}
//
//bool SymbolTableImpl::exitScope()
//{
//	if (current_scope - 1 < 0) {
//		return false;
//	}
//	// Remove ids back to the last scope entry.
//	list<symbol>::iterator to_delete;
//
//	while (!m_ids.empty() && (*m_ids.back()).m_scope == current_scope)
//	{
//		to_delete = m_ids.back();
//		unsigned int hash_to_delete = hashFunction(to_delete->m_id);
//		to_delete = m_buckets[hash_to_delete].erase(to_delete);
//
//
//		/*
//		unsigned int hashed_to_delete = hashFunction(to_delete);
//		list<symbol>::iterator p = m_buckets[hashed_to_delete].begin();
//		for (; p != m_buckets[hashed_to_delete].end();) {
//		if ((*p).m_id == to_delete && (*p).m_scope == current_scope) {
//		p = m_buckets[hashed_to_delete].erase(p);
//		break;
//		//break cuz there will only be one?
//		}
//		else {
//		p++;
//		}
//
//		}
//		*/
//		m_ids.pop_back();
//		//  m_lines.pop_back();
//	}
//
//	/*
//	if (m_ids.empty())
//	return false;
//	*/
//	// Remove the scope entry marker itself.
//
//	current_scope--;
//	//    m_ids.pop_back();
//	//    m_lines.pop_back();
//	return true;
//}
//
//bool SymbolTableImpl::declare(const string& id, int lineNum)
//{
//	if (id.empty())
//		return false;
//	if (lineNum < 0) {
//		return false;
//	}
//
//	// Check for another declaration in the same scope.
//	// Return if found, break out if encounter the scope
//	// entry marker.
//
//	string check_id = id;
//	unsigned int hashed_check_id = hashFunction(check_id);
//
//	list<symbol>::iterator p = m_buckets[hashed_check_id].begin();
//	for (; p != m_buckets[hashed_check_id].end(); p++) {
//		if ((*p).m_id == check_id && (*p).m_scope == current_scope) {
//			return false;
//		}
//	}
//
//	/*
//	size_t k = m_ids.size();
//	while (k > 0)
//	{
//	k--;
//	if (m_ids[k].empty())
//	break;
//	if (m_ids[k] == id)
//	return false;
//	}
//	*/
//
//	// Save the declaration
//
//	//create a symbol to add with all the info 
//	symbol to_add;
//	to_add.m_id = id;
//	to_add.m_line = lineNum;
//	to_add.m_scope = current_scope;
//
//	//put it into ze hash table!
//	m_buckets[hashed_check_id].push_front(to_add);
//
//	m_ids.push_back(m_buckets[hashed_check_id].begin());
//	// m_lines.push_back(lineNum);
//	return true;
//}
//
//int SymbolTableImpl::find(const string& id) const
//{
//	if (id.empty())
//		return -1;
//
//	// Search back for the most recent declaration still
//	// available.
//	unsigned int hashed_id = hashFunction(id);
//	list<symbol>::const_iterator p = m_buckets[hashed_id].begin();
//	/*symbol s;
//	s.m_id = id;
//	s.m_line = -1;
//	s.m_scope = -1;*/
//	for (; p != m_buckets[hashed_id].end(); p++) {
//		if ((*p).m_id == id) {
//
//			/*
//			if ((*p).m_scope > s.m_scope) {
//			s.m_line = (*p).m_line;
//			}
//			*/
//			return p->m_line;
//		}
//	}
//	/*
//	if (s.m_line != -1) {
//	return s.m_line;
//	}
//	*/
//	/*
//	size_t k = m_ids.size();
//	while (k > 0)
//	{
//	k--;
//	if (m_ids[k] == id)
//	return m_lines[k];
//	}
//
//	*/
//	return -1;
//}
//
//unsigned int SymbolTableImpl::hashFunction(const string & hashMe) const
//{
//	std::hash<std::string> str_hash;
//	unsigned int hashValue = str_hash(hashMe);
//
//	unsigned int bucketNum = hashValue % NUM_BUCKETS;
//	return bucketNum;
//}
