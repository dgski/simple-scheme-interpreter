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

TEST_CASE("evaluating 'import' special form", "[specialforms]")
{
    std::string expString{"(import \"../../stdlib/basics.scm\")"};
    Environments env;
    env.add();
    auto exp = parse(expString);
    auto res = eval(exp.value(), env);
    auto elseExpr = env.get(Symbol{"else"});

    REQUIRE(std::holds_alternative<Boolean>(elseExpr));
    REQUIRE(std::get<Boolean>(elseExpr) == Boolean{true});
}

TEST_CASE("apply '+' primitive function", "[primtiveFunction]")
{
    std::string expString{"(+ 1 2)"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Integer>(res));
    REQUIRE(std::get<Integer>(res) == Integer{3});
}

TEST_CASE("apply '-' primitive function", "[primtiveFunction]")
{
    std::string expString{"(- 100 10)"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Integer>(res));
    REQUIRE(std::get<Integer>(res) == Integer{90});
}

TEST_CASE("apply '=' primitive function", "[primtiveFunction]")
{
    std::string expString;
    Environments env;

    SECTION("equal")
    {
        expString = "(= 100 100)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);
        
        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{true});
    }

    SECTION("not equal")
    {
        expString = "(= 120 100)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);
        
        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{false});
    }  
}

TEST_CASE("apply '<' primitive function", "[primtiveFunction]")
{
    std::string expString;
    Environments env;

    SECTION("equal")
    {
        expString = "(< 20 100)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);
        
        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{true});
    }

    SECTION("not equal")
    {
        expString = "(< 2000 300)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);
        
        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{false});
    }
}

TEST_CASE("apply 'cons' primitive function", "[primtiveFunction]")
{
    std::string expString{"(cons 'a 'b)"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Pair>(res));

    const auto& p = std::get<Pair>(res);

    REQUIRE(std::holds_alternative<Symbol>(*p.first));
    REQUIRE(std::get<Symbol>(*p.first) == Symbol{"a"});
    REQUIRE(std::holds_alternative<Symbol>(*p.second));
    REQUIRE(std::get<Symbol>(*p.second) == Symbol{"b"});
}

TEST_CASE("apply 'first' primitive function", "[primtiveFunction]")
{
    std::string expString{"(first '(a b))"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Symbol>(res));
    REQUIRE(std::get<Symbol>(res) == Symbol{"a"});
}

TEST_CASE("apply 'rest' primitive function", "[primtiveFunction]")
{
    std::string expString{"(rest (cons 'a 'b))"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Symbol>(res));
    REQUIRE(std::get<Symbol>(res) == Symbol{"b"});
}

TEST_CASE("apply 'null?' primitive function", "[primtiveFunction]")
{
    std::string expString;
    Environments env;

    SECTION("is null")
    {
        expString = "(null? null)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);

        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{true});
    }

    SECTION("is not null")
    {
        expString = "(null? '(1 2 3 4))";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);

        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{false});
    }
}

TEST_CASE("apply 'eqv?' primitive function", "[primtiveFunction]")
{
    std::string expString;
    Environments env;

    SECTION("Symbols are equivalent")
    {
        expString = "(eqv? 'hello 'hello)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);

        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{true});
    }

    SECTION("Symbols are not equivalent")
    {
        expString = "(eqv? 'iron-man 'spider-man)";
        auto exp = parse(expString);
        auto res = eval(exp.value(), env);

        REQUIRE(std::holds_alternative<Boolean>(res));
        REQUIRE(std::get<Boolean>(res) == Boolean{false});
    }
}

TEST_CASE("apply 'println' primitive function", "[primtiveFunction]")
{
    std::string expString{"(println 20202)"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Null>(res));
}

TEST_CASE("apply 'begin' primitive function", "[primtiveFunction]")
{
    std::string expString{"(begin 'a 2 (cons 1 2) #f)"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Boolean>(res));
    REQUIRE(std::get<Boolean>(res) == Boolean{false});
}

TEST_CASE("apply 'list' primitive function", "[primtiveFunction]")
{
    std::string expString{"(list (+ 10 20))"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    REQUIRE(std::holds_alternative<Pair>(res));

    const auto& p = std::get<Pair>(res);

    REQUIRE(std::holds_alternative<Integer>(*p.first));
    REQUIRE(std::get<Integer>(*p.first) == Integer{30});
    REQUIRE(std::holds_alternative<Null>(*p.second));
}

TEST_CASE("apply 'current-time' primitive function", "[primitiveFunction]")
{
    std::string expString{"(current-time)"};
    auto exp = parse(expString);
    Environments env;
    auto res = eval(exp.value(), env);

    auto currentTime = Integer{std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count()};

    std::cout << res << std::endl;
    std::cout << currentTime << std::endl;

    REQUIRE(std::holds_alternative<Integer>(res));

    const bool withinRange = 
        (currentTime - 1000) < std::get<Integer>(res) ||
        std::get<Integer>(res) > (currentTime + 1000);

    REQUIRE(withinRange);
}