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

(define car first)
(define cdr rest)

(define caar
    (lambda (p)
        (car (car p))))

(define cdar
    (lambda (p)
        (cdr (car p))))