(defmacro when-my (condition &rest body)
  `(if ,condition (progn ,@body)))

(defun prt (x)
  (format t "~a~%" x))

(defun my-last (lst)
  (let ((lst-cdr (cdr lst)))
    (if (lst-cdr)
      (my-last (lst-cdr))
      lst)))

(when-my (> 2 1) 
  (prt "Hello!"))

(defmacro progn-prt (&rest args)



(let ((lst (list 1 2 3 :SIN :hello)))
  (progn
    (prt "Car is:")
    (prt (car lst))
    (prt "Last:")
    (prt (last lst))

    (prt "Cdr is:")
    (prt (cdr lst))
    (prt lst)
    (prt "That's all!")))


