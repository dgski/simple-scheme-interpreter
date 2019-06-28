#include <iostream>
#include "parser.h"

int main()
{
    constexpr const char* sampleExpression = R"(
        (+ 1 ( 1 ))
    )";

    std::optional<Expression> res = parse(sampleExpression);
    if(res.has_value())
    {
        std::cout << res.value() << std::endl;
    }
    else
    {
        std::cout << "Parsing Expression Failed" << std::endl;
    }
    
    return 0;
}