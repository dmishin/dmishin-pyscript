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

(defun forall (fun lst)
  (if lst
    (progn
      (funcall fun (car lst))
      (forall fun (cdr lst)))))
(forall (function prt) (list 1 2 3 4 9 :aaauu))

(defun progp (&rest args)
  (forall (function prt) args))


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


