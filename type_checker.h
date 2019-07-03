#include "expression.h"
#include "utils.h"
#include <cassert>

// (((Func (Int) Int) (x) x) 1)
// ((Func (Int) Int) (x) x)
// (Func (Int) Int)

struct Type;
using ValidatorFunc =  std::function<bool(Type&, List&)>;

struct Type
{  
    std::vector<std::shared_ptr<Type>> typeArgs;
    ValidatorFunc validator;
    bool validate(List& args)
    {
        std::cout << "validate" << std::endl;
        std::cout << args.all() << std::endl;
        return validator(*this, args);
    }
};

bool type_check(Expression& exp, Environments& env);


Type createType(Symbol& typeName, List& args, Environments& env);

struct GetType
{
    Environments& env;

    GetType(Environments& _env) : env(_env) {}

    Type operator()(Null& n) { return Type{}; }
    Type operator()(Symbol& s)
    {
        Type gen;
        gen.validator = [](Type& type, List& args)
        {
            std::cout << "Symbol validator" << std::endl;
            return true;
        };
        return gen;
    }
    Type operator()(Integer& i) { return Type{}; }
    Type operator()(Boolean& b) { return Type{};; }
    Type operator()(Pair& p)
    {
        if(std::holds_alternative<Symbol>(*p.first))
        {
            List args{*p.second};
            return createType(std::get<Symbol>(*p.first),args,env);
        }
        else if(std::holds_alternative<Pair>(*p.first))
        {
            auto res = std::visit(GetType{env}, *p.second);
            return res;
        }
    }
    Type operator()(Closure& f) { return Type{}; }
};

struct TypeChecker
{
    Environments& env;

    TypeChecker(Environments& _env) : env(_env) {}

    bool operator()(Null& n) { return true; }
    bool operator()(Symbol& s)
    {
        if(s == "Func")
        {

        }
    }
    bool operator()(Integer& i) { return false; }
    bool operator()(Boolean& b) { return false; }
    bool operator()(Pair& p)
    {
        if(std::holds_alternative<Symbol>(*p.first))
        {
            List args{*p.second};
            auto res = createType(std::get<Symbol>(*p.first),args,env);
            return true;
        }
        else if(std::holds_alternative<Pair>(*p.first))
        {
            auto res = std::visit(GetType{env}, *p.first);
            List args{ *p.second };
            return res.validate(args);
        }

        return type_check(*p.second, env);
    }
    bool operator()(Closure& f) { return false; }
};
