#include "parser.h"
#include "interpreter.h"

#include "templib.h"

#include <fstream>

int main()
{
    string line;
    Environments env{};
    env.add();
    eval(parse(add).value(), env);
    eval(parse(mapFunc).value(), env);
    eval(parse(addTwo).value(), env);
    eval(parse(filterFunc).value(), env);
    eval(parse(rangeListFunc).value(), env);
    while(true)
    {
        std::cout << "SSI>";
        getline(cin,line);
        if(auto parseResult = parse(line); parseResult.has_value())
        {
            auto res = eval(parseResult.value(), env);
            std::cout << res << std::endl;
        }
        else
        {
            cout << "Error With Expression" << endl;
        }
    }

}