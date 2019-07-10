#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../parser.h"
#include "../interpreter.h"

TEST_CASE( "Addition works", "[addition]" ) {

    std::string testString{ "(+ 200 23)" };
    auto exp = parse(testString);
    Environments env;

    auto res = eval(exp.value(), env);

    REQUIRE( std::holds_alternative<Integer>(res));
    REQUIRE( std::get<Integer>(res) == Integer{ 223 });
}