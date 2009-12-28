
def phi(a,b):
    "phi(a/b)"
    n=a/(10*b)
    a1=a%(10*b)
    if a1<=b:
        return a1+n*b,b
    else:
        return n+1,1

def phin(x,n,d):
    a,b=phi((x-n*d+1),n)
    return a*n/b

def S(x,d):
    n=1
    s=0
    while True:
        pn = phin(x,n,d)
        if pn==0:
            return s
        s+=pn
        n=n*10
        
def intersect_diag(a,b,x,y):
    return (x<=a and a<=y) or (x<=b and b<=y) or(a<=x and x<=b) or (a<=y and y<=b)

def sum_sols(a,b,fa,fb,func):
    if b-a==1:
        if fa==a: return a
        else: return 0
    if intersect_diag(a,b,fa,fb):
        c = (a+b)/2
        fc = func(c)
        return sum_sols(a,c,fa,fc,func) + sum_sols(c,b,fc,fb,func)
    else:
        return 0

def SS(d):
    func = lambda x: S(x,d)
    return int(sum_sols(0,10**11,0,func(10**11),func))

##def fin_sols(a,b,fa, fb):
##    if b-a==1:
##        if fa==a: return [a]
##        else: return []
##    if intersect_diag(a,b,fa,fb):
##        c = (a+b)/2
##        fc = S(c)
##        return fin_sols(a,c,fa,fc) + fin_sols(c,b,fc,fb)
##    else:
##        return []

print sum(map(SS,range(1,10)))
