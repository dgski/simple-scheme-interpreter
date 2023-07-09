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
struct TailCall;

using Expression = std::variant<Null, Symbol, String, Integer, Boolean, Pair, Closure, TailCall>;

struct List;
struct Closure : std::function<Expression(const List&)> {};
struct TailCall {
    std::shared_ptr<Expression> args;
};

struct Pair
{
    std::shared_ptr<const Expression> first, second;

    static std::vector<std::shared_ptr<const Expression>> garbage;
    ~Pair();
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
        os << "(" << *p.first << " ";

        if (!std::holds_alternative<Pair>(*p.second)) {
            os << *p.second << ")";
            return os;
        }

        auto next = std::get<Pair>(*p.second);

        while (std::holds_alternative<Pair>(*next.second)) {
            os << *next.first << " ";
            next = std::get<Pair>(*next.second);
        }

        os << *next.second << ")";

        return os;
    }
    std::ostream& operator()(const Closure& f)
    {
        os << "<Closure>";
        return os;
    }
    std::ostream& operator()(const TailCall& t)
    {
        os << "<TailCall " << *t.args << ">";
        return os;
    }
};