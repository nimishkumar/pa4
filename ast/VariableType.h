 #ifndef VARIABLE_TYPE_H_
 #define VARIABLE_TYPE_H_
 
#include "Type.h"
//#include "Expression.h"
#include "AstBinOp.h"
#include "AstUnOp.h"

#include <set>
#include <string>

using namespace std;

enum Constr {NI,IN,STR,INL,STRL,FU};

class VariableType: public Type
{
private:
  VariableType(const string& name);
  string name;
	bool constraints[6];
	// 0 - NIL
	// 1 - INT
	// 2 - STRING
	// 3 - INTLIST
	// 4 - STRINGLIST
	// 5 - FUNCTION
	void zeroify();
public:
  static VariableType* make(const string& name);
  virtual bool operator<(const Type& other);
  virtual string to_string();
	void makeConstraintsForBinOp(binop_type bt);
	void makeConstraintsForUnOp(unop_type ut);
	void checkConstraints();
	bool doTypesMatch(type_kind tk);
};
 
 
#endif /* VARIABLE_TYPE_H_ */
