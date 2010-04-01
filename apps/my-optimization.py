

import numpy


verbose=False

def show( *msgs ):
    if verbose:
        print " ".join(map(str, msgs))



def contracting_simplex( initial_points, func, ke=2.0, ks=0.5, kgs=0.5, max_steps=10000, eps=1e-5 ):
    N = len(initial_points[0])
    assert( N+1 == len(initial_points) )

    steps = 0

    #calculate values
    slots = [ [point, func(point)] for point in initial_points ]

    while True:
        steps += 1
        if steps > max_steps:
            show( "Max steps reached")
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
        fr = func( xr )

        if fr < f_best:
            #very good, expand
            xe = xc+ ke*(xr-xc)
            fe = func(xe )
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
            fs = func( xs )
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
            slots = [[point, func( point )] for point in new_points]
            vals = [slot[1] for slot in slots]
            if max(vals)-min(vals) < eps:
                show( "found")
                show( slots)
                break
    slots.sort( key = lambda slot:slot[1] )
    return steps, slots

def rozen(v):
    x,y=v
    return (1-x)**2+100*(y-x*x)**2
        
# print contracting_simplex(
#     map(numpy.array, [ [0,0], [0,1], [1,0] ] ),
#     lambda v: (v[0]-5)**2+2*(v[1]-3)**4
#     )

steps, slots = contracting_simplex(
    map(numpy.array, [ [-1,0], [0,5], [1,0] ] ),
    rozen,
    eps=1e-12
    )

print "Steps:", steps
print "Solution:", slots[0]
