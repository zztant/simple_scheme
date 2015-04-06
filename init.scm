(define-syntax let
 (syntax-rules ()
  ( (let ((_x _e) ...) _b1 _b2 ...)
    ((lambda (_x ...) _b1 _b2 ...) _e ... ))))

(define-syntax let*
  (syntax-rules ()
    ((let* () _body1 _body2 ...)
     (let () _body1 _body2 ...))
    ((let* ((_name1 _val1) (_name2 _val2) ...)
       _body1 _body2 ...)
     (let ((_name1 _val1))
       (let* ((_name2 _val2) ...)
         _body1 _body2 ...)))))

(define-syntax list
 (syntax-rules ()
  ((_) '())
  ((_ _v1) (cons _v1 '()))
  ((_ _v1 _v2 ...) (cons _v1 (list _v2 ...)))))

(define-syntax and
 (syntax-rules ()
  ( (and) #t )
  ( (and _v1) _v1 )
  ( (and _v1 _v2 _v3 ...) 
    (if _v1 
        (and _v2 _v3 ...)
 	#f))))      

(define-syntax or
 (syntax-rules ()
  ( (or) #f )
  ( (or _v1) _v1 )
  ( (or _v1 _v2 ...) 
    (if _v1
        _v1
        (or _v2 ...)))))

(define-syntax min
 (syntax-rules ()
  ( (_ _v1) _v1 )
  ( (_ _v1 _v2)
	(if (< _v1 _v2) _v1 _v2))
  ( (_ _v1 _v2 _v3 ...)
	(if (< _v1 (min _v2 _v3 ...))
	    _v1
		(min _v2 _v3 ...)))))

(define-syntax max
 (syntax-rules ()
  ( (_ _v1) _v1 )
  ( (_ _v1 _v2)
	(if (> _v1 _v2) _v1 _v2))
  ( (_ _v1 _v2 _v3 ...)
	(if (> _v1 (max _v2 _v3 ...))
	    _v1
		(max _v2 _v3 ...)))))

(define-syntax <=
 (syntax-rules ()
  ( (_) #t )
  ( (_ _v1) #t )
  ( (_ _v1 _v2 _v3 ...)
	(if (> _v1 _v2) 
		#f
	    (<= _v2 _v3 ...)))))

(define-syntax >=
 (syntax-rules ()
  ( (_) #t )
  ( (_ _v1) #t )
  ( (_ _v1 _v2 _v3 ...)
	(if (< _v1 _v2) 
		#f
	    (>= _v2 _v3 ...)))))

(define-syntax cond
 (syntax-rules (else)
  ((_ (else _e1 _e2 ...)) (begin _e1 _e2 ...))
  ((_ (_e0 _e1 _e2 ...)) (if _e0 (begin _e1 _e2 ...)))
  ((_ (_e0 _e1 _e2 ...) _c1 _c2 ...)
   (if _e0 (begin _e1 _e2 ...) (cond _c1 _c2 ...)))))

(define (set-car! x y)
 (cons y (cdr x)))

(define (set-cdr! x y)
 (cons (car x) y))

(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))
(define (caaar x) (car (car (car x))))
(define (caadr x) (car (car (cdr x))))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))
(define (cdaar x) (cdr (car (car x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))
(define (cdddr x) (cdr (cdr (cdr x))))
(define (caaaar x) (car (car (car (car x)))))
(define (caaadr x) (car (car (car (cdr x)))))
(define (caadar x) (car (car (cdr (car x)))))
(define (caaddr x) (car (car (cdr (cdr x)))))
(define (cadaar x) (car (cdr (car (car x)))))
(define (cadadr x) (car (cdr (car (cdr x)))))
(define (caddar x) (car (cdr (cdr (car x)))))
(define (cadddr x) (car (cdr (cdr (cdr x)))))
(define (cdaaar x) (cdr (car (car (car x)))))
(define (cdaadr x) (cdr (car (car (cdr x)))))
(define (cdadar x) (cdr (car (cdr (car x)))))
(define (cdaddr x) (cdr (car (cdr (cdr x)))))
(define (cddaar x) (cdr (cdr (car (car x)))))
(define (cddadr x) (cdr (cdr (car (cdr x)))))
(define (cdddar x) (cdr (cdr (cdr (car x)))))
(define (cddddr x) (cdr (cdr (cdr (cdr x)))))

(define (map proc sequence)
 (if (null? sequence)
     '()
	 (cons (proc (car sequence))
	       (my-map proc (cdr sequence)))))



