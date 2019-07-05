#include "parser.h"
#include "type_checker.h"

#include <fstream>
#include <sstream>

int main()
{
    std::string line;


    while(true)
    {
        std::cout << "SSI>";
        std::getline(std::cin,line);
        if(auto parseResult = parse(line); parseResult.has_value())
        {
            try
            {
                auto res = get_type(parseResult.value());
                std::cout << res->str() << std::endl;
            }
            catch(std::runtime_error& e)
            {
                std::cout << "======================== Type Error ========================" << std::endl;
                std::cout << e.what() << std::endl;
            }
        }
        else
        {
            std::cout << "Error With Expression" << std::endl;
        }
    }
    

    Pair p{
        std::make_shared<Expression>(Integer{ 1 }),
        std::make_shared<Expression>(Null{})
    };

    Expression e{p};

    List args{ e };
    IntegerType it;
    //std::cout << it.validate_args(args) << std::endl;

}