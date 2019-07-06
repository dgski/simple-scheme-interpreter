#include <chrono>

#include "interpreter.h"

Expression eval(Expression& exp, Environments& env)
{
    return std::visit(Evaluator{env},exp);
}

std::optional<Expression> applySpecialForm(SymbolInstance& formName, List& args, Environments& env)
{
    if(formName == "def")
    {
        env.set(std::get<SymbolInstance>(args.at(0)), eval(args.at(1), env));
        return NullInstance{};
    }
    else if(formName == "if")
    {
        if(std::get<BooleanInstance>(eval(args.at(0), env)))
        {
            return eval(args.at(1), env);
        }
        else 
        {
            return eval(args.at(2), env);
        }
    }
    else if(formName == "quote")
    {
        return args.at(0);
    }
    else if(formName == "env")
    {
        std::cout << env << std::endl;
        return NullInstance{};
    }
    else if(formName == "lambda")
    {
        auto argNames = args.at(0);
        auto body = args.at(1);

        return Closure{[newEnv = env, body, argNames](List& args) mutable
        {
            newEnv.add();
            if(!std::holds_alternative<NullInstance>(argNames))
            {
                insertArgsIntoEnvironment(std::get<PairInstance>(argNames), std::get<PairInstance>(args.all()), *newEnv.back());
            }
            return eval(body, newEnv);
        }};
    }

    return std::nullopt;
}

std::optional<Expression> getPrimitiveFunction(SymbolInstance& s)
{
    if(s == "+")
    {
        return Closure{[](List& args)
        {
            auto arg0 = std::get<IntegerInstance>(args.at(0));
            auto arg1 = std::get<IntegerInstance>(args.at(1));
            return IntegerInstance{ arg0 + arg1 };
        }};
    }
    else if(s == "-")
    {
        return Closure{[](List& args)
        {
            auto arg0 = std::get<IntegerInstance>(args.at(0));
            auto arg1 = std::get<IntegerInstance>(args.at(1));
            return IntegerInstance{ arg0 - arg1 };
        }};
    }

    return std::nullopt;
}



Expression Evaluator::operator()(SymbolInstance& s)
{
    auto func = getPrimitiveFunction(s);
    if(func.has_value())
    {
        return func.value();
    }

    return env.get(s);
}

Expression Evaluator::operator()(PairInstance& p)
{   
    if(std::holds_alternative<SymbolInstance>(*p.first))
    {   
        List args{*p.second};
        auto res = applySpecialForm(std::get<SymbolInstance>(*p.first), args, env);
        if(res.has_value())
        {
            return res.value();
        }
    }
    
    auto first = eval(*p.first, env);
    
    if(std::holds_alternative<Closure>(first))
    {
        auto evaluatedList = evalAllArgsInList(*p.second, env);
        List args{ evaluatedList };
        return std::get<Closure>(first)(args);
    }

    return NullInstance{};
}

void insertArgsIntoEnvironment(PairInstance& names, PairInstance& args, Environment& env)
{
    env[std::get<SymbolInstance>(*names.first)] = *args.first;

    if(!std::holds_alternative<NullInstance>(*names.second))
        insertArgsIntoEnvironment(std::get<PairInstance>(*names.second), std::get<PairInstance>(*args.second), env);
}

Expression evalAllArgsInList(Expression& exp, Environments& env)
{
    if(std::holds_alternative<NullInstance>(exp))
        return NullInstance{};

    auto p = std::get<PairInstance>(exp);
    auto first = eval(*p.first, env);
    auto second = std::holds_alternative<NullInstance>(*p.second) ?
        NullInstance{} :
        evalAllArgsInList(*p.second, env);

    return PairInstance{
        std::make_shared<Expression>(first),
        std::make_shared<Expression>(second)
    };
}

std::ostream& operator<<(std::ostream& os, const Expression& env)
{
    return std::visit(ExpressionStream{os}, env);
}