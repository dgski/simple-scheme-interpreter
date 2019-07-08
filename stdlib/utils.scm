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

(define runner
    (lambda (func start-time)
        (begin (func)
               (- (current-time) start-time))))

(define timed-test
    (lambda (func)
        (runner func (current-time))))