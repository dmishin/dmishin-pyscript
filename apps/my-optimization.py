import psyco
psyco.full()

import numpy


verbose=False

def show( *msgs ):
    if verbose:
        print " ".join(map(str, msgs))



def contracting_simplex( initial_points, func, ke=2.0, ks=0.5, kgs=0.5, max_calculations=10000, eps=1e-5  ):
    N = len(initial_points[0])
    assert( N+1 == len(initial_points) )

    steps = 0
    calculations = [0]
    def cfunc( v ): #accounting number of function calls
        calculations[0] += 1
        return func( v )

    #calculate values
    slots = [ [point, cfunc(point)] for point in initial_points ]
    

    while True:
        steps += 1
        if calculations[0] > max_calculations:
            show( "Max calls reached")
            break
        #Sort the items by the value
        slots.sort( key=lambda slot:slot[1] )
        x_worst, f_worst = slots[-1]
        x_best, f_best = slots[0]
        #now slots[0] is the least(best) and slots[-1] is the worst

        #calculate center
        xc = sum( [slot[0] for slot in slots[:-1]] )/N
        show( "Center:", xc)
        #reflect
        xr = xc*2-x_worst
        fr = cfunc( xr )

        if fr < f_best:
            #very good, expand
            xe = xc+ ke*(xr-xc)
            fe = cfunc(xe )
            if fe < fr:
                show( "expand success")
                slots[-1]=[xe, fe]
            else:
                show( "expand fail")
                slots[-1] = [xr, fr]
        elif fr < f_worst:
            #at least better
            #contract
            xs = xc + ks*(x_worst-xc)
            fs = cfunc( xs )
            if fs < fr:
                show( "contract...")
                slots[-1] = [xs, fs]
            else:
                show( "contract fail, use reflect")
                slots[-1] = [xr, fr]
        else: #too bad
            #global contract
            show( "global contract")
            new_points = [(slot[0]-xc)*kgs+xc for slot in slots]
            slots = [[point, cfunc( point )] for point in new_points]
            vals = [slot[1] for slot in slots]
            if max(vals)-min(vals) < eps:
                show( "found")
                show( slots)
                break
    slots.sort( key = lambda slot:slot[1] )
    return calculations[0], slots

def rozen(v):
    x,y=v
    return (1-x)**2+100*(y-x*x)**2
        
# print contracting_simplex(
#     map(numpy.array, [ [0,0], [0,1], [1,0] ] ),
#     lambda v: (v[0]-5)**2+2*(v[1]-3)**4
#     )


def test_suite( inital_points_set, func, eps, answer, max_calculations=10000, answer_eps=1e-2):
    
    def tester(ke, ks, kgs):
        n_failures = 0
        n_calculations = 0
        for points in inital_points_set:
            calc, slots = contracting_simplex( points, func, ke=ke, ks=ks, kgs=kgs, eps=eps, max_calculations = max_calculations)
            x, fmin = slots[0]
            d = numpy.max(numpy.abs(x-answer))
            if d>answer_eps:
                #Mistaks
                n_failures += 1
            else:
                n_calculations += calc
        return n_calculations, n_failures
    return tester




steps, slots = contracting_simplex(
    map(numpy.array, [ [-1,0], [0,5], [1,0] ] ),
    rozen,
    eps=1e-14
    )

print "Steps:", steps
print "Solution:", slots[0]


#make initial points
from random import random
n_sets = 1000
def rvec2():
    return numpy.array([random(), random()])*10-5

point_set = [[rvec2() for j in xrange(3)] for  i in xrange(n_sets)]
    
tester = test_suite( point_set, lambda v:(v[0]-1)**2+(v[1]-1)**2, 1e-12, numpy.array([1,1]))

def ofun( k_esgs):
    ke, ks, kgs = k_esgs
    steps, errors = tester( ke, ks, kgs )
    return steps/float(len(point_set)-errors)

#trying to find optimal parameters for ofun

#Found solution:
#1000 random points in +-5 range average steps count: 280
# func: rozen
#[[array([ 1.82030691,  0.47676445,  0.29707136]), 280.33199999999999], [array([ 1.82030704,  0.47676428,  0.29707133]), 280.334], [array([ 1.8203071 ,  0.47676463,  0.29707172]), 280.38799999999998], [array([ 1.82030656,  0.47676472,  0.29707128]), 280.40600000000001]]

print "Optimizing parameters"

steps, slots = contracting_simplex(
    map(numpy.array, 
        [ [2,0.5,0.5], [2.1, 0.5, 0.6], [2.2, 0.3,0.5], [1.9, 0.5, 0.4] ] ),
    ofun,
    eps=0.5,
    max_calculations = 300,
    ke = 1.82, ks=0.47, kgs = 0.297
    )

print steps, slots
