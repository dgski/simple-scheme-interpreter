# Scheme Interpreter

A subset of the Scheme language; implementation written in Modern C++. Not fast but the code is clean and easy to understand. Implementation limited by heavy stack utilization during deeply nested recursive calls.

## Special Forms

| Form | Signature |
| ---- | ----------|
| define| [name] [expr] -> Null |
| if | [predicate] [expr] [expr] -> [expr]|
| cond | [expr] ... -> [expr] |
| quote | [expr] -> Pair |
| env | -> Null |
| lambda | [list of names] [expr] -> Closure|
| import | [expr:string] -> Null|

## Primitives

| Function | Signature |
| ---------- | ----------|
| + | Integer Integer -> Integer |
| - | Integer Integer -> Integer |
| < | Integer Integer -> Boolean |
| cons | [expr] [expr] -> Pair |
| first | Pair -> [expr] |
| rest | Pair -> [expr] |
| null? | [expr] -> Boolean |
| eqv? | Symbol Symbol -> Boolean |
| println | [expr] -> Null |
| exit | -> Null |
| begin | [expr] ... -> expr |
| list | [expr] ... -> Pair |
| current-time | -> Integer |
| int? | [expr] -> Boolean |
| symbol? | [expr] -> Boolean | 
| function? | [expr] -> Boolean |
| and | Boolean ... -> Boolean |
| or | Boolean ... -> Boolean |
| string-append | String String -> String |
| string-len | String -> Integer |
| string-slice | Integer Integer String -> String|
| string-equal? | String String -> Boolean |
