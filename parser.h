#include <string>
#include <variant>
#include <optional>
#include <list>
#include <iostream>

#include "expression.h"

using ParserFunction = std::optional<const Expression>(std::string_view& s);

bool isValidSymbol(const char c);
bool isWhiteSpace(const char c);

std::optional<const Expression> boolParser(std::string_view& s);
std::optional<const Expression> intParser(std::string_view& s, std::string& intStr);
std::optional<const Expression> symbolParser(std::string_view& s, std::string& res);
std::optional<const Expression> listParser(std::string_view& s);
std::optional<const Expression> commentParser(std::string_view& s);
std::optional<const Expression> quoteParser(std::string_view& s);
std::optional<const Expression> charParser(std::string_view& s);
std::optional<const Expression> whitespaceParser(std::string_view& s, ParserFunction next);
std::optional<const Expression> multiParser(std::string_view& s);

std::optional<const Expression> parse(std::string& s);
std::optional<const Expression> multiParse(std::string& s);