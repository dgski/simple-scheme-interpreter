#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

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
class List
{
    Expression& source;

public:
    List(Expression& _source) : source(_source) {}

    Expression& at(int index) const
    {
        if(index == 0 and !std::holds_alternative<Pair>(source))
        {
            return source;
        }

        Pair* ref = &std::get<Pair>(source);
        while(index != 0)
        {
            ref = &std::get<Pair>(*ref->second);
            index -= 1;
        }
        
        return *ref->first;
    }

    Expression& all() const
    {
        return source;
    }

    Expression& last() const
    {
        Pair* ref = &std::get<Pair>(source);
        while(!std::holds_alternative<Null>(*ref->second))
        {
            ref = &std::get<Pair>(*ref->second);
        }
        
        return *ref->first;
    }
};