#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../parser.h"
#include "../interpreter.h"

TEST_CASE("parsing Symbol type works", "[parsing]")
{
    std::string testString{"saverockandroll"};
    auto exp = parse(testString);

    REQUIRE(exp.has_value());
    REQUIRE(std::holds_alternative<Symbol>(exp.value()));
    REQUIRE(std::get<Symbol>(exp.value()) == Symbol{"saverockandroll"});
}

TEST_CASE("parsing Integer type works", "[parsing]")
{
    std::string testString;

    SECTION("positive values")
    {
        testString = "1922";
        auto exp = parse(testString);

        REQUIRE(exp.has_value());
        REQUIRE(std::holds_alternative<Integer>(exp.value()));
        REQUIRE(std::get<Integer>(exp.value()) == Integer{1922});
    }
    SECTION("negative values")
    {
        testString = "-2030302";
        auto exp = parse(testString);

        REQUIRE(exp.has_value());
        REQUIRE(std::holds_alternative<Integer>(exp.value()));
        REQUIRE(std::get<Integer>(exp.value()) == Integer{-2030302});
    }
}

TEST_CASE("parsing String type works", "[parsing]")
{
    std::string testString{ "\"I am Iron Man!\"" };
    auto exp = parse(testString);

    REQUIRE(exp.has_value());
    REQUIRE(std::holds_alternative<String>(exp.value()));
    REQUIRE(std::get<String>(exp.value()) == String{"I am Iron Man!"});
}

TEST_CASE("parsing Boolean type works", "[parsing]")
{
    std::string testString;

    SECTION("true values")
    {
        testString = "#t";
        auto exp = parse(testString);

        REQUIRE(exp.has_value());
        REQUIRE(std::holds_alternative<Boolean>(exp.value()));
        REQUIRE(std::get<Boolean>(exp.value()) == Boolean{true});
    }
    SECTION("false values")
    {
        testString = "#f";
        auto exp = parse(testString);

        REQUIRE(exp.has_value());
        REQUIRE(std::holds_alternative<Boolean>(exp.value()));
        REQUIRE(std::get<Boolean>(exp.value()) == Boolean{false});
    }
}

TEST_CASE("parsing Pair type works", "[parsing]")
{
    std::string testString{"(12 #f)"};
    auto exp = parse(testString);

    REQUIRE(exp.has_value());
    REQUIRE(std::holds_alternative<Pair>(exp.value()));
    
    auto& pair1 = std::get<Pair>(exp.value());

    REQUIRE(std::holds_alternative<Integer>(*pair1.first));
    REQUIRE(std::get<Integer>(*pair1.first) == Integer{12});
    REQUIRE(std::holds_alternative<Pair>(*pair1.second));

    auto& pair2 = std::get<Pair>(*pair1.second);

    REQUIRE(std::holds_alternative<Boolean>(*pair2.first));
    REQUIRE(std::get<Boolean>(*pair2.first) == Boolean{false});
    REQUIRE(std::holds_alternative<Null>(*pair2.second));
}

TEST_CASE("evaluating 'define' special form", "[specialforms]")
{
    std::string expString{"(define a 1)"};
    auto exp = parse(expString);
    Environments env;
    env.add();
    
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Integer>(env.get(Symbol{"a"})));
    REQUIRE(std::get<Integer>(env.get(Symbol{"a"})) == Integer{1});
}

TEST_CASE("evaluating 'if' special form", "[specialforms]")
{
    std::string expString;
    Environments env;

    SECTION("true predicate")
    {
        expString = "(if #t 'apple 'pear)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);
        
        REQUIRE(std::holds_alternative<Symbol>(res));
        REQUIRE(std::get<Symbol>(res) == Symbol{"apple"});
    }

    SECTION("false predicate")
    {
        expString = "(if #f 'apple 123)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);
        
        REQUIRE(std::holds_alternative<Integer>(res));
        REQUIRE(std::get<Integer>(res) == Integer{123});
    }   
}

TEST_CASE("evaluating 'cond' special form", "[specialforms]")
{
    std::string expString{"(cond (#f 'spider-man) (#t \"edith\") (else 32))"};
    Environments env;
    auto exp = parse(expString);
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<String>(res));
    REQUIRE(std::get<String>(res) == String{"edith"});
}

TEST_CASE("evaluating 'quote' special form", "[specialforms]")
{
    std::string expString{"(quote hello)"};
    Environments env;
    auto exp = parse(expString);
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Symbol>(res));
    REQUIRE(std::get<Symbol>(res) == Symbol{"hello"});
}

TEST_CASE("evaluating 'env' special form", "[specialforms]")
{
    std::string expString{"(env)"};
    Environments env;
    auto exp = parse(expString);
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Null>(res));
}

TEST_CASE("evaluating 'lambda' special form", "[specialforms]")
{
    std::string expString{"((lambda (x y) x) 'scheme 'lisp)"};
    Environments env;
    auto exp = parse(expString);
    auto res = eval(exp.value(), env);
    
    REQUIRE(std::holds_alternative<Symbol>(res));
    REQUIRE(std::get<Symbol>(res) == Symbol{"scheme"});
}