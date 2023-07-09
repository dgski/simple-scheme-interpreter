#include "expression.h"

std::ostream& operator<<(std::ostream& os, const Expression& env)
{
    return std::visit(ExpressionStream{os}, env);
}

std::vector<std::shared_ptr<const Expression>> Pair::garbage;

Pair::~Pair() {
    garbage.push_back(first);
    garbage.push_back(second);
}