#include "parser.h"
#include "interpreter.h"

int main()
{
    std::string line;
    Environments env{};
    env.add();

    parseAndEvalFile("stdlib/basics.scm", env);
    parseAndEvalFile("stdlib/list.scm", env);
    parseAndEvalFile("stdlib/math.scm", env);
    parseAndEvalFile("stdlib/string.scm", env);
    parseAndEvalFile("stdlib/utils.scm", env);
    
    while(true)
    {
        std::cout << "SSI>";
        std::getline(std::cin,line);
        if(const auto& parseResult = parse(line); parseResult.has_value())
        {
            const auto& res = eval(parseResult.value(), env);
            std::cout << res << std::endl;
        }
        else
        {
            std::cout << "Error Parsing Expression" << std::endl;
        }
    }
}