#include "NilType.h"
#include "ConstantType.h"
#include "VariableType.h"
#include "FunctionType.h"
#include "ast/expression.h"
#include "TypeInference.h"
#include "IntListType.h"
#include "StringListType.h"

TypeInference::TypeInference(Expression * e)
{
	// Create the initial Type Enviornment
	type_tab.push();
	this->program = e;
	// Do Type Inference
	Type* t = infer(e);
	// Print the final Return Type
	cout << "res = " << t->to_string() << endl;
	Type::print_all_types();
/*
	Type* t1 = ConstantType::make("Int");
	Type* t2 = ConstantType::make("String");
	Type* t3 = ConstantType::make("Int");
	Type* var1 = VariableType::make("x");
	cout << t1->to_string() << " " << t1 << endl;
	cout << t2->to_string() << " " << t2 << endl;
	cout << t3->to_string() << " " << t3 << endl;
	vector<Type*> v1;
	v1.push_back(var1);
	v1.push_back(t2);

	vector<Type*> v2;
	v2.push_back(t3);
	v2.push_back(t2);

	Type* t4 = FunctionType::make("fun", v1);
	Type* t5 = FunctionType::make("fun", v2);
	cout << t4->to_string() << " " << t4 << endl;
	cout << t5->to_string() << " " << t5 << endl;
	
	//prints all types & reps in table
	Type::print_all_types();

	{
		Type* t1 = t4;
		Type* t2 = t5;

		cout << "Type 1:" << t1->to_string() << "Rep: " << t1->find()->to_string() << endl;
		cout <<	"Type 2:" << t2->to_string() << "Rep: " << t2->find()->to_string() << endl;

		cout << "unify: " << t1->unify(t2) << endl;

		cout << "Type 1:" << t1->to_string() << "Rep: " << t1->find()->to_string() << endl;
		cout <<	"Type 2:" << t2->to_string() << "Rep: " << t2->find()->to_string() << endl;
	} cout << "Type 1:" << t1->to_string() << "Rep: " << t1->find()->to_string() << endl; cout <<	"Type 2:" << var1->to_string() << "Rep: " << var1->find()->to_string() << endl;

	
	Type::print_all_types();
*/
}

void TypeInference::reportError(string err){
	// Used to report Type Inference errors
	cout << "Type Inference Error :: " + err << endl;
	exit(1);
}

