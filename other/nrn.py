import random
import math
from matplotlib import pyplot as pp
import numpy as np

class nrn:
    def __init__(self):
        self.exc = 0.0
        self.y = 0.0
        self.k_exc = 3.0
        self.exc_tresh = 0.3
        self.y_speed = 0.1
        self.y0=0.7
        self.y1=0.9

        self.index=None
        self.val =0.0


    def sim(self, dt, u):
        if u>2.0: u = 2.0
        
        if self.y<=0.0:
            dexc = -self.k_exc*self.exc+u+0.1*dt
        else:
            dexc = -self.k_exc*self.exc
            self.y += self.y_speed*dt

            if self.y>=1.0:
                self.y = 0.0
                
        self.exc += dexc*dt
        if self.y<=0.0 and self.exc >= self.exc_tresh:
            self.y+=self.y_speed*dt

        self.val = self.out()
        return self.val

    def out(self):
        if self.y > self.y0 and self.y< self.y1:
            return 1.0
        else:
            return 0.0

    def __str__(self):
        return "N{%g; %g}"%(self.exc, self.y)

    def rand(self):
        self.y = random()
        

class net:
    def __init__(self):
        self.nrns=[]
        self.axons=dict()
        
    def add(self, n):
        self.nrns.append(n)
        n.index=len(self.nrns)-1
        return n.index

    def add_axn(self, src, dst, k):
        axlist = self.axons.get(dst, None)
        if axlist==None:
            axlist=list()
        axlist.append([src,k])
        self.axons[dst]=axlist

    def sim(self, dt):
        v=0.0
        for n in self.nrns:
            s=0.0
            for src, k in self.axons.get(n.index,()):
                sn=self.nrns[src]
                s+=sn.val * k
            n.sim(dt, s)
            v+=n.exc
        return v
            
            

def rndnet(N=100,M=10):
    n=net()
    for i in range(N):
        nr = nrn()
        nr.y=random.random()*0.5
        n.add(nr)

    k = 1.5
    for i in range(N):
        for j in range(M):
            si=random.randint(0,N-1)
            n.add_axn(si, i, random.random()*k)

    return n

def test():
    n = rndnet(100, 8)

    t=0.0
    dt=0.01
    y=[]
    tlast=-1000
    while t<100:
        y.append((t, n.sim(dt)))
        t+=dt

    v=np.array(y)
    pp.plot(v[:,0],v[:,1])
    pp.savefig("c:\\dmishin\\fig.png")
    pp.close()

    
    
