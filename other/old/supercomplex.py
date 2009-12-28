#classes for numeric field extensions

from ppmatrix import *

class hcplx:

    def __init__(self, algebra, values):
        self.algebra = algebra
        if len(values) != algebra.size():
            raise ValueError("Size of the vector must be the same as algebra size")
        self.x = values

    def __str__(self):
        return "("+
                " + ".join(["%s*I%d"%(self.x[idx], idx) for idx in range(self.algebra.size)\
                               if self.x[idx]])+\
                ")"

    def matrix(self):
        

class algebra:

    def __init__(self, size):
        self.size=size
        
        #multiplication table
        self.M = [mtx(size, size) for i in range(size)]
        
    def __str__(self):
        print "Algebra of size %d"% self.size

    def Mijk(self, i, j, k):
        "M_{i,j}^<k>"
        "  k-th koefficient of the product of I_i and I_j"
        return self.M[k][i,j]

    def 