Type* TypeInference::infer(Expression *e){
	Type* res = NULL;

	// A switch for all expressions
	switch(e->get_type()) {
		case AST_BINOP:				//case 0
		{
			AstBinOp* b = static_cast<AstBinOp*>(e);
			res = infer_binop(b);
			break;
		}
		case AST_IDENTIFIER:		//case 1
		{
			AstIdentifier* id = static_cast<AstIdentifier*>(e);
			Type* type = type_tab.find(id);
			if(type==NULL) {
				// If the identifier is not apart of the current type enviornment report an error
				reportError("Identifier "+id->get_id()+" is not bound in current context");
			}
			res = type;
			break;
		}
		case AST_INT:				//case 2
		{
			res = ConstantType::make("int", INT_CONSTANT);
			break;
		}
		case AST_LAMBDA:			//case 3
		{
			AstLambda* lambda = static_cast<AstLambda*>(e);
			AstIdentifier* id = lambda->get_formal();
			vector<Type*> args;
			// Set the parameter for the Lambda as a Variable Type
			args.push_back(VariableType::make("param"));
			type_tab.push();
			// Set the type of the parameter to variable type in the Type Enviornment
			type_tab.add(id, VariableType::make(id->get_id()));
			// Infer on the body of the Lambda
			Type* body_t = infer(lambda->get_body());
			// remove the temporary parameter variable from the Type Enviornment
			type_tab.pop();
			args.push_back(body_t);
			// Return a function type
			res = FunctionType::make("lambda",args);
			break;
		}
		case AST_LET:				//case 4
		{
			AstLet* let = static_cast<AstLet*>(e);
			AstIdentifier* id = static_cast<AstIdentifier*>(let->get_id());
			// Get the type of the value in the let statement
			Type* type = infer(let->get_val());
			type_tab.push();
			// Add the type of the variable to the Type Enviornment
			type_tab.add(id, type);
			// Do type inference on the body
			res = infer(let->get_body());
			// Remove changes from the Type Enviornment
			type_tab.pop();
			break;
		}
		case AST_STRING:
		{
			res = ConstantType::make("string", STRING_CONSTANT);
			break;
		}
		case AST_IDENTIFIER_LIST:
		{
			break;
		}
		case AST_EXPRESSION_LIST:
		{
			/*
			AstExpressionList* list = static_cast<AstExpressionList*>(e);
			vector<Expression*> exps = list->get_expressions();
			if(infer(exps[0])->get_kind() != TYPE_FUNCTION)
				reportError("Only lambda expressions can be applied to other expressions");
			Expression* exp_lambda = exps[0];
			int counter = 0;
			while(exp_lambda->get_type()==AST_LAMBDA) {
				counter++;
				exp_lambda = static_cast<AstLambda*>(exp_lambda)->get_body();
			}
			for(int i=0; i<exps.size()-1; i++) {
				if(i==0)
					lambda = static_cast<AstLambda*>(exps[0]);
				else {
					if(infer(lambda->get_body())->get_kind() == TYPE_FUNCTION) {
						counter++;
						lambda = static_cast<AstLambda*>(lambda->get_body());
					}
					else
						reportError("Only lambda expressions can be applied to other expressions");
				}
				AstIdentifier* id = lambda->get_formal();
				type_tab.push();
				type_tab.add(id, infer(exps[exps.size()-1-i])->get_kind());
			}
			if(counter<exps.size()-1) {
				//TODO
			}
			else if(counter>exps.size()-1) {
				reportError("Only lambda expressions can be applied to other expressions");
			}
			*/
			break;
		}
		case AST_BRANCH:
		{
			AstBranch* branch = static_cast<AstBranch*>(e);
			Expression* e1 = branch->get_pred();
			Expression* e2 = branch->get_then_exp();
			Expression* e3 = branch->get_else_exp();
			// Do Type Inference on the Predicate
			Type* t1 = infer(e1);
			// Verify that the Type of the Predicate is an Int Constant
			if(t1->get_kind()!=TYPE_CONSTANT ||
				(static_cast<ConstantType*>(t1))->get_constant_type()!=INT_CONSTANT)
				reportError("Predicate in conditional must be an integer");
			// Do Type inference on both possible branches
			Type* t2 = infer(e2);
			Type* t3 = infer(e3);
			// Verify that both branches return the same type
			if(t2->get_kind()!=t3->get_kind()) {
				reportError("Then and Else types in conditional must match");
			}
			res = t2;
			break;
		}
		case AST_NIL:
		{
			res = NilType::make("nil");
			break;
		}
		case AST_LIST:
		{
			break;
		}
		case AST_UNOP:
		{
			AstUnOp* b = static_cast<AstUnOp*>(e);
			res = infer_unop(b);
			break;
		}
		case AST_READ:
		{
			AstRead* r = static_cast<AstRead*>(e);
			// Check which type of read it is and set the type to the correct ConstantType
			if(r->read_integer()) {
				res = ConstantType::make("int", INT_CONSTANT);
			}
			else {
				res = ConstantType::make("string", STRING_CONSTANT);
			}
			break;
		}
	}
	return res;
}

