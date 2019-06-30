#include <string>
#include <variant>
#include <optional>
#include <list>
#include <iostream>
#include "expression.h"

using ParserFunction = std::optional<Expression>(std::string_view& s);

bool isValidSymbol(char c);
bool isWhiteSpace(char c);

std::optional<Expression> boolParser(std::string_view& s);
std::optional<Expression> intParser(std::string_view& s, std::string& intStr);
std::optional<Expression> symbolParser(std::string_view& s, std::string& res);
std::optional<Expression> charParser(std::string_view& s);
std::optional<Expression> whitespaceParser(std::string_view& s, ParserFunction next);

std::optional<Expression> parse(std::string& s);

std::optional<Expression> multiParser(std::string_view& s);

std::optional<Expression> multiParse(std::string& s);