#include <string>
#include <variant>
#include <optional>
#include <list>
#include <iostream>

using namespace std;

struct Expression
{
    variant<string,list<Expression>> data;

    friend ostream& operator<<(ostream& os, const Expression& e)
    {
        if(holds_alternative<string>(e.data))
        {
            os << get<string>(e.data);
        }
        else if(holds_alternative<list<Expression>>(e.data))
        {
            os << '(';
            for(const Expression& expression : get<list<Expression>>(e.data))
                os << expression << ' ';
            os << ')';
        }

        return os;
    }
};

using ParserFunction = optional<Expression>(string_view s);

optional<Expression> whitespaceParser(string_view s, ParserFunction next);
optional<Expression> charParser(string_view s);


optional<Expression> parse(string str);