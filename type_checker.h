#pragma once

#include "expression.h"
#include "utils.h"
#include <cassert>
#include <vector>
#include <algorithm>

struct Type;
using TypePtr = std::shared_ptr<Type>;
using TypePtrVector = std::vector<TypePtr>;
using TypePtrMap = std::map<Symbol, TypePtr>;

struct Type
{
    virtual bool validate_args(TypePtrVector args) = 0;
    virtual std::string str() = 0;
};

TypePtr get_type(Expression& e, TypePtrMap& typeMap);

struct NullType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<NullType*>(args.at(0).get());
    }
    virtual std::string str()
    {
        return "NullType";
    }
};

struct IntegerType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<IntegerType*>(args.at(0).get());
    }
    virtual std::string str()
    {
        return "IntegerType";
    }
};

struct SymbolType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<SymbolType*>(args.at(0).get());
    }
    virtual std::string str()
    {
        return "SymbolType";
    }
};

struct BooleanType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<BooleanType*>(args.at(0).get());
    }
    virtual std::string str()
    {
        return "BooleanType";
    }
};

struct PairType : Type
{
    TypePtr typeArg0;
    TypePtr typeArg1;

    PairType(TypePtr _typeArg0, TypePtr _typeArg1)
    : typeArg0(_typeArg0),
    typeArg1(_typeArg1)
    {}

    virtual bool validate_args(TypePtrVector args)
    {
        return typeArg0->validate_args({args.at(0)}) && typeArg1->validate_args({args.at(1)});
    }
    virtual std::string str()
    {
        return "PairType: " + typeArg0->str() + " " + typeArg1->str();
    }
};

struct FuncType : Type
{
    TypePtrVector argTypes;
    TypePtr retType;
    
    FuncType(TypePtrVector& _argTypes, TypePtr _retType)
    : argTypes(_argTypes)
    {
        retType = _retType;
    }

    bool validate_body(List& args, TypePtrMap typeMap)
    {
        auto it = argTypes.begin();
        List argNames{ args.at(0) };
        for(auto& name : argNames)
        {
            if(it == argTypes.end())
            {
                return false;
            }
            typeMap[std::get<Symbol>(name)] = *it;
            ++it;
        }

        auto bodyType = get_type(args.at(1), typeMap);
        return retType->str() == bodyType->str();
    }
    virtual std::string str()
    {   
        std::string res = "FuncType (";
        for(auto& a : argTypes)
        {
            res += a->str();
        }
        res += ") ->" + retType->str();
        return res;
    }

    virtual bool validate_args(TypePtrVector args)
    {
        return true;
    }
};

struct CompleteFuncType : Type
{
    TypePtrVector argTypes;
    TypePtr retType;

    CompleteFuncType(TypePtrVector _argTypes, TypePtr _retType)
    : argTypes(_argTypes)
    {
        retType = _retType;
    }

    virtual bool validate_args(TypePtrVector args)
    {   
        return std::equal(argTypes.begin(), argTypes.end(), args.begin(),[](auto a, auto b)
        {
            return a->str() == b->str();
        });
    }
    virtual std::string str()
    {
        std::string res = "CompleteFuncType (";
        for(auto& a : argTypes)
        {
            res += a->str();
        }
        res += ") -> " + retType->str();
        return res;
    }
};

using Prototype = std::function<TypePtr(TypePtrVector)>;
std::optional<Prototype> getProtoType(Symbol& s);

struct GetType
{   
    TypePtrMap& typeMap;

    GetType(TypePtrMap& _typeMap) : typeMap(_typeMap) {}

    TypePtr operator()(Null& n) { return std::make_shared<NullType>(NullType{}); }
    TypePtr operator()(Symbol& s);
    TypePtr operator()(Integer& i) { return std::make_shared<IntegerType>(IntegerType{}); }
    TypePtr operator()(Boolean& b) { return std::make_shared<BooleanType>(BooleanType{}); }
    TypePtr operator()(Pair& p);
    TypePtr operator()(Closure& f) { return std::make_shared<IntegerType>(IntegerType{}); }
};