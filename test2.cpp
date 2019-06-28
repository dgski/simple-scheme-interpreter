#include "expression.h"

int main()
{
    /*
    Expression exp = Pair{
        std::make_shared<Expression>("add1"),
        std::make_shared<Expression>("data")
    };

    Environment env;
    env["add1"] = Function{[](Expression exp)
    {
        Integer value = std::get<Integer>(exp);
        return Integer{value + 1};
    }};
    env["data"] = Integer{32};

    auto res = eval(exp, env);
    std::cout << std::get<Integer>(res) << std::endl;
    */


    // Lambda Test
    Expression identityFunction = Pair{
        std::make_shared<Expression>("lambda"),
        std::make_shared<Expression>(Pair{
            std::make_shared<Expression>(Pair{
                std::make_shared<Expression>("x"),
                std::make_shared<Expression>(Null{})
            }), // args
            std::make_shared<Expression>("x") // body
        })
    };
    //std::cout << identityFunction << std::endl;

    Expression exp2 = Pair{
        std::make_shared<Expression>(identityFunction),
        std::make_shared<Expression>(302)
    };
    //std::cout << exp2 << std::endl;

    Environment env{};
    auto res2 = eval(exp2, env);
    std::cout << res2 << std::endl;
}