#include "ConstantType.h"


ConstantType::ConstantType(const string& name):Type(TYPE_CONSTANT)
{
  this->name = name;
}
   

ConstantType* ConstantType::make(const string& name,constant_kind typ)
{
  ConstantType* t = new ConstantType(name);
  t = static_cast<ConstantType*>(get_type(t));
	t->constantType = typ;
  return t;
  
}

bool ConstantType::operator<(const Type& other)
{
  Type& o = (Type&) other;
  if(o.get_kind() != get_kind()) return get_kind() < o.get_kind();
  ConstantType& ct = static_cast<ConstantType&>(o);
  return name < ct.name;
}

constant_kind ConstantType::get_constant_type(){
	return constantType;
}

string ConstantType::to_string()
{
  return "ConstantType(" + name + ")";
}
