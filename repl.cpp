#include "parser.h"
#include "interpreter.h"

#include "templib.h"

#include <fstream>
#include <sstream>

int main()
{
    string line;
    Environments env{};
    env.add();
    
    ifstream f{"stdlib/basics.scm"};
    if(f.is_open())
    {
        stringstream ss;
        ss << f.rdbuf();

        string s{ ss.str() };
        if(auto parseResult = multiParse(s); parseResult.has_value())
        {
            auto beginForm = Pair{
                std::make_shared<Expression>(Symbol{"begin"}),
                std::make_shared<Expression>(parseResult.value())
            };
            eval(beginForm, env);
        }
    }

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