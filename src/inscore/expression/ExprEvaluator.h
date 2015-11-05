#ifndef EXPREVALUATOR_H
#define EXPREVALUATOR_H

#include <string>
#include <unordered_map>

#include "smartpointer.h"

#include "evaluator.h"
#include "IExpression.h"
#include "Operators.h"

namespace inscore{

class IObject;

/*!
 * \brief Handle the status of the evaluator during the processing of the evaluation (failing...)
 */
class EvaluationStatus{
    bool fEvalSucceed;

public:
    EvaluationStatus();
    void init();

	inline std::string fail(const std::string defaultValue){if(defaultValue.empty()){return fail();} return defaultValue;}
	inline std::string fail(){fEvalSucceed = false; return "";}
    bool hasEvalSucceed(){return fEvalSucceed;}
};

typedef const std::string (*OperatorCb)(const std::string&,const std::string&);

/*!
 * \brief IEvaluableExpr evaluator mother class. Handle context for the evaluation, error...
 */
class ExprEvaluator: public constEvaluator{
	const char* fEvalName;
	const IObject* fContextObject;
public:
	/*!
	 * \brief evaluate an expression according to previously evaluated values
	 * \param expr expression to evaluate
	 * \param result result of the evaluation
	 * \param newEvaluatedMap previously evaluated values
	 * \return true if the evaluation succeed, false otherwise
	 *
	 * \warning evalExpression is not reentrant
	 */
	bool evalExpression(const IExpression *expr, std::string &result);

	virtual const std::string eval(const IExprOperator* arg, const IExpression* exprArg=0);
	virtual const std::string eval(const std::string& arg, const IExpression* exprArg=0);
	virtual const std::string eval(const filepath& arg, const IExpression* exprArg=0);
	virtual const std::string eval(const itladdress& arg, const IExpression* exprArg=0);

	virtual const std::string eval(const IObject *arg);


	static ExprEvaluator* create(const IObject* contextObject){return new ExprEvaluator("ExprEvaluator", contextObject);}

	const char* evaluatorName() const {return fEvalName;}


	static const IObject* objectFromAddress(itladdress address, const IObject *contextObject);

protected:
	ExprEvaluator(const char* name, const IObject* contextObject);

	EvaluationStatus fEvalStatus;

	inline const IObject* contextObject(){return fContextObject;}
	bool smartEval(const IExpression *expr, std::string &result);


	std::unordered_map<const OperatorPrototype*, OperatorCb> fCallbackList;
	void registerOperator(const OperatorPrototype &op, OperatorCb cb);
	bool callbackByOperator(const OperatorPrototype* op, OperatorCb &cb) const ;
};

}

#endif // EXPREVALUATOR_H
