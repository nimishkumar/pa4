

#ifndef TYPE_INFERENCE_H_
#define TYPE_INFERENCE_H_

#include "TypeTable.h"

class Expression;
class AstBinOp;
class AstUnOp;
class AstExpressionList;
class Type;

class TypeInference {
private:
	Expression* program;
	TypeTable type_tab;
public:
	TypeInference(Expression* p);
	Type* infer(Expression* e);
	Type* infer_binop(AstBinOp *e);
	Type* infer_unop(AstUnOp *e);
	Type* infer_expression_list(AstExpressionList* l);
	void reportError(string err);
};

#endif /* TYPE_INFERENCE_H_ */
