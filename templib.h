constexpr const char* add = R"(
    (define +
        (lambda (a b)
            (if (= a 0) b (addOne (+ (minusOne a) b)))))
)";

constexpr const char* addTwo = R"(
    (define addTwo
        (lambda (x)
            (addOne (addOne x))))
)";

constexpr const char* mapFunc = R"(
    (define map
        (lambda (func col)
            (if (null? col)
                null
                (cons (func (first col))
                      (map func (rest col))))))
)";

constexpr const char* filterFunc = R"(
    (define filter
        (lambda (pred col)
            (if (null? col)
                null
                (if (pred (first col))
                    (cons (first col) (filter pred (rest col)))
                    (filter pred (rest col))))))
)";

constexpr const char* rangeListFunc = R"(
    (define range-list
        (lambda (s e)
            (if (= s e)
                null
                (cons s (range-list (addOne s) e)))))
)";
