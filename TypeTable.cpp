#include "TypeTable.h"
#include "ast/type.h"

TypeTable::TypeTable(){

}

void TypeTable::push(){
	map<AstIdentifier*,Type*> cur_map;
	values.push_back(cur_map);
}

Type* TypeTable::find(AstIdentifier *id){
	for(vector<map<AstIdentifier*, Expression*> >::reverse_iterator it = values.rbegin(); it != values.rend(); it++)
	{
		ap<AstIdentifier*, Type*> & cur = *it;
		if(cur.count(id) > 0) return cur[id];
	}
	return NULL;
}

void TypeTable::pop()
{
	if(values.size() == 0)
	{
		cout << "You can only pop from a symbol table that you have pushed on first" << endl;
		assert(false);
	}
	values.pop_back();
}

void TypeTable::add(AstIdentifier* id, Type* e)
{
	if(values.size() == 0)
	{
		cout << "You can only add to a symbol table that you have pushed on first" << endl;
		assert(false);
	}
	values[values.size()-1][id] = e;
}

void TypeTable::print_contents(){
	cout << "TO BE IMPLEMENTED LATER" << endl;
}
