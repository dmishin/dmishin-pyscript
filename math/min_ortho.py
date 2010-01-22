import numpy
from numpy import *
class NoSolution (Exception):
    pass

def num_nz( vec, eps=1e-6 ):
    "Number of first nonzero items in a array"
    k = 0
    n = len(vec)
    while k<n and vec[k]>=eps:
        k+=1
    return k


def positive_lineq(A,eps = 1e-6):
    "find solution Ax >= 0"
    iA = linalg.pinv( A )
    n,m = A.shape
    x0 = dot( iA, ones((n,1)) )
    x0 = dot( iA, ones( (n,1) ) )
    if all( dot(A, x0)>=0 ):
        return x0
    else:
        raise NoSolution, x0

def test_positive_lineq():
    A = numpy.array(
        [[-1, 1],
         [2, -1],
         [3, -1]])
    print "solution for"
    print A
    x = positive_lineq( A )
    print "Solution is:"
    print x
    print "Check solution:"
    b = numpy.dot(A, x)
    print b

test_positive_lineq()

def minortho():
    n = 3

    def ij2idx(i,j):
        
