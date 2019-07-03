#include "parser.h"
#include "type_checker.h"

#include <fstream>
#include <sstream>

int main()
{
    std::string line;
    Environments env{};
    env.add();

    while(true)
    {
        std::cout << "SSI>";
        std::getline(std::cin,line);
        if(auto parseResult = parse(line); parseResult.has_value())
        {
            const Boolean res = type_check(parseResult.value(), env);
            std::cout << res << std::endl;
        }
        else
        {
            std::cout << "Error With Expression" << std::endl;
        }
    }

}