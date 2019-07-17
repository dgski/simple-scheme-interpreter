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
const Expression eval(const Expression& exp, Environments& env);
std::optional<const Expression> applySpecialForm(const Symbol& formName, const List& args, Environments& env);
std::optional<const Expression> getPrimitiveFunction(const Symbol& s);
const Expression parseAndEvalFile(const char* fileName, Environments& env);

class Evaluator
{
    Environments& env;
public:
    Evaluator(Environments& _env) : env(_env) {}

    const Expression operator()(const Null& n) { return n; }
    const Expression operator()(const Symbol& s);
    const Expression operator()(const String& s) { return s; }
    const Expression operator()(const Integer& i) { return i; }
    const Expression operator()(const Boolean& b) { return b; }
    const Expression operator()(const Pair& p);
    const Expression operator()(const Closure& f) { return f; }
};
// end evaluation

// helper
void insertArgsIntoEnvironment(const Pair& names, const Pair& args, Environment& env);
const Expression evalAllArgsInList(const Expression& exp, Environments& env);
// end helper