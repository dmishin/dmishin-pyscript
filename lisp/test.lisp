(defun prt (x)
  (format t "~a~%" x))
;;This prints helloworld
(prt "Hello world")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;playing with defmacro

(defun print-times (n msg)
  ;Repeat <code> n times
  (let ((repeater
          (lambda (n self)
            (progn 
              (prt msg)
              (if (> n 0)
                (funcall self (- n 1) self))))))

    (funcall repeater n repeater)))

(print-times 10 "Hello world")
