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