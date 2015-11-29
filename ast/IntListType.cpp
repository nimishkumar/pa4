#include "IntListType.h"
  
IntListType::IntListType(const string& name):Type(TYPE_INT_LIST)
{
	this->name = name;
} 
 
IntListType* IntListType::make(const string& name)
{
	IntListType* t = new IntListType(name);
	t = static_cast<IntListType*>(get_type(t));
	return t;
} 

bool IntListType::operator<(const Type& other)
{
	Type& o = (Type&) other;
	if(o.get_kind() != get_kind()) return get_kind() < o.get_kind();
	IntListType& ct = static_cast<IntListType&>(o);
	return name < ct.name;
} 

string IntListType::to_string()
{
	return "IntListType(" + name + ")";
} 
