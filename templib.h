constexpr const char* add = R"(
    (define +
        (lambda (a b)
            (if (= a 0) 0 (addOne (+ (minusOne a) b)))))
)";