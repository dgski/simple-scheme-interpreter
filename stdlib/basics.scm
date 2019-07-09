(define atom?
    (lambda (exp)
        (or (int? exp)
            (symbol? exp)
            (function? exp)
            (string? exp))))

(define not
    (lambda (expr)
        (if expr
            #f
            #t)))

(define else #t)

(define car first)
(define cdr rest)

(define caar
    (lambda (p)
        (car (car p))))

(define cdar
    (lambda (p)
        (cdr (car p))))

(define equal?
    (lambda (a b)
        (cond
            ((int? a) (= a b))
            ((symbol? a) (eqv? a b))
            ((string? a) (string-equal? a b))))
            ((list? a) (list-equal? a b)))