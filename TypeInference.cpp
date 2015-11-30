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
	type_tab.push();
	this->program = e;
	Type::print_all_types();
	Type* t = infer(e);
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
	cout << "Type Inference Error :: " + err << endl;
	exit(1);
}

Type* TypeInference::infer(Expression *e){
	Type* res = NULL;

	switch(e->get_type()) {
		case AST_BINOP:				//case 0
		{
			//cout << "in switch case binop" << endl;
			AstBinOp* b = static_cast<AstBinOp*>(e);
			res = infer_binop(b);
			break;
		}
		case AST_IDENTIFIER:		//case 1
		{
			AstIdentifier* id = static_cast<AstIdentifier*>(e);
			Type* type = type_tab.find(id);
			if(type==NULL) {
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
			vector<Type*> args;
			args.push_back(VariableType::make("param"));
			args.push_back(VariableType::make("body"));
			res = FunctionType::make("lambda",args);
			break;
		}
		case AST_LET:				//case 4
		{
			AstLet* let = static_cast<AstLet*>(e);
			AstIdentifier* id = static_cast<AstIdentifier*>(let->get_id());
			Type* type = infer(let->get_val());
			type_tab.push();
			type_tab.add(id, type);
			res = infer(let->get_body());
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
			break;
		}
		case AST_BRANCH:
		{
			AstBranch* branch = static_cast<AstBranch*>(e);
			Expression* e1 = branch->get_pred();
			Expression* e2 = branch->get_then_exp();
			Expression* e3 = branch->get_else_exp();
			Type* t1 = infer(e1);
			if(t1->get_kind()!=TYPE_CONSTANT ||
				(static_cast<ConstantType*>(t1))->get_constant_type()!=INT_CONSTANT)
				reportError("Predicate in conditional must be an integer");
			Type* t2 = infer(e2);
			Type* t3 = infer(e3);
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
	Expression * e1=e->get_first();
	Expression * e2=e->get_second();
	Type* infer_e1 = infer(e1);
	Type* infer_e2 = infer(e2);

	if(e->get_binop_type() == CONS) {
		if(infer_e1->get_kind() == TYPE_CONSTANT) {
			ConstantType* const1 = static_cast<ConstantType*>(infer_e1);
			
			if(const1->get_constant_type() == INT_CONSTANT){
				if(infer_e2->get_kind() == TYPE_CONSTANT){
					ConstantType* const2 = static_cast<ConstantType*>(infer_e2);
					if(const1->get_constant_type() == const2->get_constant_type()){
						return IntListType::make("intList",2);
					}
					reportError("Cons Using Invalid Type");
					//cout << "in binop cons" << endl;
					//assert(false);
				}
				if(infer_e2->get_kind() == TYPE_INT_LIST){
					IntListType* list = static_cast<IntListType*>(infer_e2);
					list->size++;
					return list;
				}
				reportError("Cons Using Invalid Type");
			}
			else if(const1->get_constant_type() == STRING_CONSTANT){
				if(infer_e2->get_kind() == TYPE_CONSTANT){
					ConstantType* const2 = static_cast<ConstantType*>(infer_e2);
					if(const1->get_constant_type() == const2->get_constant_type()){
						return StringListType::make("stringList",2);
					}
					reportError("Cons Using Invalid Type");
				}
				if(infer_e2->get_kind() == TYPE_STRING_LIST){
					StringListType* list = static_cast<StringListType*>(infer_e2);
					list->size++;
					return list;
				}
				reportError("Cons Using Invalid Type");
			}
			else{
				reportError("Cons Using Invalid Type");
			}
		}
		reportError("First object in Cons must be of Constant Type");
	}
	else if(e->get_binop_type() == PLUS) {
		if(!(infer_e1->unify(infer_e2))) {
			reportError("Unified Failed in Plus");
		}
		if(!(infer_e1->get_kind()==TYPE_CONSTANT)) {
			reportError("Type has to be of Type Constant for plus");
		}
		return infer_e1;
	}
	else if(e->get_binop_type() == EQ ||
			e->get_binop_type() == NEQ) {
		if(!(infer_e1->unify(infer_e2))) {
			reportError("Unified Failed in EQ");
		}
		if(!(infer_e1->get_kind()==TYPE_CONSTANT ||
				infer_e1->get_kind()==TYPE_NIL)) {
			reportError("Type Error in EQ");
		}
		return ConstantType::make("int", INT_CONSTANT);
	}
	else if(e->get_binop_type() == MINUS ||
			e->get_binop_type() == TIMES ||
			e->get_binop_type() == DIVIDE ||
			e->get_binop_type() == AND ||
			e->get_binop_type() == OR ||
			e->get_binop_type() == LT ||
			e->get_binop_type() == LEQ ||
			e->get_binop_type() == GT ||
			e->get_binop_type() == GEQ) {
		if(!infer_e1->unify(infer_e2)) {
			reportError("Unified Failed in Generic Bin Op");
		}
		ConstantType* t1 = static_cast<ConstantType*>(infer_e1);
		if(!(t1->get_constant_type() == INT_CONSTANT)) {
			reportError("Type Mismatch in Generic Bin Op");
		}
		return infer_e1;
	}
	reportError("Invalid BinOp");
}

Type* TypeInference::infer_unop(AstUnOp *b){
	Expression* e = b->get_expression();
	Type* infer_e = infer(e);

	if(b->get_unop_type() == PRINT ||
		b->get_unop_type() == ISNIL) {
		return ConstantType::make("int", INT_CONSTANT);
	}
	else if(b->get_unop_type() == HD) {
		//cout << "in unop hd/tl" << endl;
		if(infer_e->get_kind() == TYPE_INT_LIST)
			return ConstantType::make("int",INT_CONSTANT);
		if(infer_e->get_kind() == TYPE_STRING_LIST)
			return ConstantType::make("string",STRING_CONSTANT);
		return infer_e;
	}else if(b->get_unop_type() == TL) {
		if(infer_e->get_kind() == TYPE_INT_LIST) {
			IntListType* list = static_cast<IntListType*>(infer_e);
			if(list->size<3)
				return ConstantType::make("int",INT_CONSTANT);
			list->size--;
			return list;
		}
		if(infer_e->get_kind() == TYPE_STRING_LIST) {
			StringListType* list = static_cast<StringListType*>(infer_e);
			if(list->size<=3)
				return ConstantType::make("string",STRING_CONSTANT);
			list->size--;
			return list;
		}
		return NilType::make("nil");
	}
	reportError("Invalid Unop");
}

Type* TypeInference::infer_expression_list(AstExpressionList *l){
	// to be implemented
	return NULL;
}
