#include "interpreter.h"

Expression eval(Expression& exp, Environments& env)
{
    return std::visit(Evaluator{env},exp);
}

std::optional<Expression> applySpecialForm(Symbol& formName, List& args, Environments& env)
{
    if(formName == "define")
    {
        env.set(std::get<Symbol>(args.at(0)), eval(args.at(1), env));
        return Null{};
    }
    else if(formName == "if")
    {
        if(std::get<Boolean>(eval(args.at(0), env)))
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
        return Null{};
    }
    else if(formName == "lambda")
    {
        auto argNames = args.at(0);
        auto body = args.at(1);

        return Closure{[newEnv = env, body, argNames](List& args) mutable
        {
            newEnv.add();
            insertArgsIntoEnvironment(std::get<Pair>(argNames), args.all(), *newEnv.back());
            return eval(body, newEnv);
        }};
    }
    else if(formName == "begin")
    {   
        auto res = evalAllArgsInList(args.all(), env);
        List adaptor{res};
        return adaptor.last();
    }
    else if(formName == "list")
    {
        auto res = evalAllArgsInList(args.all(), env);
        return res;
    }

    return std::nullopt;
}

std::optional<Expression> getPrimitiveFunction(Symbol& s)
{
    if(s == "addOne")
    {
        return Closure{[](List& args)
        {
            return Integer{std::get<Integer>(args.at(0)) + 1};
        }};
    }
    else if(s == "minusOne")
    {
        return Closure{[](List& args)
        {
            return Integer{std::get<Integer>(args.at(0)) - 1};
        }};
    }
    else if(s == "=")
    {
        return Closure{[](List& args)
        {
            auto arg0 = std::get<Integer>(args.at(0));
            auto arg1 = std::get<Integer>(args.at(1));
            return Boolean{ arg0 == arg1 };
        }};
    }
    else if(s == "<")
    {
        return Closure{[](List& args)
        {
            auto arg0 = std::get<Integer>(args.at(0));
            auto arg1 = std::get<Integer>(args.at(1));
            return Boolean{ arg0 < arg1 };
        }};
    }
    else if(s == "cons")
    {
        return Closure{[](List& args)
        {
            return Pair{
                std::make_shared<Expression>(args.at(0)),
                std::make_shared<Expression>(args.at(1))
            };
        }};
    }
    else if(s == "first")
    {
        return Closure{[](List& args)
        {
            return *std::get<Pair>(args.at(0)).first;
        }};
    }
    else if(s == "rest")
    {
        return Closure{[](List& args)
        {
            return *std::get<Pair>(args.at(0)).second;
        }};
    }
    else if(s == "null?")
    {
        return Closure{[](List& args)
        {
            return Boolean{ std::holds_alternative<Null>(args.at(0)) };
        }};
    }
    else if(s == "eqv?")
    {
        return Closure{[](List& args)
        {
            return Boolean{ std::get<Symbol>(args.at(0)) == std::get<Symbol>(args.at(1)) };
        }};
    }
    else if(s == "println")
    {
        return Closure{[](List& args)
        {
            std::cout << args.at(0) << '\n';
            return Null{};
        }};
    }

    return std::nullopt;
}

std::ostream& operator<<(std::ostream& os, const Expression& env)
{
    return std::visit(ExpressionStream{os}, env);
}

Expression Evaluator::operator()(Symbol& s)
{
    auto func = getPrimitiveFunction(s);
    if(func.has_value())
    {
        return func.value();
    }

    return env.get(s);
}

Expression Evaluator::operator()(Pair& p)
{   
    if(std::holds_alternative<Symbol>(*p.first))
    {   
        List args{*p.second};
        auto res = applySpecialForm(std::get<Symbol>(*p.first), args, env);
        if(res.has_value())
        {
            return res.value();
        }
    }
    
    auto first = eval(*p.first, env);
    
    if(std::holds_alternative<Closure>(first))
    {
        auto evaluatedList = evalAllArgsInList(std::get<Pair>(*p.second), env);
        List args{ evaluatedList };
        return std::get<Closure>(first)(args);
    }

    return Null{};
}

void insertArgsIntoEnvironment(Pair& names, Pair& args, Environment& env)
{
    env[std::get<Symbol>(*names.first)] = *args.first;

    if(!std::holds_alternative<Null>(*names.second))
        insertArgsIntoEnvironment(std::get<Pair>(*names.second), std::get<Pair>(*args.second), env);
}

Expression evalAllArgsInList(Pair& p, Environments& env)
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