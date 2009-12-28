import random

def adda(v1,v2,k1=1,k2=1):
    v1[:] = [x*k1+y*k2 for x,y in zip(v1,v2)]

def add(v1,v2,k1=1,k2=1):
    return [x*k1+y*k2 for x,y in zip(v1,v2)]

def scale(v,k):
    return [x*k for x in v]
def scalea(v,k):
    x[:] = [x*k for x in v]

def copy(v):
    return v[:]

def sumv(vecs):
    s=copy(vecs[0])
    for v1 in vecs[1:]:
        adda(s,v1)
    return s


def mean(vecs):
    return scale(sumv(vecs),1.0/len(vecs))
    
def maxidx(vec):
    imin = None
    val = None
    for idx, x in enumerate(vec):
        if val == None or x>val:
            imin = idx
            val = x
    return val, imin

def minidx(vec):
    imin = None
    val = None
    for idx, x in enumerate(vec):
        if val == None or x<val:
            imin = idx
            val = x
    return val, imin

def vec_range(vecs):
    mins=copy(vecs[0])
    maxs=copy(vecs[0])
    for v in vecs[1:]:
        mins[:] = [min(x, m) for x,m in zip(v, mins)]
        maxs[:] = [max(x, m) for x,m in zip(v, maxs)]
    return add(maxs,mins,1,-1)
    
def nmead(poly, func, alpha=1.0, beta=2.0, gamma=0.5, delta=0.5, eps = 1e-6, max_steps = 1000):
    steps = 0
    calculations = 0
    
    assert(len(poly) - 1 == len(poly[0]))
    
    def refl(xc, x):
        return add(xc, x, 1+alpha, -alpha)
        
    def expand(xc, x):
        return add(xc, x, 1-beta, beta)
    def shrink(xc, x):
        return add(xc, x, 1-gamma, gamma)
    
    def is_finish(points):
        return sum(map(abs, vec_range(points)))<eps
    
    vals = map(func, poly)
    calculations += len(poly)
    
    
    while steps <= max_steps and not is_finish(poly):
        f_worst, worst_i = maxidx(vals)
        x_worst = poly[worst_i]
        print "X worst: %s, F(Xw)=%s"%(vec2str(x_worst), f_worst)
        del poly[worst_i]
        
        f_best = min(vals)

        x_c = mean(poly)
        x_r = refl(x_c, x_worst)
        f_r = func(x_r)
        calculations += 1

        print "  Center:",vec2str(x_c)
        print "  X_r=%s, F_r=%s"%(vec2str(x_r), f_r)
        print "  Fbest=%s"%(f_best)
        
        needScale = False
##        if f_r < f_best: #reflection succeed
##            x_e = expand(x_c, x_r)
##            f_e = func(x_e)
##            calculations += 1
##            print "  trying expand... F_e=%s"%f_e,
##            if f_e < f_r: #expansion succeed
##                x_next = x_e
##                f_next = f_e
##                #print "Expand:", 
##            else: #expaansion fail
##                x_next = x_r
##                f_next = f_r
##                print "Flip, noexpand:", 
##        elif f_r < f_worst: #reflection partial success
##            print "  trying contract...",
##            x_s = shrink(x_c, x_r)
##            f_s = func(x_s)
##            calculations += 1
##            if f_s < f_r: #shrink success
##                x_next = x_s
##                f_next = f_s
##                print "ShrinK:", 
##            else:
##                x_next = x_r
##                f_next = f_r
##                print "Flip, noshrink", 
##        else: #reflection fail
##            x_next = x_worst
##            f_next = f_worst
##            needScale = True
        if f_r < f_worst: #reflection succeed
            x_e = expand(x_c, x_r)
            f_e = func(x_e)
            calculations += 1
            print "  trying expand... F_e=%s"%f_e,
            if f_e < f_r: #expansion succeed
                x_next = x_e
                f_next = f_e
                #print "Expand:", 
            else: #expaansion fail
                x_next = x_r
                f_next = f_r
                print "Flip, noexpand:", 
        else : #reflection partial success
            print "  trying contract...",
            x_s = shrink(x_c, x_worst)
            f_s = func(x_s)
            calculations += 1
            if f_s < f_worst: #shrink success
                x_next = x_s
                f_next = f_s
                print "ShrinK:", 
            else:
                x_next = x_worst
                f_next = f_worst
                needScale = True
        
        poly.append(x_next)
        vals.append(f_next)

        if needScale:
            for i in range(len(poly)):
                poly[i] = add(x_c, poly[i], 1-delta, delta)
            vals = map(func, poly)
            calculations += len(poly)
            print "Total scale:", 
        print poly2str(poly)
        print "------------------------"
        steps += 1
    #something found
    xbest = mean(poly)
    fbest = func(xbest)
    calculations += 1
    #print "Total calculations: %d, total steps: %d"%(calculations, steps)
    return xbest, fbest, steps, calculations

def rosenbrock(z):
    return (1-z[0])**2+100.0*(z[1]-z[0]**2)**2


def get_effectiveness(abgd):
    alpha, beta, gamma, delta = abgd
    rndgen = random.Random()
    rndgen.seed(0)
    def rnd():
        return rndgen.uniform(-0.5,0.5)
        
    errors = 0
    calc_total = 0
    
    n_tries = 100
    for i in xrange(n_tries):
        pts=[[rnd(), rnd()] for i in range(3)]
        xbest, fbest, steps, calcs = nmead(pts, lambda z:(z[0]-1)**2+(z[1]-1)**2, alpha, beta, gamma, delta)
        print steps, xbest
        calc_total += calcs
        if abs(xbest[0]-1.0)>1e-3 or abs(xbest[1]-1.0)>1e-3:
            errors += 1
            
    return errors/float(n_tries), calc_total/float(n_tries)
def vec2str(v):
    return "["+", ".join(["%g"%x for x in v])+"]"
def poly2str(poly):
    return "{"+"; ".join(map(vec2str, poly))+"}"
def test():
    
    #print nmead([[1.,0.],[0.,1.],[-1.,0.]], lambda z: z[0]**2+z[1]**2)

    #print nmead([[.1,0.],[0.,.1],[-.1,0.]],
    #            rosenbrock)
    print nmead([[.1,0.],[0.,.1],[-.1,0.]],
        lambda z: (z[0]-1.5)**2+(z[1]-0.7)**2,
        gamma = 0.5
    )

test()
