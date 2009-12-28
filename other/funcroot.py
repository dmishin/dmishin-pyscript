#Calculation of Taylor series of functional roots for functions, having 
#root at zero: f(0) = 0.

from numtheor import sqrti, factorial
from math import sqrt
from rational import rat

def funcroot(b ,sqrt_b1):
    "Functional root of function sum(b(i)*x^i, i=1..n)\n"
    "b is list of coefficients of the original fucntion\n"
    "sqrt_b1 is a square root of the b[0]."
    
    a = [None]*len(b) #return value
    
    
    a[0] = 0
    a[1] = sqrt_b1
    
    Pij_memo = dict() #dictionary for memoization of P(i,j) fucntion
    
    def P(n,m):
        
        if n==1:
            return a[m]
        if m<n:
            return 0

        key = (n,m)
        if key in Pij_memo:
            return Pij_memo[key]
        
        if m==n:
            v = a[1]**m
        else:
            v = sum([ a[i]*P(n-1,m-i) for i in range(1,m) ])

        Pij_memo[key]=v
        return v
    
    def aj(j):
        s = sum([a[i]*P(i,j)  for i in range(2,j) ])
        return (b[j] - s)/(a[1]+a[1]**j)
    
    for j in xrange(2,len(a)):
        a[j]=aj(j)
    return a
    
#generator-based version of the same function. 
def funcrootgen(b ,sqrt_b1):
    "Functional root of function sum(b(i)*x^i, i=1..n)"
    a = [None]*len(b) #return value
    
    a[0] = 0
    a[1] = sqrt_b1
    
    Pij_memo = dict() #dictionary for memoization of P(i,j) fucntion
    def P(n,m):
        
        if n==1:
            return a[m]
        if m<n:
            return 0

        key = (n,m)
        if key in Pij_memo:
            return Pij_memo[key]
        
        if m==n:
            v = a[1]**m
            Pij_memo[key]=v
            return v
        v = sum([ a[i]*P(n-1,m-i) for i in range(1,m) ])
        Pij_memo[key]=v
        return v
    
    def aj(j):
        s = sum([a[i]*P(i,j)  for i in range(2,j) ])
        return (b[j] - s)/(a[1]+a[1]**j)
    
    for j in xrange(2,len(a)):
        a[j]=aj(j)
        yield a[j]
    

#sample usege functions

def funcroot_sin(n):
    def sink(i):
        if i%2==0:
            return 0
        return rat(1,factorial(i)*(-1)**((i-1)/2))
        
    a = [ sink(i) for i in range(n)  ]
    return funcroot(a, 1)

def funcroot_sx(n):
        a=[0 for i in range(n)]
        k = 1
        while k<n:
                a[k]=rat(1,k)
                k=k*2
        return funcroot(a, 1)
                

def funcroot_sin_gen(n):
    def sink(i):
        if i%2==0:
            return 0
        return rat(1,factorial(i)*(-1)**((i-1)/2))
        
        a = [ sink(i) for i in range(n)  ]

        out = open("sin-funcroot.py","w")
        out.write("from rational import rat\n")
        out.write("a=[]\n")

        try:
                idx = 1
                for r in funcrootgen(a, 1):
                        out.write("a.append(%s)\n"%repr(r))
                        out.write("\n")
                        out.flush()
                        print idx, float(r)
                        idx+=1
        except Exception:
                out.close()
        
                

def test():      
    import sys
    sys.setrecursionlimit(10000)
    pr = funcroot_sx(66)
    import poly2rat as p2r
    cr = p2r.poly2cf(pr[:10], lambda x: x==0 if isinstance(x,int) else x.num==0)
    print cr
