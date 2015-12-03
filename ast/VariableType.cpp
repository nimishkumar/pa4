#include "VariableType.h"


VariableType::VariableType(const string& name):Type(TYPE_VARIABLE)
{
  this->name = name;
	constraints[0] = true;
	constraints[1] = true;
	constraints[2] = true;
	constraints[3] = true;
	constraints[4] = true;
	constraints[5] = true;
}
   

VariableType* VariableType::make(const string& name)
{
  VariableType* t = new VariableType(name);
  t = static_cast<VariableType*>(get_type(t));
  return t;
  
}

bool VariableType::operator<(const Type& other)
{
  Type& o = (Type&) other;
  if(o.get_kind() != get_kind()) return get_kind() < o.get_kind();
  VariableType& ct = static_cast<VariableType&>(o);
  return name < ct.name;
}


string VariableType::to_string()
{
	/*
	string stuff = "\n";
	if(constraints[NI])
		stuff += "NIL\n";
	if(constraints[IN])
		stuff += "INT\n";
	if(constraints[STR])
		stuff += "STR\n";
	if(constraints[INL])
		stuff += "INTLIST\n";
	if(constraints[STRL])
		stuff += "STRLIST\n";
	if(constraints[FU])
		stuff += "FUNCTION\n";
		*/
  return "VariableType(" + name + ")"; 
}

void VariableType::makeConstraintsForBinOp(binop_type bt){
	switch(bt){
		case PLUS: {
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case MINUS: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case TIMES: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case DIVIDE: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case AND: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case OR: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case EQ: {
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case NEQ: {
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case LT: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case LEQ: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case GT: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case GEQ: {
			constraints[STR] = false;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
			break;
		}
		case CONS: {
			constraints[STR] = true;
			constraints[NI] = false;
			constraints[INL] = false;
			constraints[STRL] = false;
			constraints[FU] = false;
		}
		case CONS2: {
			constraints[NI] = false;
			constraints[FU] = false;
		}
		default:
			cout << "ERROR : BINOP TYPE NOT KNOWN" << endl;
			exit(1);
	}
	checkConstraints();
}

// DONT ACTUALLY NEED
void VariableType::zeroify(){
	constraints[0] = false;
	constraints[1] = false;
	constraints[2] = false;
	constraints[3] = false;
	constraints[4] = false;
	constraints[5] = false;
}

// DONT NEED
void VariableType::makeConstraintsForUnOp(unop_type ut){
	switch(ut){
		case HD: break;
		case TL: break;
		case ISNIL: break;
		case PRINT: break;
		default:
			cout << "ERROR : UNOP TYPE NOT KNOWN" << endl;
			exit(1);
	}
}

void VariableType::checkConstraints(){
	int cnt=0;
	for(int i=0;i<6;i++)
		if(constraints[i])
			return;
	cout << "ERROR :: VARIABLE TYPE HAS NO POSSIBLE VALUE" << endl;
	exit(1);
}

bool VariableType::doTypesMatch(type_kind tk,int constant){
	switch(tk){
		case TYPE_USERDEFINED: {
			// Not sure yet how this shall work
			break;
		}
		case TYPE_NIL: {
			return constraints[NI];
		}
		case TYPE_CONSTANT: {
			if(constant)
				return constraints[IN];
			return constraints[STR];
		}
		case TYPE_INT_LIST: {
			return constraints[INL];
		}
		case TYPE_STRING_LIST: {
			return constraints[STRL];
		}
		case TYPE_FUNCTION: {
			return constraints[FU];
		}
		case TYPE_VARIABLE: {
			// not sure exactly how this one should be made
			break;
		}
	}
	return false;
}
