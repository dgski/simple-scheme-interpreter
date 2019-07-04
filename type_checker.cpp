#include "type_checker.h"

Elem get_elem(Expression& exp, DefinedTypes& df)
{
    return std::visit(GetElem{df}, exp);
}

Type integerType()
{
    return Type{
        "<Int>",
        [](std::vector<Instance> args, DefinedTypes& df)
        {   
            if(args.at(0).value != "integer")
            {
                throw std::runtime_error("Needs Integer Argument");
            }
            return Instance{"integer"};
        }
    };
}
Prototype integerPrototype()
{
    return Prototype{
        [](List& args, DefinedTypes& df)
        {
            return integerType();
        }
    };
}

Type pairType(Elem type1, Elem type2)
{
    return Type{
        "<Pair>",
        [type1, type2](std::vector<Instance> args, DefinedTypes& df)
        {   
            if(args.size() == 1)
                return args.at(0);

            try
            {
                std::get<Type>(type1).to_instance({args.at(0)}, df);
            }
               catch(const std::exception& e)
            {   
                std::string msg = "=========== Pair Error ===========\n";
                msg += "Expecting: " + std::get<Type>(type1).tag + '\n';
                msg += "Got: " + args.at(0).value;

                throw std::runtime_error(msg);
            }

            try
            {
                std::get<Type>(type2).to_instance({args.at(1)}, df);
            }
            catch(const std::exception& e)
            {   
                std::string msg = "=========== Pair Error ===========\n";
                msg += "Expecting: " + std::get<Type>(type2).tag + '\n';
                msg += "Got: " + args.at(1).value;

                throw std::runtime_error(msg);
            }
            
            
            

            return Instance{"pair"};
        }
    };
}
Prototype pairPrototype()
{
    return Prototype{
    [](List& args, DefinedTypes& df)
    {
        auto type1 = get_elem(args.at(0), df);
        auto type2 = get_elem(args.at(1), df);
        if(!std::holds_alternative<Type>(type1) || !std::holds_alternative<Type>(type2))
        {
            throw std::runtime_error("Pair Needs Valid Type Arguments");
        }

        return pairType(type1, type2);
    }
    };
}


Prototype funcPrototype()
{
    return Prototype{
        [](List& args, DefinedTypes& df)
        {
            //auto argTypes = get_elem(args.at(0), df);
            //auto body = get_elem(args.at(1), df);

            std::cout << "Func argTypes: " << args.at(0) << std::endl;
            std::cout << "Func returnType: " << args.at(1) << std::endl;


            return Type{
                "<Function>",
                [](std::vector<Instance> args, DefinedTypes& df)
                {
                    //std::cout << "Func argNames:" << args.at(0) << std::endl;
                    //std::cout << "Func body:" << args.at(1) << std::endl;

                    return Instance{"function"};
                }
            };
        }
    };
}


Elem GetElem::operator()(Symbol& s)
{
    if(s == "Int")
    {
        return integerPrototype();
    }
    else if(s == "Pair")
    {
        return pairPrototype();
    }
    
    if(s == "Func")
    {
        return funcPrototype();
    }
    

    return Instance{"symbol"};
}

// ((Pair (Int) (Pair (Int) (Int))) 1 ((Pair (Int) (Int)) 1 2))