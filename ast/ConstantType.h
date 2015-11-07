 #ifndef CONSTANT_TYPE_H_
 #define CONSTANT_TYPE_H_
 
#include "Type.h"
 
 
#include <set>
#include <string>

using namespace std;
enum constant_kind {STRING_CONSTANT,INT_CONSTANT}

 class ConstantType: public Type
 {
private:
	ConstantType(const string& name);
	string name;
	constant_kind constantType;
   
 public:
    static ConstantType* make(const string& name,constant_kind typ);
    virtual bool operator<(const Type& other);
    virtual string to_string();
    constant_kind get_constant_type();
    
 };
 
 
 #endif /* CONSTANT_TYPE_H_ */
