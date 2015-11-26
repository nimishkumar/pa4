#ifndef TYPETABLE_H_
#define TYPETABLE_H_

class Type;
class AstIdentifier;
#include <vector>
#include <map>
#include <iostream>
#include <assert.h>

using namespace std;

class TypeTable{
private:
	vector<map<AstIdentifier*,Type*> > values;
public:
	TypeTable();
	void push();
	void pop();
	void add(AstIdentifier* id,Type* t);
	Type* find(AstIdentifier *id);
	void print_contents();
};

#endif
