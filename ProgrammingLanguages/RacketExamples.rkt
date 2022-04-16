#lang racket

; Function 1 TODO
(define (last a L)
  (local ((define (last-help a L n)
    (cond
      [(null? L) -1]
      [(eq? a (first L)) (+ n (last-help a (rest L) 1))]
      [else (last-help a (rest L) (+ 1 n))])))
    (last-help a L 1))
)

; Function 2
(define (wrap M)
  (cond
    [(null? M) '()]
    [else (cons (cons (first M) '()) (wrap (rest M)))])
)

; Function 3
(define (count-parens-all M)
  (cond
    [(null? M) 2]
    [(list? (first M)) (+ (count-parens-all (first M)) (count-parens-all (rest M)))]
    [(pair? (first M)) (+ (count-parens-all (first M)) (count-parens-all (rest M)))]
    [else (count-parens-all (rest M))])
)

; Function 4
(define (insert-right-all new old M)
  (cond
    [(null? M) '()]
    [(list? (first M)) (cons (insert-right-all new old (first M)) (insert-right-all new old (rest M)))]
    [(pair? (first M)) (cons (insert-right-all new old (first M)) (insert-right-all new old (rest M)))]
    [(eq? old (first M)) (cons old (cons new (insert-right-all new old (rest M))))]
    [else (cons (first M) (insert-right-all new old (rest M)))])
)

; Function 5
(define (invert M)
  (cond
    [(null? M) '()]
    [else (cons (cons (first (rest (first M))) (first (first M))) (invert (rest M)))])
)

; Function 6
(define (filter-out pred L)
  (cond
    [(null? L) '()]
    [(pred (first L)) (filter-out pred (rest L))]
    [else (cons (first L) (filter-out pred (rest L)))])
)

; Function 7
(define (summatrices M1 M2)
  (cond
    [(null? M1) '()]
    [else (cons (map (lambda (x y) (+ x y)) (first M1) (first M2)) (summatrices (rest M1) (rest M2)))])
)

; Function 8
(define (swapper a1 a2 M)
  (cond
    [(null? M) '()]
    [(list? (first M)) (cons (swapper a1 a2 (first M)) (swapper a1 a2 (rest M)))]
    [(pair? (first M)) (cons (swapper a1 a2 (first M)) (swapper a1 a2 (rest M)))]
    [(eq? a1 (first M)) (cons a2 (swapper a1 a2 (rest M)))]
    [(eq? a2 (first M)) (cons a1 (swapper a1 a2 (rest M)))]
    [else (cons (first M) (swapper a1 a2 (rest M)))])
)

; Function 9
(define (flatten M)
  (cond
    [(null? M) '()]
    [(not (list? M)) (list M)]
    [else (append (flatten (first M)) (flatten (rest M)))])
)

; Function 10
(define (binary-tree-insert T n)
  1
)

; Function 11
(define (abstract f g h)
  (local ((define (helper a M)
            (cond
              [(null? M) (f a M)]
              [(not (pair? (first M))) (g a M)]
              [else (h a M)])))
  helper ))

(define rember* (abstract (lambda (a M) '())
                          (lambda (a M) (if (eq? a (first M)) (rember* a (rest M)) (cons (first M) (rember* a (rest M)))))
                          (lambda (a M) (cons (rember* a (first M)) (rember* a (rest M)))))
)

(define depth (abstract (lambda (a M) 1)
                        (lambda (a M) (depth a (rest M)))
                        (lambda (a M) (max (add1 (depth a (first M))) (depth a (rest M)))))
)