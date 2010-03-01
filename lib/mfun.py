from numpy import dot, diag
from numpy.linalg import eig, inv
def mfun(A, f):
    "Apply function to a matrix"
    d,v=eig(A)
    return dot(dot((v), diag(map(f, d))), inv(v))
