(defun prt (x)
  (format t "~a~%" x))
;;This prints helloworld
(prt "Hello world")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;playing with defmacro

(defun print-times (n msg)
  ;Repeat <code> n times
  (let ((repeater
          (lambda (n)
            (prog
              (prt msg)
              (if (> n 0)
                (funcall repeater (- n 1)))))))
    (funcall repeater n)))

(print-times 10 "Hello world")
