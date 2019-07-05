#include "type_checker.h"

TypePtr get_type(Expression& e)
{
    std::cout << e << std::endl;
    return std::visit(GetType{}, e);
}

std::optional<Prototype> getProtoType(Symbol& s)
{
    if(s == "Int")
    {
        return [](TypePtrVector args)
        {
            return std::make_shared<IntegerType>();
        };
    }
    if(s == "Null")
    {
        return [](TypePtrVector args)
        {
            return std::make_shared<NullType>();
        };
    }
    if(s == "Boolean")
    {
        return [](TypePtrVector args)
        {
            return std::make_shared<BooleanType>();
        };
    }
    if(s == "Pair")
    {
        return [](TypePtrVector args)
        {
            return std::make_shared<PairType>(
                args.at(0),
                args.at(1)
            );
        };
    }
    if(s == "Func")
    {
        return [](TypePtrVector args)
        {
            auto retType = args.back();
            args.pop_back();

            return std::make_shared<FuncType>(args, retType);
        };
    }

    return std::nullopt;
}

TypePtr GetType::operator()(Pair& p)
    {
        if(std::holds_alternative<Symbol>(*p.first))
        {
            if(auto proto = getProtoType(std::get<Symbol>(*p.first)); proto.has_value())
            {
                TypePtrVector typeArgs;
                List args{ *p.second };
                for(auto& a : args)
                {
                    typeArgs.push_back(get_type(a));
                }
                std::cout << "creating Type" << std::endl;
                return proto.value()(typeArgs);
            }
        }
        if(std::holds_alternative<Pair>(*p.first))
        {
            auto type = get_type(*p.first);

            if(auto completeFuncType = dynamic_cast<CompleteFuncType*>(type.get()); completeFuncType)
            {
                TypePtrVector typeArgs;
                List args{ *p.second };
                for(auto& a : args)
                {
                    typeArgs.push_back(get_type(a));
                }

                if(!completeFuncType->validate_args(typeArgs))
                {
                    throw std::runtime_error("Incorrenct Arguments for Function");
                }

                return completeFuncType->retType;
            }

            if(auto funcType = dynamic_cast<FuncType*>(type.get()); funcType)
            {
                List args{ *p.second };
                funcType->validate_body(args);
                return std::make_shared<CompleteFuncType>(funcType->argTypes, funcType->retType);
            }

            TypePtrVector typeArgs;
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

            return type;
        }

        return std::make_shared<IntegerType>(IntegerType{});
    }