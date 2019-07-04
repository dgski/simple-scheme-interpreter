#include "parser.h"
#include "type_checker.h"

#include <fstream>
#include <sstream>

int main()
{
    std::string line;
    DefinedTypes df;

    while(true)
    {
        std::cout << "SSI>";
        std::getline(std::cin,line);
        if(auto parseResult = parse(line); parseResult.has_value())
        {
            auto res = get_elem(parseResult.value(), df);
            std::cout << std::get<Instance>(res).value << std::endl;
        }
        else
        {
            std::cout << "Error With Expression" << std::endl;
        }
    }

}