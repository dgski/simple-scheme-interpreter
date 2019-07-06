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


// evaluation


Expression eval(Expression& exp, Environments& env);
std::optional<Expression> applySpecialForm(Symbol& formName, List& args, Environments& env);
std::optional<Expression> getPrimitiveFunction(Symbol& s);
Expression parseAndEvalFile(const char* fileName, Environments& env);

class Evaluator
{
    Environments& env;
public:
    Evaluator(Environments& _env) : env(_env) {}

    Expression operator()(Null& n) { return n; }
    Expression operator()(Symbol& s);
    Expression operator()(String& s) { return s; }
    Expression operator()(Integer& i) { return i; }
    Expression operator()(Boolean& b) { return b; }
    Expression operator()(Pair& p);
    Expression operator()(Closure& f) { return f; }
};
// end evaluation

// helper
void insertArgsIntoEnvironment(Pair& names, Pair& args, Environment& env);
Expression evalAllArgsInList(Expression& exp, Environments& env);
// end helper