#include <chrono>
#include <algorithm>

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
    else if(formName == "cond")
    {
        for(Expression& e : args)
        {
            List subExpr{ e };
            if(std::get<Boolean>(eval(subExpr.at(0), env)))
            {
                return eval(subExpr.at(1), env);
            }
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
        auto& argNames = args.at(0);
        auto& body = args.at(1);

        return Closure{[newEnv = env, body, argNames](List& args) mutable
        {
            newEnv.add();
            if(!std::holds_alternative<Null>(argNames))
            {
                insertArgsIntoEnvironment(std::get<Pair>(argNames), std::get<Pair>(args.all()), *newEnv.back());
            }
            return eval(body, newEnv);
        }};
    }
    else if(formName == "import")
    {
        const auto& fileName = std::get<String>(args.at(0));
        return parseAndEvalFile(fileName.c_str(), env);
    }

    return std::nullopt;
}

std::optional<Expression> getPrimitiveFunction(Symbol& s)
{
    if(s == "+")
    {
        return Closure{[](List& args)
        {
            auto arg0 = std::get<Integer>(args.at(0));
            auto arg1 = std::get<Integer>(args.at(1));
            return Integer{ arg0 + arg1 };
        }};
    }
    else if(s == "-")
    {
        return Closure{[](List& args)
        {
            auto arg0 = std::get<Integer>(args.at(0));
            auto arg1 = std::get<Integer>(args.at(1));
            return Integer{ arg0 - arg1 };
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
    else if(s == "exit")
    {
        return Closure{[](List& args)
        {
            exit(EXIT_SUCCESS);
            return Null{};
        }};
    }
    else if(s == "begin")
    {   
        return Closure{[](List& args)
        {
            return args.last();
        }};
    }
    else if(s == "list")
    {
        return Closure{[](List& args)
        {
            return args.all();
        }};
    }
    else if(s == "current-time")
    {
        return Closure{[](List& args)
        {   
            auto unixTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            );
            return Integer{ unixTime.count() };
        }};
    }
    else if(s == "int?")
    {
        return Closure{[](List& args)
        {
            return std::holds_alternative<Integer>(args.at(0));
        }};
    }
    else if(s == "symbol?")
    {
        return Closure{[](List& args)
        {
            return std::holds_alternative<Symbol>(args.at(0));
        }};
    }
    else if(s == "function?")
    {
        return Closure{[](List& args)
        {
            return std::holds_alternative<Closure>(args.at(0));
        }};
    }
    else if(s == "and")
    {
        return Closure{[](List& args)
        {
            return Boolean{
                std::all_of(args.begin(), args.end(), [](Expression& e)
                {
                    return std::get<Boolean>(e) == Boolean{true};
                })
            };
        }};
    }
    else if(s == "or")
    {
        return Closure{[](List& args)
        {
            return Boolean{
                std::any_of(args.begin(), args.end(), [](Expression& e)
                {
                    return std::get<Boolean>(e) == Boolean{true};
                })
            };
        }};
    }
    else if(s == "string-append")
    {
        return Closure{[](List& args)
        {
            const auto& str0 = std::get<String>(args.at(0));
            const auto& str1 = std::get<String>(args.at(1));

            return String{ str0 + str1 };
        }};
    }
    else if(s == "string-len")
    {
        return Closure{[](List& args)
        {
            const auto& str = std::get<String>(args.at(0));
            return Integer{ (long long)str.length() };
        }};
    }
    else if(s == "string-slice")
    {
        return Closure{[](List& args)
        {
            auto start = std::get<Integer>(args.at(0));
            auto end = std::get<Integer>(args.at(1));
            const auto& str = std::get<String>(args.at(2));
            return String{ str.substr(start, end) };
        }};
    }
    else if(s == "string-equal?")
    {
        return Closure{[](List& args)
        {
            const auto& str0 = std::get<String>(args.at(0));
            const auto& str1 = std::get<String>(args.at(1));

            return Boolean{ str0 == str1 };
        }};
    }

    return std::nullopt;
}

Expression Evaluator::operator()(Symbol& s)
{
    const auto& func = getPrimitiveFunction(s);
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
        List args{ *p.second };
        const auto& res = applySpecialForm(std::get<Symbol>(*p.first), args, env);
        if(res.has_value())
        {
            return res.value();
        }
    }
    
    const auto& first = eval(*p.first, env);
    
    if(std::holds_alternative<Closure>(first))
    {
        auto evaluatedList = evalAllArgsInList(*p.second, env);
        List args{ evaluatedList };
        return std::get<Closure>(first)(args);
    }

    throw std::runtime_error("Invalid Expression");
}

void insertArgsIntoEnvironment(const Pair& names, const Pair& args, Environment& env)
{
    env[std::get<Symbol>(*names.first)] = *args.first;

    if(!std::holds_alternative<Null>(*names.second))
    {
        insertArgsIntoEnvironment(std::get<Pair>(*names.second), std::get<Pair>(*args.second), env);
    }
}

Expression evalAllArgsInList(const Expression& exp, Environments& env)
{
    if(std::holds_alternative<Null>(exp))
    {
        return Null{};
    }

    auto& p = std::get<Pair>(exp);
    const auto& first = eval(*p.first, env);
    const auto& second = std::holds_alternative<Null>(*p.second) ?
        Null{} :
        evalAllArgsInList(*p.second, env);

    return Pair{
        std::make_shared<Expression>(first),
        std::make_shared<Expression>(second)
    };
}

Expression parseAndEvalFile(const char* fileName, Environments& env)
{
    std::ifstream f{fileName};
    if(f.is_open())
    {
        std::stringstream ss;
        ss << f.rdbuf();

        std::string s{ ss.str() };
        if(const auto& parseResult = multiParse(s); parseResult.has_value())
        {
            auto beginForm = Expression{Pair{
                std::make_shared<Expression>(Symbol{"begin"}),
                std::make_shared<Expression>(parseResult.value())
            }};
            return eval(beginForm, env);
        }
    }

    return Null{};
}