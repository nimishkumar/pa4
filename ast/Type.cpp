#include "Type.h"
#include "FunctionType.h"
#include <iostream>
using namespace std;

set<Type*, TypeComparator> Type::types;

void Type::print_all_types()
{
	cout << "************* All current types ********************" << endl;
	for(auto it = types.begin(); it != types.end(); it++)
	{
		cout << (*it)->to_string() << " Representative: " <<  (*it)->find()->to_string() << endl;
	}
	cout << "****************************************************" << endl;
}


Type* Type::get_type(Type* t)
{
    if(types.count(t) > 0)
    {
	Type* res = (Type*)*types.find(t);
	delete t;
	return res;
    }
    types.insert(t);
    return t;
}

Type::Type(type_kind tk)
{
  this->tk = tk;
  parent = this;
}

type_kind Type::get_kind()
{
  return tk;
}


void Type::set_parent(Type* t)
{
  parent = t;
}
     
Type* Type::find()
{
	// Every type will have a parent
	// The parent whose parent is itself is the equivalence class representative
  Type* old = parent;
  Type* t = old->parent;
  while(old != t)
  {
      old = t;
      t = t->parent;
  }
  return t;
}

void Type::compute_union(Type* other)
{
	// Find the representatives (again,fix this)
  Type* t1 = this->find();
  Type* t2 = other->find();
	// if constant set the second as child of first
  if(t1->tk == TYPE_CONSTANT || t1->tk == TYPE_USERDEFINED || t1->tk == TYPE_NIL || t1->tk == TYPE_INT_LIST || t1->tk == TYPE_STRING_LIST)
  {
      t2->set_parent(t1);
      return;
  }
	// if other constant set the first as child of the second
  if(t2->tk == TYPE_CONSTANT || t2->tk == TYPE_USERDEFINED || t2->tk == TYPE_NIL || t2->tk == TYPE_INT_LIST || t2->tk == TYPE_STRING_LIST)
  {
      t1->set_parent(t2);
      return;
  }
	// same for functions
  if(t1->tk == TYPE_FUNCTION)
  {
      t2->set_parent(t1);
      return;
  }
  if(t2->tk == TYPE_FUNCTION)
  {
      t1->set_parent(t2);
      return;
  }
	// for variables always set the second as child of the first
  t2->set_parent(t1);
  
}


bool Type::unify(Type* other)
{
	// finds the representatives of the EQ class that these types are a part of
  Type* t1 = this->find();
  Type* t2 = other->find();
	// if the representatives are the same then the union is nothing
  if(t1 == t2) return true;
  // if they are both functions
  if(t1->tk == TYPE_FUNCTION && t2->tk == TYPE_FUNCTION)
  {
		// compute their union
  	t1->compute_union(t2);
		FunctionType* f1 = static_cast<FunctionType*>(t1);
		FunctionType* f2 = static_cast<FunctionType*>(t2);
		// if they are different function names the union is wrong
		if(f1->get_name() != f2->get_name()) return false;
		const vector<Type*> & arg1 = f1->get_args();
		const vector<Type*> & arg2 = f2->get_args();
		// if the ares are not the same then the union is wrong
		if(arg1.size() != arg2.size()) return false;
		for(unsigned int i = 0; i < arg1.size(); i++)
		{
			if(arg1[i]->unify(arg2[i]) == false) return false;
		}
		return true;
  }
	// if one of them is a variable the union always works ?? wat
  if(t1->tk == TYPE_VARIABLE || t2->tk == TYPE_VARIABLE) {
      t1->compute_union(t2);
      return true;
  }
  
	// return if the union doesnt work if there is a type mismatch
  return false;
}
