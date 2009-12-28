import math

default_tol = 1e-14

class float_interval:

    def __init__(self, arg0, arg1=None):
        if isinstance(arg0, float_interval) and arg1==None:
            self.val = arg0.val
            self.tol = arg0.tol
            return
        self.val = float(arg0)
        if arg1!=None:
            self.tol = arg1
        else:
            self.tol = arg0 * default_tol

    def __float__(self):
        return self.val

    def __int__(self):
        return int(self.val)

    def __add__(self, x):
        if isinstance(x, float_interval):
            return float_interval(self.val+x.val, self.tol+x.tol)
        else:
            return float_interval(self.val+x, self.tol)

    def __mul__(self, x):
        if isinstance(x, float_interval):
            return float_interval(self.val*x.val, abs(self.val*x.tol)+abs(x.val*self.tol))
        else:
            return float_interval(self.val*x, abs(self.tol*x))

    def __div__(self,x):
        if isinstance(x, float_interval):
            return float_interval(self.val/x.val,
                                  abs(self.tol/x.val)+abs(x.tol*self.val/(x.val*x.val)))
        else:
            return float_interval(self.val/x.val,
                                  abs(self.tol/x.val)+abs(x.tol*self.val/(x.val*x.val)))
    def __sub__(self, x):
        if isinstance(x, float_interval):
            return float_interval(self.val-x.val, self.tol+x.tol)
        else:
            return float_interval(self.val-x, self.tol)

    def __str__(self):
        return "(%s|%1.2g)"%(self.val, self.tol)
    
    def __repr__(self):
        return "float_interval(%g,%g)"%(self.val, self.tol)

    def sin(self):
        return float_interval(
            math.sin(self.val)+abs(math.cos(self.val)*self.tol))
    def cos(self):
        return float_interval(
            math.cos(self.val)+abs(math.sin(self.val)*self.tol))
    def exp(self):
        ex = math.exp(self.val)
        return float_interval(ex, ex*self.tol)
    def fun(self, func):
        v0=func(self.val-self.tol)
        v1=func(self.val+self.tol)
        return float_interval((v0+v1)*0.5, (v1-v0)*0.5)
            
