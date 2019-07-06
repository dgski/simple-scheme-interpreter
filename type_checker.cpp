#include "type_checker.h"

auto IntPtrInstance = std::make_shared<IntegerType>();

TypePtr get_type(Expression& e, TypePtrMap& typeMap)
{
    return std::visit(GetType{typeMap}, e);
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
    if(s == "Bool")
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

TypePtr GetType::operator()(Symbol& s)
{
    // (Func (Int) (Int) (Int))
    if(s == "+")
    {
        TypePtrVector args{ IntPtrInstance, IntPtrInstance };
        auto retType = IntPtrInstance;
        return std::make_shared<CompleteFuncType>(args, retType);
    }

    // (Func (Int) (Int) (Int))
    if(s == "-")
    {
        TypePtrVector args{ IntPtrInstance, IntPtrInstance };
        auto retType = IntPtrInstance;
        return std::make_shared<CompleteFuncType>(args, retType);
    }

    auto it = typeMap.find(s);
    if(it != typeMap.end())
    {
        return (*it).second;
    }

    return std::make_shared<SymbolType>();
}

static TypePtrVector getTypesInList(List&& args, TypePtrMap& typeMap)
{
    TypePtrVector typeArgs;
    for(auto& a : args)
    {
        typeArgs.push_back(get_type(a, typeMap));
    }
    return typeArgs;
}

TypePtr GetType::operator()(Pair& p)
    {
        if(std::holds_alternative<Symbol>(*p.first))
        {
            if(auto proto = getProtoType(std::get<Symbol>(*p.first)); proto.has_value())
            {
                auto typeArgs = getTypesInList(List{ *p.second }, typeMap);
                return proto.value()(typeArgs);
            }
            
            auto type = get_type(*p.first, typeMap);
            if(auto completeFuncType = dynamic_cast<CompleteFuncType*>(type.get()); completeFuncType)
            {
                auto typeArgs = getTypesInList(List{ *p.second }, typeMap);
                if(!completeFuncType->validate_args(typeArgs))
                {
                    throw std::runtime_error("Incorrenct Arguments for Function");
                }

                return completeFuncType->retType;
            }
        }
        if(std::holds_alternative<Pair>(*p.first))
        {
            auto type = get_type(*p.first, typeMap);
            if(auto completeFuncType = dynamic_cast<CompleteFuncType*>(type.get()); completeFuncType)
            {
                auto typeArgs = getTypesInList(List{ *p.second }, typeMap);
                if(!completeFuncType->validate_args(typeArgs))
                {
                    throw std::runtime_error("Incorrenct Arguments for Function");
                }
                return completeFuncType->retType;
            }

            if(auto funcType = dynamic_cast<FuncType*>(type.get()); funcType)
            {
                List args{ *p.second };
                if(!funcType->validate_body(args, typeMap))
                {
                    throw std::runtime_error("Function returns Incorrect Type");
                }

                return std::make_shared<CompleteFuncType>(funcType->argTypes, funcType->retType);
            }

            auto typeArgs = getTypesInList(List{ *p.second }, typeMap);
            if(!type->validate_args(typeArgs))
            {
                throw std::runtime_error("Wrong Type Args");
            }

            return type;
        }

        return std::make_shared<IntegerType>(IntegerType{});
    }