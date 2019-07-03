#include "type_checker.h"

Type createType(Symbol& typeName, List& args, Environments& env)
{   
    if(typeName == "Int")
    {
        std::cout << "creating int type" << std::endl;
        Type integer;
        integer.validator = [](Type& type, List& args)
        {
            std::cout << "Int Validator" << std::endl;
            return std::holds_alternative<Integer>(args.at(0));
        };
        return integer;
    }
    if(typeName == "Pair")
    {
        std::cout << "creating pair type" << std::endl;
        Type p;

        p.typeArgs.push_back(std::make_shared<Type>(std::visit(GetType{env}, args.at(0))));
        p.typeArgs.push_back(std::make_shared<Type>(std::visit(GetType{env}, args.at(1))));        

        p.validator = [](Type& type, List& args)
        {
            auto first = args.at(0);
            auto second = args.at(1);

            std::cout << "args:" << first << std::endl;
            std::cout << "args:" << second << std::endl;

            List firstAdaptor{ first };
            List secondAdaptor{ second };

            return type.typeArgs.at(0)->validate(firstAdaptor) && type.typeArgs.at(0)->validate(secondAdaptor);
        };
        return p;
    }

    if(typeName == "Func")
    {
        Type func;

        std::cout << args.all() << std::endl;
        
        int i = 0;
        Expression* last = &args.last();
        while(&args.at(i) != last)
        {
            func.typeArgs.push_back(std::make_shared<Type>(std::visit(GetType{env}, args.at(i))));
            i++;
        }

        func.validator = [](Type& type, List& args)
        {
            auto argNames = args.at(0);
            auto lambdaBody = args.at(1);
            
            auto end = type.typeArgs.end();
            /*
            auto begin = type.typeArgs.begin();
            for(auto i = begin; i != end - 1; ++i)
            {
                
            }
            */

            List adaptor { lambdaBody };
            bool returnTypeValid = (*end)->validate(adaptor);

            std::cout << "argNames:" << argNames << std::endl;
            std::cout << "lambdaBody:" << lambdaBody << std::endl;
        
            return false;
        };
        return func;
    }   
}

bool type_check(Expression& exp, Environments& env)
{
    return std::visit(TypeChecker{env}, exp);

    return false;
}