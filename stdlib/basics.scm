(define +
    (lambda (a b)
        (if (= a 0)
            b
            (addOne (+ (minusOne a) b)))))

(define minus
    (lambda (a b)
        (if (= b 0)
            a
            (minusOne (minus a (minusOne b))))))

(define *
    (lambda (a b)
        (if (= a 0)
            0
            (+ b (* (minus a 1) b)))))

(define **
    (lambda (a b)
        (if (= a 0)
            1
            (* b (** (minus a 1) b)))))

(define length
    (lambda (col)
        (if (null? col)
            0
            (addOne (length (rest col))))))

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
            (cons s (iota (addOne s) e)))))

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

(define car first)
(define cdr rest)

(define caar
    (lambda (p)
        (car (car p))))

(define cdar
    (lambda (p)
        (car (cdr p))))

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

(define test (struct '(id)))
(define item (test '(100)))