Type* TypeInference::infer_binop(AstBinOp *e){
	// Get and Infer the first and second expressions of the BinOps
	Expression * e1=e->get_first();
	Expression * e2=e->get_second();
	Type* infer_e1 = infer(e1);
	Type* infer_e2 = infer(e2);

	if(e->get_binop_type() == CONS) {
		// If both Expressions are of Variable Type
		if(infer_e1->get_kind() == TYPE_VARIABLE &&
			infer_e2->get_kind() == TYPE_VARIABLE) {
			// Return a VariableType
			return VariableType::make("list");
		}
		// If only the first Expression is of VariableType
		if(infer_e1->get_kind() == TYPE_VARIABLE) {
			// If the second Expression is Constant
			if(infer_e2->get_kind() == TYPE_CONSTANT){
				// Set The ListType according to the respective ConstantType
				ConstantType* const2 = static_cast<ConstantType*>(infer_e2);
				if(const2->get_constant_type() == INT_CONSTANT) {
					infer_e1->unify(ConstantType::make("int",INT_CONSTANT));
					return IntListType::make("list",2);
				}
				if(const2->get_constant_type() == STRING_CONSTANT) {
					infer_e1->unify(ConstantType::make("string",STRING_CONSTANT));
					return StringListType::make("list",2);
				}
			}
			// If the second Expression is an IntList
			if(infer_e2->get_kind() == TYPE_INT_LIST){
				// Return an IntList Type
				infer_e1->unify(ConstantType::make("int",INT_CONSTANT));
				IntListType* list = static_cast<IntListType*>(infer_e2);
				list->size++;
				return list;
			}
			// If the second Expression is a StringList
			if(infer_e2->get_kind() == TYPE_STRING_LIST){
				// Return a StringList Type
				infer_e1->unify(ConstantType::make("string",STRING_CONSTANT));
				StringListType* list = static_cast<StringListType*>(infer_e2);
				list->size++;
				return list;
			}
			// Report an error if an invalid Type is used
			reportError("Cons Using Invalid Type");
		}
		// If the second Expression is of VariableType
		if(infer_e2->get_kind() == TYPE_VARIABLE) {
			// If the first Expression is of ConstantType
			if(infer_e1->get_kind() == TYPE_CONSTANT) {
				// Set The ListType according to the respective ConstantType
				ConstantType* const1 = static_cast<ConstantType*>(infer_e1);
				if(const1->get_constant_type()==INT_CONSTANT)
					return IntListType::make("list",1);
				if(const1->get_constant_type()==STRING_CONSTANT)
					return StringListType::make("list",1);
			}
			// Report an Error if using an Invalid Type
			reportError("Cons Using Invalid Type");
		}
		// If Both Expressions are of Constant Type
		if(infer_e1->get_kind() == TYPE_CONSTANT) {
			ConstantType* const1 = static_cast<ConstantType*>(infer_e1);
			// If the first Expression is an Int Constant
			if(const1->get_constant_type() == INT_CONSTANT){
				// If the second Expression is an Int Constant or an IntList
				// return an IntList Type
				if(infer_e2->get_kind() == TYPE_CONSTANT){
					ConstantType* const2 = static_cast<ConstantType*>(infer_e2);
					if(const1->get_constant_type() == const2->get_constant_type()){
						return IntListType::make("intList",2);
					}
				}
				if(infer_e2->get_kind() == TYPE_INT_LIST){
					IntListType* list = static_cast<IntListType*>(infer_e2);
					list->size++;
					return list;
				}	
			}
			// If the first Expression is a String Constant
			else if(const1->get_constant_type() == STRING_CONSTANT){
				// If the second Expression is a String Constant or a StringList
				// return an StringList Type
				if(infer_e2->get_kind() == TYPE_CONSTANT){
					ConstantType* const2 = static_cast<ConstantType*>(infer_e2);
					if(const1->get_constant_type() == const2->get_constant_type()){
						return StringListType::make("stringList",2);
					}
				}
				if(infer_e2->get_kind() == TYPE_STRING_LIST){
					StringListType* list = static_cast<StringListType*>(infer_e2);
					list->size++;
					return list;
				}
			}
			// Report an error if there is an invalid type being used
			reportError("Const is being used with Invalid Types");
		}
		// Report an error if the first object is not of Constant Type
		reportError("First object in Cons must be of Constant Type");
	}
	
	// Unify the two Types
	bool unifyRet = infer_e1->unify(infer_e2);

	// Check if the Unify failed
	if(!unifyRet){
		reportError("Unified Failed in BinOp");
	}

	// Relocate the first Type in case it was changed
	infer_e1 = infer_e1->find();
	// If Both are Variable type return a variable type
	if(infer_e1->get_kind()==TYPE_VARIABLE) {
		return infer_e1;
	}
	// If the BinOp is PLUS
	if(e->get_binop_type() == PLUS) {
		// Make sure they are both of Type Constant
		if(!(infer_e1->get_kind()==TYPE_CONSTANT)) {
			reportError("Type has to be of Type Constant for plus");
		}
		return infer_e1;
	}
	// If the BinOp is Eq or NEQ
	else if(e->get_binop_type() == EQ ||
			e->get_binop_type() == NEQ) {
		// Check to make sure valid types are being used
		if(!(infer_e1->get_kind()==TYPE_CONSTANT ||
				infer_e1->get_kind()==TYPE_NIL)) {
			reportError("Type Error in EQ");
		}
		// Return an Int Constant
		return ConstantType::make("int", INT_CONSTANT);
	}
	// If the BinOp is a Generic BinOp
	else if(e->get_binop_type() == MINUS ||
			e->get_binop_type() == TIMES ||
			e->get_binop_type() == DIVIDE ||
			e->get_binop_type() == AND ||
			e->get_binop_type() == OR ||
			e->get_binop_type() == LT ||
			e->get_binop_type() == LEQ ||
			e->get_binop_type() == GT ||
			e->get_binop_type() == GEQ) {
		ConstantType* t1 = static_cast<ConstantType*>(infer_e1);
		// Make sure the Expressions are Int Constants
		if(!(t1->get_constant_type() == INT_CONSTANT)) {
			reportError("Type Mismatch in Generic Bin Op");
		}
		return infer_e1;
	}
	// Report an Error if an Invalid BinOp is being used
	reportError("Invalid BinOp");
}

