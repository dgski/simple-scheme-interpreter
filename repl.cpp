#include "parser.h"
#include "interpreter.h"

#include <fstream>
#include <sstream>

void parseAndEvalFile(const char* fileName, Environments& env)
{
    std::ifstream f{fileName};
    if(f.is_open())
    {
        std::stringstream ss;
        ss << f.rdbuf();

        std::string s{ ss.str() };
        if(auto parseResult = multiParse(s); parseResult.has_value())
        {
            auto beginForm = Expression{Pair{
                std::make_shared<Expression>(Symbol{"begin"}),
                std::make_shared<Expression>(parseResult.value())
            }};
            eval(beginForm, env);
        }
    }
}

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
            std::cout << "Error With Expression" << std::endl;
        }
    }

}