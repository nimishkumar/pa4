#include "StringListType.h"

StringListType::StringListType(const string& name):Type(TYPE_STRING_LIST)
{
	this->name = name;
}

StringListType* StringListType::make(const string& name)
{
	StringListType* t = new StringListType(name);
	t = static_cast<StringListType*>(get_type(t));
	return t;
}

bool StringListType::operator<(const Type& other)
{
	Type& o = (Type&) other;
	if(o.get_kind() != get_kind()) return get_kind() < o.get_kind();
	StringListType& ct = static_cast<StringListType&>(o);
	return name < ct.name;
}

string StringListType::to_string()
{
	return "StringListType(" + name + ")";
}
