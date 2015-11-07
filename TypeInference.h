

#ifndef TYPE_INFERENCE_H_
#define TYPE_INFERENCE_H_


#include "SymbolTable.h"

class Expression;
class AstBinOp;
class AstUnOp;
class AstExpressionList;

class TypeInference {
private:
	Expression* program;
public:
	TypeInference(Expression* p);
	void infer(Expression* e);
	void infer_binop(AstBinOp *e);
	void infer_unop(AstUnOp *e);
	void infer_expression_list(AstExpressionList* l);
};

#endif /* TYPE_INFERENCE_H_ */
