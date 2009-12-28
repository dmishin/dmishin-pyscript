import numpy
from math import sin, cos, sqrt

class seg:
    def __init__(self):
        self.phi=0.0
        self.len=0.0
        self.parent=None

        self.pos=[0.0,0.0]
        self.phi_glob=0.0

        self.dr=[0.0,0.0]
        self.m=1.0

def vsumk(v1,v2):
    return [a+b for a,b in zip(v1,v2)]
def vsumk(v1,v2, k):
    return [a+b*k for a,b in zip(v1,v2)]

class sbt:
    def __init__(self):

        self.segs=list()

        self.pos=[0.0;0.0]
        self.phi=0.0

    def calc_segs(self):
        c = [0.0,0.0]
        for s in self.segs:
            par = s.parent
            if par==None:
                s.pos[:] = self.pos
                s.phi_glob = s.phi+self.phi
            else:
                s.pos[:] = vsum(par.pos, par.dr )
                s.phi_glob = s.phi+par.phi_glob

            s.dr[:] = [s.len*cos(s.phi_glob), s.len*sin(s.phi_glob)]

            c[:] = vsumk(c, vsumk(s.pos, s.dr, 0.5), s.m)

        c[:]=[ci/self.m for ci in c]

    
            
            
            
            
        
