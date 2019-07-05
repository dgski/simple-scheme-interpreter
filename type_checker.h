#include "expression.h"
#include "utils.h"
#include <cassert>
#include <vector>
#include <algorithm>

// (((Func (Int) Int) (x) x) 1)
// ((Func (Int) Int) (x) x)
// (Func (Int) Int)

struct Type
{
    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args) = 0;
};

std::shared_ptr<Type> get_type(Expression& e);


struct NullType : Type
{
    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {
        return dynamic_cast<NullType*>(args.at(0).get());
    }
};

struct IntegerType : Type
{
    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {
        return dynamic_cast<IntegerType*>(args.at(0).get());
    }
};

struct SymbolType : Type
{
    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {
        return dynamic_cast<SymbolType*>(args.at(0).get());
    }
};

struct BooleanType : Type
{
    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {
        return dynamic_cast<BooleanType*>(args.at(0).get());
    }
};

struct PairType : Type
{
    std::shared_ptr<Type> typeArg0;
    std::shared_ptr<Type> typeArg1;

    PairType(std::shared_ptr<Type> _typeArg0, std::shared_ptr<Type> _typeArg1)
    : typeArg0(_typeArg0),
    typeArg1(_typeArg1)
    {}

    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {
        return typeArg0->validate_args({args.at(0)}) && typeArg1->validate_args({args.at(1)});
    }
};

struct FuncType : Type
{
    std::vector<std::shared_ptr<Type>> argTypes;
    std::shared_ptr<Type> retType;
    
    FuncType(std::vector<std::shared_ptr<Type>>& _argTypes, std::shared_ptr<Type> _retType)
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

    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {
        return true;
    }
};

struct CompleteFuncType : Type
{
    std::vector<std::shared_ptr<Type>> argTypes;
    std::shared_ptr<Type> retType;

    CompleteFuncType(std::vector<std::shared_ptr<Type>>& _argTypes, std::shared_ptr<Type> _retType)
    : argTypes(_argTypes)
    {
        retType = _retType;
    }

    virtual bool validate_args(std::vector<std::shared_ptr<Type>> args)
    {   
        return std::equal(argTypes.begin(), argTypes.end(), args.begin(),[](auto a, auto b)
        {
            std::cout << "Checking if types are equivalent" << std::endl;
            return false;
        });
    }
};


using Prototype = std::function<std::shared_ptr<Type>(std::vector<std::shared_ptr<Type>>)>;
std::optional<Prototype> getProtoType(Symbol& s);

struct GetType
{   
    std::shared_ptr<Type> operator()(Null& n) { return std::make_shared<NullType>(NullType{}); }
    std::shared_ptr<Type> operator()(Symbol& s)
    {
        return std::make_shared<SymbolType>(SymbolType{});
    }
    std::shared_ptr<Type> operator()(Integer& i) { return std::make_shared<IntegerType>(IntegerType{}); }
    std::shared_ptr<Type> operator()(Boolean& b) { return std::make_shared<BooleanType>(BooleanType{}); }
    std::shared_ptr<Type> operator()(Pair& p)
    {
        if(std::holds_alternative<Symbol>(*p.first))
        {
            if(auto proto = getProtoType(std::get<Symbol>(*p.first)); proto.has_value())
            {
                std::vector<std::shared_ptr<Type>> typeArgs;
                List args{ *p.second };
                for(auto& a : args)
                {
                    std::cout << a << std::endl;
                    typeArgs.push_back(get_type(a));
                }
                std::cout << "creating Type" << std::endl;
                return proto.value()(typeArgs);
            }
        }
        if(std::holds_alternative<Pair>(*p.first))
        {
            auto type = get_type(*p.first);

            if(auto funcType = dynamic_cast<CompleteFuncType*>(type.get()); funcType)
            {
                std::vector<std::shared_ptr<Type>> typeArgs;
                List args{ *p.second };
                for(auto& a : args)
                {
                    typeArgs.push_back(get_type(a));
                }

                if(!funcType->validate_args(typeArgs))
                {
                    throw std::runtime_error("Incorrenct Arguments for Function");
                }

                return funcType->retType;
            }

            if(auto funcType = dynamic_cast<FuncType*>(type.get()); funcType)
            {
                List args{ *p.second };
                funcType->validate_body(args);
                return std::make_shared<CompleteFuncType>(funcType->argTypes, funcType->retType);
            }

            std::vector<std::shared_ptr<Type>> typeArgs;
            List args{ *p.second };
            for(auto& a : args)
            {
                typeArgs.push_back(get_type(a));
            }
                
            std::cout << "validating arg Types" << std::endl;
            if(!type->validate_args(typeArgs))
            {
                throw std::runtime_error("Wrong Type Args");
            }
        }

        return std::make_shared<IntegerType>(IntegerType{});
    }
    std::shared_ptr<Type> operator()(Closure& f) { return std::make_shared<IntegerType>(IntegerType{}); }
};
