#ifndef USERDEFINEDTYPE_H_
#define USERDEFINEDTYPE_H_

#include "Type.h"
#include "ConstantType.h"
#include <set>
#include <string>

using namespace std;

class UserDefinedType : public Type{
private:
	UserDefinedType(const string& name);
	string name;
	constant_kind userType;
public:
	static UserDefinedType* make(const string& name,constant_kind typ);
	virtual bool operator<(const Type& other);
	virtual string to_string();
	constant_kind get_constant_type();
};

#endif
