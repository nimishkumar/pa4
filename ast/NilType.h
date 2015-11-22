#ifndef NILTYPE_H_
#define NILTYPE_H_

#include "Type.h"
#include <set>
#include <string>

using namespace std;

class NilType : public Type{
private:
	NilType(const string& name);
	string name;
public:
	static NilType* make(const string& name);
	virtual bool operator < (const Type& other);
	virtual string to_string();	
};

#endif
