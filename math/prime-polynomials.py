import numtheor

N=2000000
print "Making prime map until %d"%N

primes = [True]*N

def pinout( k ):
    idx = k+k
    while idx < N:
        primes[idx] = False
        idx += k
for i in xrange(2, N):
    if primes[i]: pinout(i)


def test( a, b):
    nprimes = 0
    nall = 1
    x = 0
    while True:
        y = x*x + a*x + b
        if y>=N: break
        if primes[y]:
            nprimes += 1
        nall += 1
        x+=1
    return float(nprimes)/float(nall)

import numpy

def make_map( A,B):
    V=numpy.zeros((A,B))
    for a in xrange(A):
        for b in xrange(B):
            V[a,b] = test(a,b)
    return V
