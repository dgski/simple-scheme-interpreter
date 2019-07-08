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

(define append
    (lambda (elem col)
        (if (null? col)
            (cons elem null)
            (cons (first col) (append elem (rest col))))))

(define else #t)
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