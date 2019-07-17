#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>
#include <fstream>
#include <sstream>

#include "expression.h"
#include "parser.h"
#include "interpreter.h"

class list_adaptor_iterator
{
public:
    const Expression* data;
    const Expression* next;
    using iterator_category = std::forward_iterator_tag;
    using value_type = const Expression;
    using difference_type = size_t;
    using pointer = const Expression*;
    using reference = const Expression&;

    list_adaptor_iterator(){}
    list_adaptor_iterator(pointer _data, pointer _next) : data(_data), next(_next) {}

    reference operator*() { return *data; }
    bool operator!=(const list_adaptor_iterator& other)
    {
        return data != other.data;
    }
    bool operator==(const list_adaptor_iterator& other)
    {
        return data == other.data;
    }
    list_adaptor_iterator& operator++()
    {
        if(std::holds_alternative<Null>(*next))
        {
            data = nullptr;
            next = nullptr;
        }
        else
        {
            const auto& p = std::get<Pair>(*next);
            data = p.first.get();
            next = p.second.get();
        }

        return *this;
    }
    list_adaptor_iterator operator++(int)
    {
        if(std::holds_alternative<Null>(*next))
        {
            return list_adaptor_iterator(nullptr, nullptr);
        }
        else
        {
            const auto& p = std::get<Pair>(*next);
            return list_adaptor_iterator(p.first.get(), p.second.get());
        }
    }
};

class List
{
    const Expression& source;

public:
    List(const Expression& _source) : source(_source) {}

    const Expression& at(int index) const
    {
        const Pair* ref = &std::get<Pair>(source);
        while(index != 0)
        {
            ref = &std::get<Pair>(*ref->second);
            index -= 1;
        }
        
        return *ref->first;
    }

    const Expression& all() const
    {
        return source;
    }

    const Expression& last() const
    {
        const Pair* ref = &std::get<Pair>(source);
        while(!std::holds_alternative<Null>(*ref->second))
        {
            ref = &std::get<Pair>(*ref->second);
        }
        
        return *ref->first;
    }

    list_adaptor_iterator begin() const
    {
        const auto& p = std::get<Pair>(source);
        return list_adaptor_iterator(p.first.get(), p.second.get());
    }

    list_adaptor_iterator end() const
    {
        return list_adaptor_iterator(nullptr, nullptr);
    }
};

// environments
struct Environment : public std::map<Symbol,Expression>
{
    bool contains(const Symbol& s) const
    {
        const auto it = find(s);
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

    const Expression& get(const Symbol& s) const
    {
        auto it = rbegin();
        while(it != rend())
        {
            if((*it)->contains(s))
                return (*(*it))[s];
            ++it;
        }

        throw std::runtime_error("Symbol is not bound to value");
    }

    void set(const Symbol s, const Expression& value)
    {
        back()->insert({s, value});
    }
    void set(const Symbol s, const Expression&& value)
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