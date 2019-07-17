#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

// types
struct Null{};
using Symbol = std::string;
struct String : std::string {};
using Integer = long long;
using Boolean = bool;
struct Pair;
struct Closure;

using Expression = std::variant<Null, Symbol, String, Integer, Boolean, Pair, Closure>;

struct List;
struct Closure : std::function<Expression(const List&)> {};

struct Pair
{
    std::shared_ptr<const Expression> first, second;
};
// end types

std::ostream& operator<<(std::ostream& os, const Expression& e);
class ExpressionStream
{
    std::ostream& os;
public:
    ExpressionStream(std::ostream& _os) : os(_os) {}

    std::ostream& operator()(const Null& n)
    {   
        os << "null";
        return os;
    }
    std::ostream& operator()(const Symbol& s)
    {
        os << s;
        return os;
    }
    std::ostream& operator()(const String& s)
    {
        os << '"' << s << '"';
        return os;
    }
    std::ostream& operator()(const Integer& i)
    {
        os << i;
        return os;
    }
    std::ostream& operator()(const Boolean& b)
    {
        os << (b ? "#t" : "#f");
        return os;
    }
    std::ostream& operator()(const Pair& p)
    {
        os << "(" << *p.first << " " << *p.second << ")";
        return os;
    }
    std::ostream& operator()(const Closure& f)
    {
        os << "<Closure>";
        return os;
    }
};