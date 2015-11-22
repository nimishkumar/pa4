#include "UserDefinedType.h"

UserDefinedType::UserDefinedType(const string& name):Type(TYPE_USERDEFINED)
{
	this->name = name;
}

UserDefinedType* UserDefinedType::make(const string& name,constant_kind typ)
{
	UserDefinedType* t = new UserDefinedType(name);
	t = static_cast<UserDefinedType*>(get_type(t));
	t->userType = typ;
	return t;
}

bool UserDefinedType::operator<(const Type& other)
{
	Type& o = (Type&) other;
	if(o.get_kind() != get_kind()) return get_kind() < o.get_kind();
	UserDefinedType& ct = static_cast<UserDefinedType&>(o);
	return name < ct.name;
}

constant_kind UserDefinedType::get_constant_type(){
	return userType;
}

string UserDefinedType::to_string()
{
	return "UserDefinedType(" + name + ")";
}

