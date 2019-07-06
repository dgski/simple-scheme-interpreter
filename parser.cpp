#include "parser.h"

bool isValidSymbol(char c)
{
    switch(c)
    {
        case '(': return false;
        case ')': return false;
        case ' ': return false;
        case '\n': return false;
        case '\'': return false;
    }

    if(isdigit(c))
        return false;

    return true;
}

bool isWhiteSpace(char c)
{
        switch(c)
    {
        case ' ': return true;
        case '\n': return true;
        case '\t': return true;
    }

    return false;
}

std::optional<Expression> boolParser(std::string_view& s)
{
    if(s.length() == 0)
    {
        return std::nullopt;
    }

    const char c = s.front();
    s.remove_prefix(1);

    if(c == 't')
    {
        return BooleanInstance{ true };
    }
    else if(c == 'f')
    {
        return BooleanInstance{ false };
    }

    return std::nullopt;    
}

std::optional<Expression> intParser(std::string_view& s, std::string& intStr)
{
    if(s.length() == 0)
    {
        return IntegerInstance{ atoi(intStr.c_str()) };
    }
    
    const char c = s.front();
    if(isdigit(c))
    {
        s.remove_prefix(1);
        intStr += c;
        return intParser(s, intStr);
    }
    else if(intStr == "-" && isWhiteSpace(c) || isValidSymbol(c))
    {
        return symbolParser(s, intStr);
    }

    return IntegerInstance{ atoi(intStr.c_str()) };
}

std::optional<Expression> symbolParser(std::string_view& s, std::string& res)
{
    if(s.length() == 0)
    {
        return SymbolInstance{ res };
    }
    
    const char c = s.front();
    if(isValidSymbol(c))
    {
        s.remove_prefix(1);
        res += c;
        return symbolParser(s,res);
    }
    
    return SymbolInstance{ res };
}

std::optional<Expression> listParser(std::string_view& s)
{   
    if(s.front() == ')')
    {
        s.remove_prefix(1);
        return NullInstance{};
    }

    const auto first = whitespaceParser(s, charParser);
    const auto rest = listParser(s);
    
    return PairInstance{
        std::make_shared<Expression>(first.value()),
        std::make_shared<Expression>(rest.value())
    };
}

std::optional<Expression> commentParser(std::string_view& s)
{
    if(s.empty())
    {
        return NullInstance{};
    }

    const char c = s.front();
    s.remove_prefix(1);

    if(c == '\n')
    {
        return whitespaceParser(s, charParser);
    }

    return commentParser(s);
}

std::optional<Expression> charParser(std::string_view& s)
{
    if(s.length() == 0)
    {
        return std::nullopt;
    }

    const char c = s.front();
    s.remove_prefix(1);

    if(c == '(')
    {
        return listParser(s);
    }
    else if(c == ')')
    {
        return NullInstance{};
    }
    else if(c == '#')
    {
        return boolParser(s);
    }
    else if(isdigit(c) || c == '-')
    {
        std::string intStr;
        intStr += c;
        return intParser(s, intStr);
    }
    else if(c == '\'')
    {        
        if(auto res = charParser(s); res.has_value())
        {
            return PairInstance{
                std::make_shared<Expression>(SymbolInstance{"quote"}),
                std::make_shared<Expression>(PairInstance{
                    std::make_shared<Expression>(res.value()),
                    std::make_shared<Expression>(NullInstance{})
                })
            };
        }

        return std::nullopt;
    }
    else if(c == ';')
    {
        return commentParser(s);
    }
    else
    {
        std::string symStr;
        symStr += c;
        return symbolParser(s, symStr);
    }
}

std::optional<Expression> whitespaceParser(std::string_view& s, ParserFunction next)
{
    if(s.length() == 0)
    {
        return std::nullopt;
    }

    const char c = s.front();
    if(isWhiteSpace(c))
    {
        s.remove_prefix(1);
        return whitespaceParser(s, next);
    }

    return next(s);
}

std::optional<Expression> parse(std::string& s)
{
    std::string_view sv{s};
    return whitespaceParser(sv, charParser);
}

std::optional<Expression> parse(std::string s)
{
    std::string_view sv{s};
    return whitespaceParser(sv, charParser);
}

std::optional<Expression> multiParser(std::string_view& s)
{   
    if(s.empty())
    {
        return NullInstance{};
    }

    const auto first = whitespaceParser(s, charParser);
    const auto rest = multiParser(s);

    if(first.has_value() && rest.has_value())
    {
        return PairInstance{
            std::make_shared<Expression>(first.value()),
            std::make_shared<Expression>(rest.value())
        };
    }

    return std::nullopt;
}

std::optional<Expression> multiParse(std::string& s)
{
    std::string_view sv{s};
    return multiParser(sv);
}