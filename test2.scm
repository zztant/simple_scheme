
(display "123123")

(define queue (list 2 3 4 5 6))

(define (enqueue q elem)
 (if (null? q) (list elem) 
     (cons (car q) (enqueue (cdr q) elem)))) 

(define (dequeue q) (cdr q))


