#include "interpreter.h"

Expression eval(Expression& exp, Environments& env)
{
    return std::visit(Evaluator{env},exp);
}

std::optional<Expression> applySpecialForm(Symbol& formName, Arguments& args, Environments& env)
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
        auto names = args.at(0);
        auto lambdaBody = args.at(1);
        auto newEnv = env;
        newEnv.add();

        return Function{[newEnv, lambdaBody, names](Arguments& args) mutable
        {
            insertArgsIntoEnvironment(std::get<Pair>(names), args.all(), *newEnv.back());
            return eval(lambdaBody, newEnv);
        }};
    }
    else if(formName == "begin")
    {   
        auto res = evalAllArgsInList(args.all(), env);
        Arguments adaptor{res};
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
        return Function{[](Arguments& args) mutable
        {
            return Integer{std::get<Integer>(args.at(0)) + 1};
        }};
    }
    else if(s == "minusOne")
    {
        return Function{[](Arguments& args) mutable
        {
            return Integer{std::get<Integer>(args.at(0)) - 1};
        }};
    }
    else if(s == "=")
    {
        return Function{[](Arguments& args) mutable
        {
            auto arg0 = std::get<Integer>(args.at(0));
            auto arg1 = std::get<Integer>(args.at(1));
            return Boolean{ arg0 == arg1 };
        }};
    }
    else if(s == "<")
    {
        return Function{[](Arguments& args) mutable
        {
            auto arg0 = std::get<Integer>(args.at(0));
            auto arg1 = std::get<Integer>(args.at(1));
            return Boolean{ arg0 < arg1 };
        }};
    }
    else if(s == "cons")
    {
        return Function{[](Arguments& args) mutable
        {
            return Pair{
                std::make_shared<Expression>(args.at(0)),
                std::make_shared<Expression>(args.at(1))
            };
        }};
    }
    else if(s == "first")
    {
        return Function{[](Arguments& args) mutable
        {
            return *std::get<Pair>(args.at(0)).first;
        }};
    }
    else if(s == "rest")
    {
        return Function{[](Arguments& args) mutable
        {
            return *std::get<Pair>(args.at(0)).second;
        }};
    }
    else if(s == "null?")
    {
        return Function{[](Arguments& args) mutable
        {
            return Boolean{ std::holds_alternative<Null>(args.at(0)) };
        }};
    }
    else if(s == "eqv?")
    {
        return Function{[](Arguments& args) mutable
        {
            return Boolean{ std::get<Symbol>(args.at(0)) == std::get<Symbol>(args.at(1)) };
        }};
    }
    else if(s == "println")
    {
        return Function{[](Arguments& args) mutable
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
        Arguments args{*p.second};
        auto res = applySpecialForm(std::get<Symbol>(*p.first), args, env);
        if(res.has_value())
        {
            return res.value();
        }
    }
    
    auto first = eval(*p.first, env);
    
    if(std::holds_alternative<Function>(first))
    {
        auto evaluatedList = evalAllArgsInList(std::get<Pair>(*p.second), env);
        Arguments args{ evaluatedList };
        return std::get<Function>(first)(args);
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