#include "type_checker.h"

std::shared_ptr<Type> get_type(Expression& e)
{
    std::cout << e << std::endl;
    return std::visit(GetType{}, e);
}

std::optional<Prototype> getProtoType(Symbol& s)
{
    if(s == "Int")
    {
        return [](std::vector<std::shared_ptr<Type>> args)
        {
            return std::make_shared<IntegerType>();
        };
    }
    if(s == "Null")
    {
        return [](std::vector<std::shared_ptr<Type>> args)
        {
            return std::make_shared<NullType>();
        };
    }
    if(s == "Boolean")
    {
        return [](std::vector<std::shared_ptr<Type>> args)
        {
            return std::make_shared<BooleanType>();
        };
    }
    if(s == "Pair")
    {
        return [](std::vector<std::shared_ptr<Type>> args)
        {
            return std::make_shared<PairType>(
                args.at(0),
                args.at(1)
            );
        };
    }
    if(s == "Func")
    {
        return [](std::vector<std::shared_ptr<Type>> args)
        {
            auto retType = args.back();
            args.pop_back();

            return std::make_shared<FuncType>(args, retType);
        };
    }

    return std::nullopt;
}