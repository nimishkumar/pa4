#include "NilType.h"

NilType::NilType(const string& name):Type(TYPE_NIL)
{
	this->name = name;
}

NilType* NilType::make(const string& name)
{
	NilType* t = new NilType(name);
	t = static_cast<NilType*>(get_type(t));
	return t;
}

bool NilType::operator<(const Type& other)
{
	Type& o = (Type&) other;
	if(o.get_kind() != get_kind()) return get_kind() < o.get_kind();
	NilType& ct = static_cast<NilType&>(o);
	return name < ct.name;
}

string NilType::to_string()
{
	return "NilType(" + name + ")";
}
