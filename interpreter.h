#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include "expression.h"

// environments
struct Environment : public std::map<Symbol,Expression>
{
    bool contains(const Symbol& s) const
    {
        auto it = find(s);
        return it != end();
    }

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

struct Environments : public std::vector<std::shared_ptr<Environment>>
{
    void add()
    {
        emplace_back(std::make_shared<Environment>());
    }

    Expression get(const Symbol& s) const
    {
        auto it = rbegin();
        while(it != rend())
        {
            if((*it)->contains(s))
                return (*(*it))[s];
            ++it;
        }

        return Null{};
    }

    void set(const Symbol s, Expression value)
    {
        back()->insert({s, value});
    }

    friend std::ostream& operator<<(std::ostream& os, const Environments& env)
    {
        os << '{';
        for(const auto& e : env)
        {
            os << *e;
        }
        os << '}';

        return os;
    }
};
// end environments

// evaluation
struct Arguments
{
    Pair& listOfArgs;
    Arguments(Expression& _listOfArgs) : listOfArgs(std::get<Pair>(_listOfArgs)) {}

    Expression& at(int index) const
    {
        Pair* ref = &listOfArgs;
        while(index != 0)
        {
            ref = &std::get<Pair>(*ref->second);
            index -= 1;
        }
        
        return *ref->first;
    }

    Pair& all() const
    {
        return listOfArgs;
    }
};

Expression eval(Expression& exp, Environments& env);
std::optional<Expression> applySpecialForms(Symbol& formName, Arguments& args, Environments& env);
std::optional<Expression> getPrimitiveFunction(Symbol& s);

struct Evaluator
{
    Environments& env;

    Evaluator(Environments& _env) : env(_env) {}

    Expression operator()(Null& n)
    {
        return n;
    }
    Expression operator()(Symbol& s);
    Expression operator()(Integer& i)
    {
        return i;
    }
    Expression operator()(Boolean& b)
    {
        return b;
    }
    Expression operator()(Pair& p);
    Expression operator()(Function& f)
    {
        return f;
    }
};
// end evalualtion

// helper
void insertArgsIntoEnvironment(Pair& names, Pair& args, Environment& env);
Expression evalAllArgsInList(Pair& p, Environments& e);
// end helper