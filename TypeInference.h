

#ifndef TYPE_INFERENCE_H_
#define TYPE_INFERENCE_H_


#include "SymbolTable.h"

class Expression;
class AstBinOp;
class AstUnOp;
class AstExpressionList;
class Type;

class TypeInference {
private:
	Expression* program;
public:
	TypeInference(Expression* p);
	Type* infer(Expression* e);
	Type* infer_binop(AstBinOp *e);
	Type* infer_unop(AstUnOp *e);
	Type* infer_expression_list(AstExpressionList* l);
};

#endif /* TYPE_INFERENCE_H_ */
