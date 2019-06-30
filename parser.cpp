#include "parser.h"

bool isValidSymbolChar(char c)
{
    switch(c)
    {
        case '(': return false;
        case ')': return false;
        case ' ': return false;
        case '\n': return false;
    }

    if(isdigit(c))
        return false;

    return true;
}

optional<Expression> boolParser(string_view& s)
{
    if(s.length() == 0)
    {
        return nullopt;
    }

    const char c = s.front();
    s.remove_prefix(1);

    if(c == 't')
    {
        return Boolean{ true };
    }
    else if(c == 'f')
    {
        return Boolean{ false };
    }

    return nullopt;    
}

optional<Expression> intParser(string_view& s, string& intStr)
{
    if(s.length() == 0)
    {
        return Integer{ atoi(intStr.c_str()) };
    }
    
    const char c = s.front();
    if(isdigit(c))
    {
        s.remove_prefix(1);
        intStr += c;
        return intParser(s, intStr);
    }

    return Integer{ atoi(intStr.c_str()) };
}

optional<Expression> symbolParser(string_view& s, string& res)
{
    if(s.length() == 0)
    {
        return Symbol{ res };
    }
    
    const char c = s.front();
    if(isValidSymbolChar(c))
    {
        s.remove_prefix(1);
        res += c;
        return symbolParser(s,res);
    }
    
    return Symbol{ res };
}

optional<Expression> listParser(string_view& s)
{   
    if(s.front() == ')')
    {
        s.remove_prefix(1);
        return Null{};
    }

    const auto first = whitespaceParser(s, charParser);
    const auto rest = listParser(s);
    
    return Pair{
        make_shared<Expression>(first.value()),
        make_shared<Expression>(rest.value())
    };
}

optional<Expression> charParser(string_view& s)
{
    if(s.length() == 0)
    {
        return nullopt;
    }

    const char c = s.front();
    s.remove_prefix(1);

    if(c == '(')
    {
        return listParser(s);
    }
    else if(c == ')')
    {
        return Null{};
    }
    else if(c == '#')
    {
        return boolParser(s);
    }
    else if(isdigit(c) || c == '-')
    {
        string intStr;
        intStr += c;
        return intParser(s, intStr);
    }
    else
    {
        string symStr;
        symStr += c;
        return symbolParser(s, symStr);
    }
}

optional<Expression> whitespaceParser(string_view& s, ParserFunction next)
{
    if(s.length() == 0)
    {
        return nullopt;
    }

    const char c = s.front();
    if(c == ' ' || c == '\n')
    {
        s.remove_prefix(1);
        return whitespaceParser(s, next);
    }

    return next(s);
}

optional<Expression> parse(string s)
{
    string_view sv{s};
    return whitespaceParser(sv, charParser);
}