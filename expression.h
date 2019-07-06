#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

struct NullInstance{};
using SymbolInstance = std::string;
using IntegerInstance = long long;
using BooleanInstance = bool;
struct PairInstance;
struct Closure;

using Expression = std::variant<NullInstance, SymbolInstance, IntegerInstance, BooleanInstance, PairInstance, Closure>;

struct List;
struct Closure : std::function<Expression(List&)> {};

struct PairInstance
{
    std::shared_ptr<Expression> first, second;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);
struct ExpressionStream
{
    std::ostream& os;

    ExpressionStream(std::ostream& _os) : os(_os) {}

    std::ostream& operator()(NullInstance n)
    {   
        os << "null";
        return os;
    }
    std::ostream& operator()(SymbolInstance s)
    {
        os << s;
        return os;
    }
    std::ostream& operator()(IntegerInstance i)
    {
        os << i;
        return os;
    }
    std::ostream& operator()(BooleanInstance b)
    {
        os << (b ? "#t" : "#f");
        return os;
    }
    std::ostream& operator()(PairInstance p)
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