#pragma once

#include <variant>
#include <string>
#include <map>
#include <iostream>
#include <functional>
#include <memory>

struct Null{};
using Symbol = std::string;
using Integer = int;
using Boolean = bool;
struct Pair;
struct Function;

using Expression = std::variant<Null, Symbol, Integer, Boolean, Pair, Function>;

std::ostream& operator<<(std::ostream& os, const Expression& e);

struct Function : std::function<Expression(Expression)> {};

struct Pair
{
    std::shared_ptr<Expression> first;
    std::shared_ptr<Expression> second;
};

class Environment : public std::map<Symbol,Expression>
{
    friend std::ostream& operator<< (std::ostream& os, const Environment& env)
    {
        os << '{';
        for(const auto&[k,v] : env)
        {
            os << k << ':' << v << ", ";
        }
        os << '}';

        return os;
    }
};

Expression eval(Expression exp, Environment& env);
Expression apply(Symbol funcName, Expression args, Environment& env);

void insertArgsIntoEnvironment(Pair& names, Pair& args, Environment& env);

Expression evalAllArgsInList(Pair& p, Environment& e);

struct Evaluator
{
    Environment& env;

    Evaluator(Environment& _env) : env(_env) {}

    Expression operator()(Null n)
    {
        return n;
    }
    Expression operator()(Symbol s)
    {
        return env[s];
    }
    Expression operator()(Integer i)
    {
        return i;
    }
    Expression operator()(Boolean b)
    {
        return b;
    }
    Expression operator()(Pair p)
    {   
        if(std::holds_alternative<Symbol>(*p.first) && (std::get<Symbol>(*p.first) == "lambda"))
        {   
            auto second = *p.second;
            auto rest = std::get<Pair>(second);
            auto lambdaBody = *std::get<Pair>(*(rest.second)).first;
            auto newEnv = env;
            auto names = std::get<Pair>(*(rest.first));

            return Function{[newEnv, lambdaBody, names](Expression args) mutable
            {
                insertArgsIntoEnvironment(names, std::get<Pair>(args), newEnv);
                return eval(lambdaBody, newEnv);
            }};
        }

        if(std::holds_alternative<Symbol>(*p.first))
        {
            return apply(std::get<Symbol>(*p.first), *p.second, env);
        }
        
        auto first = eval(*p.first, env);
      
        if(std::holds_alternative<Function>(first))
        {
            auto args = evalAllArgsInList(std::get<Pair>(*p.second), env);
            return std::get<Function>(first)(evalAllArgsInList);
        }

        return Pair{std::make_shared<Expression>(first),std::make_shared<Expression>(*p.second)};
    }
    Expression operator()(Function f)
    {
        return f;
    }
};

struct ExpressionStream
{
    std::ostream& os;

    ExpressionStream(std::ostream& _os) : os(_os) {}

    std::ostream& operator()(Null n)
    {   
        os << "null";
        return os;
    }
    std::ostream& operator()(Symbol s)
    {
        os << s;
        return os;
    }
    std::ostream& operator()(Integer i)
    {
        os << i;
        return os;
    }
    std::ostream& operator()(Boolean b)
    {
        os << (b ? "#t" : "#f");
        return os;
    }
    std::ostream& operator()(Pair p)
    {
        os << "(" << *p.first << " " << *p.second << ")";
        return os;
    }
    std::ostream& operator()(Function f)
    {
        os << "<Function>";
        return os;
    }
};