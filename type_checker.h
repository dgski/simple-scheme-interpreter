#include "expression.h"
#include "utils.h"
#include <cassert>
#include <vector>
#include <algorithm>

// (((Func (Int) Int) (x) x) 1)
// ((Func (Int) Int) (x) x)
// (Func (Int) Int)


struct Type;
using TypePtr = std::shared_ptr<Type>;
using TypePtrVector = std::vector<TypePtr>;

struct Type
{
    virtual bool validate_args(TypePtrVector args) = 0;
    virtual std::string str() = 0;
};

TypePtr get_type(Expression& e);


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
        return
            typeArg0->validate_args({args.at(0)}) &&
            typeArg1->validate_args({args.at(1)});
    }
    virtual std::string str()
    {
        return "PairType: " + typeArg0->str() + " " + typeArg0->str();
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

    bool validate_body(List& args)
    {
        auto argNames = args.at(0);
        auto body = args.at(1);
        std::cout << "validateBody" << std::endl;
        std::cout << "argNames" <<  argNames << std::endl;
        std::cout << "body" << body << std::endl;
        return true;
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
            std::cout << "Checking if types are equivalent" << std::endl;
            std::cout << "Want:" << a->str() << std::endl;
            std::cout << "Got:" << b->str() << std::endl;
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
        res += ") ->" + retType->str();
        return res;
    }
};


using Prototype = std::function<TypePtr(TypePtrVector)>;
std::optional<Prototype> getProtoType(Symbol& s);

struct GetType
{   
    TypePtr operator()(Null& n) { return std::make_shared<NullType>(NullType{}); }
    TypePtr operator()(Symbol& s)
    {
        // (Func (Int) (Int) (Int))
        if(s == "+")
        {
            TypePtrVector args{ std::make_shared<IntegerType>(),std::make_shared<IntegerType>() };
            auto retType = std::make_shared<IntegerType>();
            return std::make_shared<CompleteFuncType>(args, retType);
        }


        return std::make_shared<SymbolType>(SymbolType{});
    }
    TypePtr operator()(Integer& i) { return std::make_shared<IntegerType>(IntegerType{}); }
    TypePtr operator()(Boolean& b) { return std::make_shared<BooleanType>(BooleanType{}); }
    TypePtr operator()(Pair& p);
    TypePtr operator()(Closure& f)
    {
        std::cout << "got closure" << std::endl;
        return std::make_shared<IntegerType>(IntegerType{});
    }
};

// (((Func (Boolean) (Boolean)) (x) x) #t)