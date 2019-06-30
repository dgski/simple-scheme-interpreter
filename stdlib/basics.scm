(define +
    (lambda (a b)
        (if (= a 0) b (addOne (+ (minusOne a) b)))))

(define addTwo
    (lambda (x)
        (addOne (addOne x))))

(define map
    (lambda (func col)
        (if (null? col)
            null
            (cons (func (first col))
                    (map func (rest col))))))

(define filter
    (lambda (pred col)
        (if (null? col)
            null
            (if (pred (first col))
                (cons (first col) (filter pred (rest col)))
                (filter pred (rest col))))))

(define iota
    (lambda (s e)
        (if (= s e)
            null
            (cons s (iota (addOne s) e)))))