Type* TypeInference::infer_unop(AstUnOp *b){
	// Infer on the Expression
	Expression* e = b->get_expression();
	Type* infer_e = infer(e);

	// If the UnOp is a PRINT or ISNIL
	if(b->get_unop_type() == PRINT ||
		b->get_unop_type() == ISNIL) {
		// Return an Int Constant
		return ConstantType::make("int", INT_CONSTANT);
	}
	// If the UnOp is a HD
	else if(b->get_unop_type() == HD) {
		// If the Type is an IntList return an Int Constant
		if(infer_e->get_kind() == TYPE_INT_LIST)
			return ConstantType::make("int",INT_CONSTANT);
		// If the Type is a StringList return a String Constant
		if(infer_e->get_kind() == TYPE_STRING_LIST)
			return ConstantType::make("string",STRING_CONSTANT);
		// Else return the type of the Expression
		return infer_e;
	// If the UnOp is a TL
	}else if(b->get_unop_type() == TL) {
		// If the Type is an IntList
		if(infer_e->get_kind() == TYPE_INT_LIST) {
			// Return an IntList
			IntListType* list = static_cast<IntListType*>(infer_e);
			if(list->size<3)
				return ConstantType::make("int",INT_CONSTANT);
			list->size--;
			return list;
		}
		// If the Type is a StringList
		if(infer_e->get_kind() == TYPE_STRING_LIST) {
			// Return a StringList
			StringListType* list = static_cast<StringListType*>(infer_e);
			if(list->size<=3)
				return ConstantType::make("string",STRING_CONSTANT);
			list->size--;
			return list;
		}
		// Otherwise return a Nil Type
		return NilType::make("nil");
	}
	// Report an error if an invalid UnOp was used
	reportError("Invalid Unop");
}

Type* TypeInference::infer_expression_list(AstExpressionList *l){
	// to be implemented mabye
	reportError("This functionality has not been implemented yet");
	return NULL;
}
