; math
(define <=
    (lambda (a b)
        (or (= a b)
            (< a b))))

(define >
    (lambda (a b)
        (not (<= a b))))

(define =>
    (lambda (a b)
        (or (= a b)
            (> a b))))
(define *
    (lambda (a b)
        (if (= a 0)
            0
            (+ b (* (- a 1) b)))))

(define **
    (lambda (a b)
        (if (= b 0)
            1
            (* a (** a (- b 1))))))

(define tet
    (lambda (a b)
        (if (= b 0)
            1
            (** b (tet a (- b 1))))))

(define /
    (lambda (a b)
        (if (> 0 (- a b))
            0
            (+ 1 (/ (- a b) b)))))

(define remainder
    (lambda (a b)
        (- a
               (* b (/ a b)))))

(define even?
    (lambda (n)
        (= 0 (remainder n 2))))

(define odd?
    (lambda (n)
        (not (even? n))))

; list
(define length
    (lambda (col)
        (if (null? col)
            0
            (+ 1 (length (rest col))))))

(define map
    (lambda (func col)
        (if (null? col)
            null
            (cons (func (first col))
                    (map func (rest col))))))

(define append
    (lambda (elem col)
        (if (null? col)
            (cons elem null)
            (cons (first col) (append elem (rest col))))))

(define filter
    (lambda (pred col)
        (if (null? col)
            null
            (if (pred (first col))
                (cons (first col) (filter pred (rest col)))
                (filter pred (rest col))))))

(define accumulate
    (lambda (func init col)
        (if (null? col)
            init
            (func (first col) (accumulate func init (rest col))))))

(define iota
    (lambda (s e)
        (if (= s e)
            null
            (cons s (iota (+ 1 s) e)))))

(define or
    (lambda (a b)
        (if a
            #t
            (if b
                #t
                #f))))

(define and
    (lambda (a b)
        (if a
            (if b
                #t
                #f)
            #f)))

(define not
    (lambda (expr)
        (if expr
            #f
            #t)))

(define car first)
(define cdr rest)
; struct
(define caar
    (lambda (p)
        (car (car p))))

(define cdar
    (lambda (p)
        (cdr (car p))))

(define struct-helper
  (lambda (fields values)
    (if (or (null? fields) (null? values))
        null
        (cons (cons (car fields) (car values))
              (struct-helper (cdr fields) (cdr values))))))

(define struct
  (lambda (fields)
    (lambda (values)
      (struct-helper fields values))))

(define :
  (lambda (field struct)
    (if (eqv? field (caar struct))
        (cdar struct)
        (: field (cdr struct)))))
; utils
(define runner
    (lambda (func start-time)
        (begin (func)
               (- (current-time) start-time))))

(define timed-test
    (lambda (func)
        (runner func (current-time))))