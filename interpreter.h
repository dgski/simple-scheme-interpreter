#pragma once

#include <variant>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <memory>

#include "expression.h"
#include "utils.h"


Expression eval(Expression& exp, Environments& env);
std::optional<Expression> applySpecialForm(SymbolInstance& formName, List& args, Environments& env);
std::optional<Expression> getPrimitiveFunction(SymbolInstance& s);

class Evaluator
{
    Environments& env;
public:
    Evaluator(Environments& _env) : env(_env) {}

    Expression operator()(NullInstance& n) { return n; }
    Expression operator()(SymbolInstance& s);
    Expression operator()(IntegerInstance& i) { return i; }
    Expression operator()(BooleanInstance& b) { return b; }
    Expression operator()(PairInstance& p);
    Expression operator()(Closure& f) { return f; }
};
// end evaluation

// helper
void insertArgsIntoEnvironment(PairInstance& names, PairInstance& args, Environment& env);
Expression evalAllArgsInList(Expression& exp, Environments& env);
// end helper