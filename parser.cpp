#include "parser.h"

optional<Expression> charParser(string_view s)
{
    if(s.length() == 0)
        return nullopt;

    char c = s.front();
    s.remove_prefix(1);

    if(c == '(')
    {
        return whitespaceParser(s,charParser);
    }
    if(c == ')')
    {
        return Expression{list<Expression>{}};
    }
    else
    {
        string current;
        current += c;
        auto currExpression = Expression{current};

        auto next = whitespaceParser(s, charParser);
        if(next.has_value())
        {
            if(holds_alternative<list<Expression>>(next.value().data))
            {
                auto rest = get<list<Expression>>(next.value().data);
                list<Expression> l{currExpression};
                l.splice(l.end(),rest);
                return Expression{l};
            }
        }
        else
        {
            return currExpression;
        }
    }
}

optional<Expression> whitespaceParser(string_view s, ParserFunction next)
{
    if(s.length() == 0)
        return nullopt;

    char c = s.front();
    if(c == ' ' || c == '\n')
    {
        s.remove_prefix(1);
        return whitespaceParser(s, next);
    }
    else
    {
        return next(s);
    }
}

optional<Expression> parse(string s)
{
    return whitespaceParser(s, charParser);
}