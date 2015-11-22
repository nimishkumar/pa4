#include "NilType.h"
#include "ConstantType.h"
#include "VariableType.h"
#include "FunctionType.h"
#include "ast/expression.h"
#include "TypeInference.h"


TypeInference::TypeInference(Expression * e)
{
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

Type* TypeInference::infer(Expression *e){
	Type* res = NULL;
	cout << "before switch" << endl;

	switch(e->get_type()) {
		case AST_BINOP:
		{
			cout << "in switch case binop" << endl;
			AstBinOp* b = static_cast<AstBinOp*>(e);
			res = infer_binop(b);
			break;
		}
		case AST_INT:
		{
			res = ConstantType::make("int", INT_CONSTANT);
			break;
		}
		case AST_LAMBDA:
		{
			//res = FunctionType
			break;
		}
		case AST_STRING:
		{
			res = ConstantType::make("string", STRING_CONSTANT);
			break;
		}
		case AST_NIL:
		{
			res = NilType::make("nil");
			break;
		}
		case AST_UNOP:
		{
			AstUnOp* b = static_cast<AstUnOp*>(e);
			res = infer_unop(b);
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
		assert(infer_e1->unify(infer_e2));
		cout << "unified in cons" << endl;
		assert(false);
	}
	else if(e->get_binop_type() == PLUS) {
		assert(infer_e1->unify(infer_e2));
		cout << "unified in plus" << endl;
		assert(infer_e1->get_kind()==TYPE_CONSTANT);
		cout << "retuning in plus" << endl;
		return infer_e1;
	}
	else if(e->get_binop_type() == EQ) {
		assert(infer_e1->unify(infer_e2));
		cout << "unified in equals" << endl;
		assert(infer_e1->get_kind()==TYPE_CONSTANT ||
				infer_e1->get_kind()==TYPE_NIL);
		return ConstantType::make("int", INT_CONSTANT);
	}
	else if(e->get_binop_type() == MINUS ||
			e->get_binop_type() == TIMES ||
			e->get_binop_type() == DIVIDE ||
			e->get_binop_type() == AND ||
			e->get_binop_type() == OR ||
			e->get_binop_type() == NEQ ||
			e->get_binop_type() == LT ||
			e->get_binop_type() == LEQ ||
			e->get_binop_type() == GT ||
			e->get_binop_type() == GEQ) {
		assert(infer_e1->unify(infer_e2));
		cout << "unified in binop" << endl;
		ConstantType* t1 = static_cast<ConstantType*>(infer_e1);
		assert(t1->get_constant_type() == INT_CONSTANT);
		return infer_e1;
	}
	assert(false);
}

Type* TypeInference::infer_unop(AstUnOp *b){
	Expression* e = b->get_expression();
	Type* infer_e = infer(e);

	if(b->get_unop_type() == PRINT ||
		b->get_unop_type() == ISNIL) {
		return ConstantType::make("int", INT_CONSTANT);
	}
	else if(b->get_unop_type() == HD ||
			b->get_unop_type() == TL) {
		cout << "in unop hd/tl" << endl;
		assert(false);
	}
	assert(false);
}

Type* TypeInference::infer_expression_list(AstExpressionList *l){
	// to be implemented
	return NULL;
}
