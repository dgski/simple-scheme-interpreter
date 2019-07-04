#include "expression.h"
#include "utils.h"
#include <cassert>
#include <vector>

// (((Func (Int) Int) (x) x) 1)
// ((Func (Int) Int) (x) x)
// (Func (Int) Int)

/*
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
*/

// Prototype -> Type -> Instance
//
// Int -> <Int> -> (<Int> 1)
//
// Pair -> <Pair <Int> <Int>> -> (<Pair <Int> <Int>> 1 2)
//
// Func -> <Func <<Int>> <Int>> -> (<Func <<Int>> <Int>> (x) x)

struct Type;
using DefinedTypes = std::map<Symbol, Type>; 


struct Instance;

using BuildInstanceFunc = std::function<Instance(std::vector<Instance> args, DefinedTypes& df)>;
using ValidateInstanceFunc = std::function<bool(Instance& inst, DefinedTypes& df)>;

struct Instance
{
    std::string value;
};

struct Type
{
    std::string tag;
    BuildInstanceFunc to_instance;
};


using BuildTypeFunc = std::function<Type(List& args, DefinedTypes& df)>;

struct Prototype
{
    BuildTypeFunc to_type;
};


using Elem = std::variant<Prototype, Type, Instance>;


Prototype integer();
Prototype pair();
Prototype func();


Elem get_elem(Expression& exp, DefinedTypes& df);



struct GetElem
{   
    DefinedTypes df;

    GetElem(DefinedTypes& _df) : df(_df) {}

    Elem operator()(Null& n) { return Type{}; }
    Elem operator()(Symbol& s);
    Elem operator()(Integer& i) { return Instance{"integer"}; }
    Elem operator()(Boolean& b) { return Type{}; }
    Elem operator()(Pair& p)
    {
        auto elem = get_elem(*p.first, df);
        if(std::holds_alternative<Prototype>(elem))
        {
            // Build Type
            List args{ *p.second };
            return std::get<Prototype>(elem).to_type(args, df);
        }
        else if(std::holds_alternative<Type>(elem))
        {
            // Build Instance
            List args{ *p.second };
            std::vector<Instance> inst;
            for(auto& a : args)
                inst.push_back(std::get<Instance>(get_elem(a, df)));
            
            return std::get<Type>(elem).to_instance(inst, df);
        }
        else if(std::holds_alternative<Instance>(elem))
        {
            // Validate Function Call
            // Return Return Type
            throw std::runtime_error("Only Function Instance is callable");
            return std::get<Instance>(elem);
        }
    }
    Elem operator()(Closure& f) { return Type{}; }
};

/*
struct TypeChecker
{

    bool operator()(Null& n) { return true; }
    bool operator()(Symbol& s)
    {
        return true;
    }
    bool operator()(Integer& i) { return true; }
    bool operator()(Boolean& b) { return true; }
    bool operator()(Pair& p)
    {



        return type_check(*p.second);
    }
    bool operator()(Closure& f) { return false; }
};
*/
