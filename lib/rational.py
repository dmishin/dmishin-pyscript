#Python module for simple rational numbers

import numtheor
import math

def float2rat(f, eps = 1e-6, iters = 1000):
    "Converts float to rational"
    flr = math.floor(f)
    frac = f-flr
    if eps<=0:
        raise ValueError, "Treshold must be positive"
    
    if iters == 0 or abs(frac)<eps:
        return rat(int(flr),1) #it is actually an integer

    ifrac = 1.0/frac
    
    return rat(int(flr))+1/float2rat(ifrac,eps*ifrac*ifrac, iters-1)

def str2rat(s, base=10):
    "Convert string in a form num/den to a rational. String may be single integer too."
    
    if not isinstance(s,basestring):
        raise ValueError, "Argument must be string"
    
    spl = s.split("/")
    if len(spl)==1:
        return rat(int(s, base))
    if len(spl)==2:
        return rat( int(spl[0],base), int(spl[1],base) )
    raise ValueError, "String is not a fraction: "+s
    
class rat:
    "Rational number class"
    
    def __init__(self,num=0,den=1, normalize = True):
        "Create rational value"
        self.num = int(num)
        self.den = int(den)
        if normalize:
            self._norm()
    
    def __str__(self):
        if self.den == 1:
            return str(self.num)
        return "%d/%d"%(self.num, self.den)
    
    def __repr__(self):
        return "rat(%d, %d)"%(self.num, self.den)
    
    def __float__(self):
        return float(self.num)/float(self.den)
    
    def _norm(self):
        "Normalize: remove the common denominator"
        if self.num == 0:
            self.den = 1
            return
        if self.den == 0:
            #invalud value
            if   self.num < 0:
                self.num = -1
                
            elif self.num > 0:
                self.num = 1
                    
            return
            
        if self.den<0: #normalize sign
            self.num = -self.num
            self.den = -self.den
            
        k = numtheor.gcd(self.num, self.den)
        self.num /= k
        self.den /= k
    
    def intpart(self):
        return self.num/self.den
    
    def copy(self):
        return rat(self.num, self.den, False)
        
    def __mul__(self, v):
        if isinstance(v, int) or isinstance(v, long):
            return rat(self.num*v, self.den)
        if isinstance(v, rat):
            return rat(self.num * v.num, self.den*v.den)
        raise ValueError, "Wrong type of multipolier"
    
    def __rmul__(self, v):
        return self.__mul__(v)
        
    def __div__(self, v):
        if isinstance(v, int) or isinstance(v, long):
            return rat(self.num, self.den*v)
        if isinstance(v, rat):
            return rat(self.num * v.den, self.den*v.num)
        raise ValueError, "Wrong type of multipolier"
    
    def __rdiv__(self, v):
        if isinstance(v, int) or isinstance(v, long):
            return rat(self.den*v, self.num)
        if isinstance(v, rat):
            return rat(self.den * v.num, self.num*v.den)
        raise ValueError, "Wrong type of multipolier"
        
    def __add__(self, v):
        if isinstance(v, int) or isinstance(v, long):
            return rat(self.num+v*self.den, self.den)
        if isinstance(v, rat):
            return rat(self.num * v.den + self.den*v.num, self.den*v.den)
        raise ValueError, "Wrong type of second argument: %s"%v
    def __radd__(self, v):
        return self.__add__(v)

        
    def __pow__(self, v):
        return rat(self.num**v, self.den**v, False)
    
    def __sub__(self,v):
        if isinstance(v, int) or isinstance(v, long):
            return rat(self.num-v*self.den, self.den)
        if isinstance(v, rat):
            return rat(self.num * v.den - self.den*v.num, self.den*v.den)
        raise ValueError, "Wrong type of second argument: %s"%v

    def __neg__(self):
        return rat(-self.num, self.den, False)

    def __rsub__(self,v):
        "v - rat"
        if isinstance(v, int) or isinstance(v, long):
            return rat(-self.num+v*self.den, self.den)
        if isinstance(v, rat):
            return rat(-self.num * v.den + self.den*v.num, self.den*v.den)
        raise ValueError, "Wrong type of second argument: %s"%v
                
    
    def __abs__(self):
        "absolute value"
        return rat(abs(self.num),abs(self.den),False)

    def __nonzero__ (self):
        return self.num != 0
            
    def __eq__(self, v):
        if isinstance(v, rat):
            return self.num*v.den == self.den*v.num
        if isinstance(v, int) or isinstance(v, long):
            return self.num*v == self.den
        raise ValueError, "Can not compare rational and %s"%v

    def __le__(self, v):
        if isinstance(v, rat):
            return self.num*v.den <= self.den*v.num
        else:
            return self.num <= self.den*v
    def __lt__(self, v):
        if isinstance(v, rat):
            return self.num*v.den < self.den*v.num
        else:
            return self.num < self.den*v

    def __hash__(self):
        return (self.num, self.den).__hash__()

    def to_chain(self):
        "Converts rational to chain fraction, that is returned as list."
        chain = list()
        num = self.num
        den = self.den
        while den != 0:
            ipart = num/den
            chain.append(ipart)
            num -= den*ipart
            num,den = den, num
        return chain
    
    def from_chain(self, chain):
        "Converts value from chain fraction"
        num = 1
        den = 0
        for c in chain[::-1]:
            num, den = den, num
            num += c*den
        self.num = num
        self.den = den

    def __int__(self):
        return self.intpart()
