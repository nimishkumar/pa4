#ifndef STRINGLISTTYPE_H_
#define STRINGLISTTYPE_H_

#include "Type.h"

#include <set>
#include <string>

using namespace std;
 
class StringListType: public Type
{
private:
	StringListType(const string& name);
	string name;
public:
	static StringListType* make(const string& name);
	virtual bool operator<(const Type& other);
	virtual string to_string();
};

#endif
