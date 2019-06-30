#include <string>
#include <variant>
#include <optional>
#include <list>
#include <iostream>
#include "expression.h"

using namespace std;

using ParserFunction = optional<Expression>(string_view& s);

bool isValidSymbolChar(char c);

optional<Expression> boolParser(string_view& s);
optional<Expression> intParser(string_view& s, string& intStr);
optional<Expression> symbolParser(string_view& s, string& res);
optional<Expression> charParser(string_view& s);
optional<Expression> whitespaceParser(string_view& s, ParserFunction next);

optional<Expression> parse(string str);