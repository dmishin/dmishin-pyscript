import numtheor as nt
import numpy as np

def digs(i,n):
    d=nt.digits(i,2)
    d.extend([0]*(n-len(d)))
    return d

def cntdets(n):
    N=n**2
    cnt=0
    for i in xrange(2**N):
        m=np.matrix(digs(i, N), copy=False)
        m.shape=(n,n)
        d=int(np.linalg.det(m))
        m=None
        if d % 2==1:

            cnt+=1
    return cnt
