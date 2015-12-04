#include <fstream>
#include "ITLError.h"

#include "ExprEvaluator.h"

#include "IObject.h"
#include "IScene.h"
#include "IExpressionHandler.h"
#include "IExprParser.h"
#include "QFileDownloader.h"


using namespace std;

namespace inscore{

bool ExprEvaluator::evalExpression(const IExpression *expr, std::string& result){
	fEvalStatus.init();
	string r;
	smartEval(expr->rootNode(), r);
	if(fEvalStatus.hasEvalSucceed())
		result = r;
	return fEvalStatus.hasEvalSucceed();
}

//_____________________________________________________________
bool ExprEvaluator::smartEval(const IExprArgBase *expr, string& result)
{
	result = expr->getEvaluated();					//Storing the previously evaluated value in result ("" if no value)

	if(!expr->dynamicEval() && !result.empty())	//If expr is static and already evaluated
		return false;							//result = previously evaluated value

	string e = expr->accept(this);				//We evaluate

	if(e==result && !e.empty())					//If the result is the same and not null
		return false;							//result = previously evaluated value

	result = e;									//result = new value
	*expr->evaluated() = e;						//updating evaluated value

	return true;
}


//_____________________________________________________________
const string ExprEvaluator::eval(const IExprOperator* arg, const IExprArgBase *exprArg)
{
	OperatorCb cb;

	if( !callbackByOperator(arg->operatorPrototype(), cb) ){
		ITLErr<<fEvalName	<<": operator \""
							<<arg->operatorPrototype()->getName()
							<<"\" has no definition in this evaluator";
		return fEvalStatus.fail();
	}

	string arg1, arg2;
	bool changed = exprArg->getEvaluated().empty();	//If the operator had never been evaluated we need to force the evaluation
													//even if arg1 and arg2 didn't change (happening when tree are composed together)
	changed |= smartEval(arg->constArg1(), arg1);
	changed |= smartEval(arg->constArg2(), arg2);

	if(!fEvalStatus.hasEvalSucceed())				//If the evaluation failed during evaluation of arg1 or arg2
		return "";									//Return empty string (anyway the value shouldn't be read in afterward...)

	if(!changed)									//If arg1 and arg2 didn't change
		return exprArg->getEvaluated();				//Return the previously evaluated value

	if(arg1.empty())
		arg1 = emptyValue();
	if(arg2.empty())
		arg2 = emptyValue();

	bool success = false;
	string r = cb(arg1, arg2, success);

	if(!success){
		ITLErr<<fEvalName<<": operator "<<arg->operatorPrototype()->getName()<<"("<<arg1<<", "<<arg2<<") failed to compute.";
		return fEvalStatus.fail(exprArg);
	}

	return r;


}

//_____________________________________________________________
const string ExprEvaluator::eval(const std::string &arg, const IExprArgBase *)
{
    return arg;
}

//_____________________________________________________________
const string ExprEvaluator::eval(const filepath& arg, const IExprArgBase *exprArg)
{

    std::string fileData="";
    std::ifstream ifs;

	ifs.open(fContextObject->getScene()->absolutePath(arg), std::ifstream::in);

    if(!ifs.is_open()){
		ITLErr<<fEvalName<<": can't find \""<<(string)arg<<"\""<<ITLEndl;
		return fEvalStatus.fail(exprArg);
    }

    char c = ifs.get();
    while (ifs.good()) {
        fileData += c;
        c = ifs.get();
      }

    ifs.close();

	return eval(fileData);
}


//_____________________________________________________________
const string ExprEvaluator::eval(const urlpath &arg, const IExprArgBase *exprArg)
{
	QFileDownloader* downloader = new QFileDownloader(fContextObject->getScene()->getRootPath().c_str());

	if(!downloader){
		ITLErr<<fEvalName<<": QFileDownloader not available..."<<ITLEndl;
		return fEvalStatus.fail(exprArg);
	}

//	QObject::connect(downloader, &QFileDownloader::failed, [ arg, this, downloader]
//		{
//			//sITLErr<<this->fEvalName<<": "<<(std::string)arg<<" failed to download."<<ITLEndl;
//			downloader->deleteLater();
//		});


//	QObject::connect(downloader, &QFileDownloader::downloaded, [ exprArg, downloader]
//		{
//			*exprArg->evaluated() = std::string(downloader->data());
//			ITLErr<<"url retreived: "<<downloader->data();

//			downloader->deleteLater();
//		});

//	downloader->getAsync( ((std::string)arg).c_str(), fContextObject->getOSCAddress().c_str() );

	return exprArg->getEvaluated().empty()?emptyValue():exprArg->getEvaluated();
}

//_____________________________________________________________
const string ExprEvaluator::eval(const itladdress &arg, const IExprArgBase *exprArg)
{
	const IObject* o = objectFromAddress(arg, fContextObject);

	if(!o){
		ITLErr<<fEvalName<<": "<<(string)arg<<" not known at this address..."<<ITLEndl;
		return fEvalStatus.fail(exprArg);
    }
	return eval(o);
}

//_____________________________________________________________
const string ExprEvaluator::eval(const iexpression &arg, const IExprArgBase *exprArg)
{
	SIExpression expr;
	if(IExprParser::parseExpression(arg, expr)){
		const IExprArgBase* exprTree= expr->rootNode();
		return exprTree->accept(this);
	}

	ITLErr<<fEvalName<<": error incorrect IExpression"<<ITLEndl;
	return fEvalStatus.fail(exprArg);
}

//_____________________________________________________________
const string ExprEvaluator::eval(const IObject *arg)
{
	return "IObject: "+arg->name();
}



//_____________________________________________________________
//_____________________________________________________________
ExprEvaluator::ExprEvaluator(const char *name, const IObject* contextObject):
	fEvalName(name)
{
	fContextObject = contextObject;
}

//_____________________________________________________________
void ExprEvaluator::registerOperator(const OperatorPrototype& op, OperatorCb cb)
{
	fCallbackList.insert({&op, cb});
}

//_____________________________________________________________
bool ExprEvaluator::callbackByOperator(const OperatorPrototype* op, OperatorCb &cb) const
{
	try{
		cb = fCallbackList.at(op);
	}catch(std::out_of_range){
		return false;
	}

	return true;
}


//_____________________________________________________________
const IObject* ExprEvaluator::objectFromAddress(itladdress address, const IObject* contextObject)
{
	return contextObject->findnode(address);
}


//_____________________________________________________________
void EvaluationStatus::init()
{
    fEvalSucceed = true;
}

EvaluationStatus::EvaluationStatus():
	fEvalSucceed(true)
{
}

} //end namespace
