#include "expression.h"

Expression eval(Expression exp, Environment env)
{
    return std::visit(Evaluator{env},exp);
}

Expression apply(Symbol funcName, Expression args, Environment env)
{
    auto arg = *std::get<Pair>(args).first;

    if(funcName == "addOne")
    {
        return Integer{std::get<Integer>(eval(arg,env)) + 1};
    }
    else if(funcName == "square")
    {
        auto value = std::get<Integer>(eval(arg,env));
        return Integer{value * value};
    }

    auto envFunc = std::get<Function>(env[funcName]);
    return envFunc(eval(arg, env));
}

std::ostream& operator<<(std::ostream& os, const Expression& e)
{
    return std::visit(ExpressionStream{os}, e);
}