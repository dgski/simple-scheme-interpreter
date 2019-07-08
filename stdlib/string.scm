(define string-empty?
    (lambda (str)
        (= (string-len str) 0)))

(define string-at
    (lambda (index str)
        (string-slice index (+ index 1) str)))

(define string-first
    (lambda (str)
            (string-at 0 str)))

(define string-rest
    (lambda (str)
        (string-slice 1 (- (string-len str) 1) str)))

(define string->list
    (lambda (str)
        (if (string-empty? str)
            null
            (cons (string-first str) (string->list (string-rest str))))))

(define list->string
    (lambda (col)
        (accumulate string-append "" col)))