#converting polynomial to the continued fraction, approximating it at zero


def polysum(a,b):
    if len(a)<len(b):
        a,b=b,a
    #len(a)>=len(b)
    return [ai+bi for ai,bi in zip(a,b)]+a[len(b):]

def polymul(a, k):
    return [ai*k for ai in a]

def default_is_zero(x):
    return float(abs(x))<1e-6

def cut_pow_x(a, is_zero = default_is_zero):
    m = 0
    while a and is_zero(a[0]):
        m+=1
        del a[0]

    if len(a) == 0:
        return 0
    if m==0:
        print "Warning: difference do not divided by x, as axpected. Stopping decomposition."        
    return m

def chainfrac(b,a,n=None, is_zero = default_is_zero):
##    print "----------"
##    print b,a
    if n == None:
        n = max(len(a), len(b))

    q_0 = b[0]/a[0]
    
    #b1 = b*a(0)-a*b(0)
    b1 = polysum(polymul(b,a[0]), polymul(a,-b[0]))
##    print "b1=",b1
    #b1 = b1/x^m
    m = cut_pow_x(b1, is_zero)
    n = n-m

    
    if m==0:
        return [(q_0, m)]
    if n <= 0:
        return [(q_0, m)]

    rval = chainfrac(polymul(a,a[0]),b1, n, is_zero)
    rval.insert(0, (q_0, m))

    return rval
    
def cfval(cfrac, x):
    q,m = cfrac[0]
    if len(cfrac)==1:
        return q
    else:
        return q+(x**m)/cfval(cfrac[1:],x)

def polyval(poly, x):
    s=poly[0]
    xn = x
    for a in poly[1:]:
        s = s + xn*a
        xn = xn*x
    return s

def poly2cf(poly, is_zero = default_is_zero, unit = 1):
    return chainfrac(poly,[unit], n = len(poly), is_zero = is_zero)


