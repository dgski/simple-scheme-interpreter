(define length
    (lambda (col)
        (if (null? col)
            0
            (+ 1 (length (rest col))))))

(define at
    (lambda (index col)
        (if (= 0 index)
            (first col)
            (at (- index 1) (rest col)))))

(define map
    (lambda (func col)
        (if (null? col)
            null
            (cons (func (first col))
                    (map func (rest col))))))

(define maptwo
    (lambda (func col-one col-two)
        (if (or (null? col-one) (null? col-two))
            null
            (cons (func (first col-one) (first col-two))
                (maptwo func (rest col-one) (rest col-two))))))

(define append
    (lambda (elem col)
        (if (null? col)
            (cons elem null)
            (cons (first col) (append elem (rest col))))))

(define filter
    (lambda (pred col)
      (cond
        ((null? col) null)
        ((pred (first col)) (cons (first col) (filter pred (rest col))))
        (else (filter pred (rest col))))))

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

(define list-equal?
    (lambda (a b)
        (cond
            ((and (null? a) (null? b)) #t)
            ((null? a) #f)
            ((null? b) #f)
            (else (and (equal? (first a) (first b))
                       (list-equal? (rest a) (rest b)))))))

(define list?
    (lambda (exp)
        (not (atom? exp))))

(define contains
    (lambda (subcol col)
        (cond
            ((and (null? subcol) (null? col)) #t)
            ((null? subcol) #t)
            ((null? col) #f)
            ((and (equal? (first subcol) (first col))
                  (contains (rest subcol) (rest col))) #t)
            (else (contains subcol (rest col))))))

(define take
    (lambda (i col)
        (if (= i 0)
            null
            (cons (first col) (take (- i 1) (rest col))))))