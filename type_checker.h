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
};

TypePtr get_type(Expression& e);


struct NullType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<NullType*>(args.at(0).get());
    }
};

struct IntegerType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<IntegerType*>(args.at(0).get());
    }
};

struct SymbolType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<SymbolType*>(args.at(0).get());
    }
};

struct BooleanType : Type
{
    virtual bool validate_args(TypePtrVector args)
    {
        return dynamic_cast<BooleanType*>(args.at(0).get());
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

    virtual bool validate_args(TypePtrVector args)
    {
        return true;
    }
};

struct CompleteFuncType : Type
{
    TypePtrVector argTypes;
    TypePtr retType;

    CompleteFuncType(TypePtrVector& _argTypes, TypePtr _retType)
    : argTypes(_argTypes)
    {
        retType = _retType;
    }

    virtual bool validate_args(TypePtrVector args)
    {   
        return std::equal(argTypes.begin(), argTypes.end(), args.begin(),[](auto a, auto b)
        {
            std::cout << "Checking if types are equivalent" << std::endl;
            return false;
        });
    }
};


using Prototype = std::function<TypePtr(TypePtrVector)>;
std::optional<Prototype> getProtoType(Symbol& s);

struct GetType
{   
    TypePtr operator()(Null& n) { return std::make_shared<NullType>(NullType{}); }
    TypePtr operator()(Symbol& s)
    {
        return std::make_shared<SymbolType>(SymbolType{});
    }
    TypePtr operator()(Integer& i) { return std::make_shared<IntegerType>(IntegerType{}); }
    TypePtr operator()(Boolean& b) { return std::make_shared<BooleanType>(BooleanType{}); }
    TypePtr operator()(Pair& p);
    TypePtr operator()(Closure& f) { return std::make_shared<IntegerType>(IntegerType{}); }
};
