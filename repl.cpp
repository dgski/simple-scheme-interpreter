#include "parser.h"
#include "interpreter.h"

int main()
{
    std::string line;
    Environments env{};
    env.add();

    parseAndEvalFile("stdlib/basics.scm", env);
    
    while(true)
    {
        std::cout << "SSI>";
        std::getline(std::cin,line);
        if(auto parseResult = parse(line); parseResult.has_value())
        {
            auto res = eval(parseResult.value(), env);
            std::cout << res << std::endl;
        }
        else
        {
            std::cout << "Error Parsing Expression" << std::endl;
        }
    }

}