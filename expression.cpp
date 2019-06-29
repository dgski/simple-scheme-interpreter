#include "expression.h"

Expression eval(Expression exp, Environment& env)
{
    return std::visit(Evaluator{env},exp);
}

Expression apply(Symbol funcName, Expression args, Environment& env)
{
    if(funcName == "addOne")
    {
        auto arg = *std::get<Pair>(args).first;
        return Integer{std::get<Integer>(eval(arg,env)) + 1};
    }
    else if(funcName == "minusOne")
    {
        auto arg = *std::get<Pair>(args).first;
        return Integer{std::get<Integer>(eval(arg,env)) - 1};
    }
    else if(funcName == "=")
    {
        auto arg1 = std::get<Integer>(eval(*std::get<Pair>(args).first, env));
        auto arg2 = std::get<Integer>(eval(*std::get<Pair>(*std::get<Pair>(args).second).first, env));
        return Boolean{arg1 == arg2};
    }
    else if(funcName == "cons")
    {
        auto arg1 = *std::get<Pair>(args).first;
        auto arg2 = *std::get<Pair>(*std::get<Pair>(args).second).first;
        return Pair{
            std::make_shared<Expression>(eval(arg1,env)),
            std::make_shared<Expression>(eval(arg2,env))
        };
    }
    else if(funcName == "define")
    {
        auto arg1 = *std::get<Pair>(args).first;
        auto arg2 = *std::get<Pair>(*std::get<Pair>(args).second).first;
        env[std::get<Symbol>(arg1)] = eval(arg2, env);
        return Null{};
    }
    else if(funcName == "if")
    {
        auto arg1 = *std::get<Pair>(args).first;
        auto arg2 = *std::get<Pair>(*std::get<Pair>(args).second).first;
        auto arg3 = *std::get<Pair>(*std::get<Pair>(*std::get<Pair>(args).second).second).first;
        
        if(std::get<Boolean>(eval(arg1, env)))
        {
            return eval(arg2, env);
        }
        else 
        {
            return eval(arg3, env);
        }
    }
    
    auto envFunc = std::get<Function>(env[funcName]);
    return envFunc(evalAllArgsInList(std::get<Pair>(args), env));
}

std::ostream& operator<<(std::ostream& os, const Expression& env)
{
    return std::visit(ExpressionStream{os}, env);
}

void insertArgsIntoEnvironment(Pair& names, Pair& args, Environment& env)
{
    env[std::get<Symbol>(*names.first)] = *args.first;

    if(!std::holds_alternative<Null>(*names.second))
        insertArgsIntoEnvironment(std::get<Pair>(*names.second), std::get<Pair>(*args.second), env);
}

Expression evalAllArgsInList(Pair& p, Environment& env)
{
    auto first = eval(*p.first, env);
    auto second = std::holds_alternative<Null>(*p.second) ?
        Null{} :
        evalAllArgsInList(std::get<Pair>(*p.second), env);

    return Pair{
        std::make_shared<Expression>(first),
        std::make_shared<Expression>(second)
    };
}