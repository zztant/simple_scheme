(define (generate-one-element-at-a-time lst)
 (define (control-state return)
  (for-each (lambda (element)
			 (call/cc (lambda (resume-here)
					   (set! control-state resume-here)
					   (return element))))
			lst)
  (return 'end))
 (define (generator)
  (call/cc control-state))
 generator)


(define generate-digit
 (generate-one-element-at-a-time '(0 1 2)))

(define queue '())
(define (empty-queue?)
 (null? queue))
(define (enqueue x)
 (set! queue (append queue (list x))))
(define (dequeue)
 (let ((x (car queue)))
  (set! queue (cdr queue))
  x))

(define (fork proc)
 (call/cc 
  (lambda (k)
   (enqueue k)
   (proc))))

(define (yield)
 (call/cc
  (lambda (k)
   (enqueue k)
   ((dequeue)))))

(define (thread-exit)
 (if (empty-queue?)
	 (exit)
	 ((dequeue))))

(define (do-stuff-n-print str)
 (lambda ()
  (let loop ((n 0))
   (format #t "~A~A\n" str n)
   (yield)
   (loop (1+ n)))))
