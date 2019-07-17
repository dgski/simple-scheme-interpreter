#include "parser.h"

bool isValidSymbol(const char c)
{
    switch(c)
    {
        case '(': return false;
        case ')': return false;
        case '"': return false;
        case ' ': return false;
        case '\n': return false;
        case '\'': return false;
    }

    if(isdigit(c))
    {
        return false;
    }

    return true;
}

bool isWhiteSpace(const char c)
{
    switch(c)
    {
        case ' ': return true;
        case '\n': return true;
        case '\t': return true;
    }

    return false;
}

std::optional<const Expression> boolParser(std::string_view& s)
{
    if(s.length() == 0)
    {
        return std::nullopt;
    }

    const char c = s.front();
    
    if(c == 't')
    {
        s.remove_prefix(1);
        return Boolean{ true };
    }
    else if(c == 'f')
    {
        s.remove_prefix(1);
        return Boolean{ false };
    }

    std::string symbolStr;
    return symbolParser(s, symbolStr);  
}

std::optional<const Expression> intParser(std::string_view& s, std::string& intStr)
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
    else if(intStr == "-" && isWhiteSpace(c) || isValidSymbol(c))
    {
        return symbolParser(s, intStr);
    }

    return Integer{ atoi(intStr.c_str()) };
}

std::optional<const Expression> symbolParser(std::string_view& s, std::string& res)
{
    if(s.length() == 0)
    {
        return Symbol{ res };
    }
    
    const char c = s.front();

    if(isValidSymbol(c))
    {
        s.remove_prefix(1);
        res += c;
        return symbolParser(s,res);
    }
    
    return Symbol{ res };
}

std::optional<const Expression> listParser(std::string_view& s)
{   
    if(s.front() == ')')
    {
        s.remove_prefix(1);
        return Null{};
    }

    const auto& first = whitespaceParser(s, charParser);
    const auto& rest = listParser(s);
    
    return Pair{
        std::make_shared<Expression>(first.value()),
        std::make_shared<Expression>(rest.value())
    };
}

std::optional<const Expression> commentParser(std::string_view& s)
{
    if(s.empty())
    {
        return Null{};
    }

    const char c = s.front();
    s.remove_prefix(1);

    if(c == '\n')
    {
        return whitespaceParser(s, charParser);
    }

    return commentParser(s);
}

std::optional<const Expression> stringParser(std::string_view& s, std::string& res)
{
    if(s.length() == 0)
    {
        return std::nullopt;
    }
    
    const char c = s.front();
    s.remove_prefix(1);

    if(c == '"')
    {
        return String{ res };
    }

    res += c;
    return stringParser(s,res);
}

std::optional<const Expression> quoteParser(std::string_view& s)
{
    if(const auto& res = charParser(s); res.has_value())
    {
        return Pair{
            std::make_shared<Expression>(Symbol{ "quote" }),
            std::make_shared<Expression>(Pair{
                std::make_shared<Expression>(res.value()),
                std::make_shared<Expression>(Null{})
            })
        };
    }

    return std::nullopt;
}

std::optional<const Expression> charParser(std::string_view& s)
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
    if(c == '"')
    {
        std::string str;
        return stringParser(s, str);
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
        std::string intStr;
        intStr += c;
        return intParser(s, intStr);
    }
    else if(c == '\'')
    {
        return quoteParser(s);
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

std::optional<const Expression> whitespaceParser(std::string_view& s, ParserFunction next)
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

std::optional<const Expression> parse(std::string& s)
{
    std::string_view sv{s};
    return whitespaceParser(sv, charParser);
}

std::optional<const Expression> parse(std::string s)
{
    std::string_view sv{s};
    return whitespaceParser(sv, charParser);
}

std::optional<const Expression> multiParser(std::string_view& s)
{   
    if(s.empty())
    {
        return Null{};
    }

    const auto& first = whitespaceParser(s, charParser);
    const auto& rest = multiParser(s);

    if(first.has_value() && rest.has_value())
    {
        return Pair{
            std::make_shared<Expression>(first.value()),
            std::make_shared<Expression>(rest.value())
        };
    }

    return std::nullopt;
}

std::optional<const Expression> multiParse(std::string& s)
{
    std::string_view sv{s};
    return multiParser(sv);
}