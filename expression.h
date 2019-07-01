#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

struct Null{};
using Symbol = std::string;
using Integer = long long;
using Boolean = bool;
struct Pair;
struct Closure;

using Expression = std::variant<Null, Symbol, Integer, Boolean, Pair, Closure>;

struct List;
struct Closure : std::function<Expression(List&)> {};

struct Pair
{
    std::shared_ptr<Expression> first, second;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);
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
    std::ostream& operator()(Closure f)
    {
        os << "<Closure>";
        return os;
    }
};
// end types