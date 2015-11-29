#ifndef INTLISTTYPE_H_
#define INTLISTTYPE_H_

#include "Type.h"

#include <set>
#include <string>

using namespace std;
 
class IntListType: public Type
{
private:
	IntListType(const string& name);
	string name;
public:
	static IntListType* make(const string& name);
	virtual bool operator<(const Type& other);
	virtual string to_string();
};

#endif
