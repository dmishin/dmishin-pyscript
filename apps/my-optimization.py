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

def contracting_simplex1( initial_points, func, ke=2.0, ks=0.5, kgs=0.5, max_calculations=10000, eps=1e-5  ):
    "Variant 1: contract towards est point"
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
            new_points = [(slot[0]-x_best)*kgs+x_best for slot in slots[1:]]
            slots[1:] = [[point, cfunc( point )] for point in new_points]
            vals = [slot[1] for slot in slots]
            if max(vals)-min(vals) < eps:
                show( "found")
                show( slots)
                break
    slots.sort( key = lambda slot:slot[1] )
    return calculations[0], slots

def contracting_simplex2( initial_points, func, params=[1.0, 2.0, 0.5, 0.5], max_calculations=10000, eps=1e-5  ):
    "Variant 1: contract towards est point"
    alpha, beta, gamma, delta = params
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
        #Sort the items by the value
        slots.sort( key=lambda slot:slot[1] )
        steps += 1

        if calculations[0] > max_calculations:
            show( "Max calls reached")
            break
        
        xl,fl= slots[0] #best
        xg,fg = slots[-2] #next to the worst
        xh, fh = slots[-1]#forst 
        show( "Slots:", slots)
        #calculate center
        xc = sum( [slot[0] for slot in slots[:-1]] )/N
        show( "Center:", xc)
        
        #flip worst
        xr = (1+alpha)*xc-alpha*xh
        fr = cfunc( xr )
        
        if fr < fl: #good
            #expand
            xe = beta*xr+(1-beta)*xc
            fe = cfunc( xe )
            if fe < fr:
                #expand success
                show("expand success")
                slots[-1]=[xe, fe]
            else:
                show("expand fail, use flip")
                slots[-1] = [xr, fr]
        elif fr<fg: #not so good, but at least better than.
            #use it
            show("use flip simple")
            slots[-1] = [xr, fr]
        else: #too bad (worse that fg )
            if fr < fh: #at least, not worse than xh?
                #swap xh and xr
                xr,xh, fr, fh = xh, xr, fh, fr
            #now xr is worse than xh in any case
            #try contract
            xs = gamma*xh+(1-gamma)*xc
            fs = cfunc( xs )
            
            if fs<fh:
                #contract success, use it
                show("shrink success, use it")
                slots[-1]=[xs, fs]
            else:
                show("shrink fail, global contract")
                #contract fail.
                #last resort: global shrink
                for i in xrange(1,N+1):
                    xi = slots[i][0]*delta+(1-delta)*xl
                    fi = cfunc( xi )
                    slots[i] = [xi, fi]
    
        #check exit condition
        vals=[slot[1] for slot in slots]
        if max(vals)-min(vals)<eps:
            break

    return calculations[0], slots
              

def fun2( v ):
    x,y=v
    return ((x-1)**2+y**2)*((x+1)**2+y**2)
verbose = True
print contracting_simplex2( 
    map(numpy.array, [
            [0, 1],
            [0, -1],
            [0.1, 0.3]]),
    fun2,
    params=[1.05, 2.2, 0.3, 0.6],
    eps = 1e-4)
raise ValueError, "stop"
        

def rozen(v):
    x,y=v
    return (1-x)**2+100*(y-x*x)**2
        
# print contracting_simplex(
#     map(numpy.array, [ [0,0], [0,1], [1,0] ] ),
#     lambda v: (v[0]-5)**2+2*(v[1]-3)**4
#     )


def test_suite( inital_points_set, func, eps, answer, max_calculations=10000, answer_eps=1e-2):
    
    def tester( params ):
        n_failures = 0
        n_calculations = 0
        for points in inital_points_set:
            calc, slots = contracting_simplex1( points, func, params=params, eps=eps, max_calculations = max_calculations)
            x, fmin = slots[0]
            d = numpy.max(numpy.abs(x-answer))
            if d>answer_eps:
                #Mistaks
                n_failures += 1
                n_calculations += calc
            else:
                n_calculations += calc
        return n_calculations, n_failures
    return tester

def test_suite2( inital_points_set, func, eps, answer, max_calculations=10000, answer_eps=1e-2):
    
    def tester( params ):
        n_failures = 0
        n_calculations = 0
        for points in inital_points_set:
            calc, slots = contracting_simplex2( points, func, params=params, eps=eps, max_calculations = max_calculations)
            x, fmin = slots[0]
            d = numpy.max(numpy.abs(x-answer))
            if d>answer_eps:
                #Mistaks
                n_failures += 1
                n_calculations += calc
            else:
                n_calculations += calc
        return n_calculations, n_failures
    return tester



#verbose=True
# steps, slots = contracting_simplex2(
#     map(numpy.array, [ [-1,0], [0,5], [1,0] ] ),
#     rozen,
#     eps=1e-14
#     )

# print "Steps:", steps
# print "Solution:", slots[0]
# print "Simplex:", slots
# #raise ValueError, "stop"

#make initial points
from random import random
n_sets = 1000
def rvec2():
    return numpy.array([random(), random()])*10-5

point_set = [[rvec2() for j in xrange(3)] for  i in xrange(n_sets)]

def fun_squares( v ):    
    return (v[0]-1)**2+(v[1]-1)**2
def fun_abs( v ):
    return abs(v[0]-1)+abs(v[1]-1)

tester = test_suite2( point_set, rozen, 1e-12, numpy.array([1,1]) ,max_calculations=400)

def ofun( params ):
    steps, errors = tester( params )
    return (steps+errors*10000)/float(len(point_set))

#trying to find optimal parameters for ofun
#Solutions:  EXPAND CONTRACT GLOBAL_CONTRACT
#Found solution:
#1000 random points in +-5 range average steps count: 280
# func: rozen
#[[array([ 1.82030691,  0.47676445,  0.29707136]), 280.33199999999999],
#Variant1:
#120 [[array([ 1.91984403,  0.45231335,  0.37215738]), 248.053],
#82  [[array([ 1.8466898 ,  0.52638334,  0.37307314]), 241.99199999999999]
#116 [[array([ 1.85394995,  0.52267316,  0.3766231 ]), 245.21799999999999]

#Solution for squares: (119 calculations)
#50 [[array([ 1.65758375,  0.5715003 ,  0.22908405]), 119.25], [array([ 1.65735041,  0.57159582,  0.22894623]), 119.306], [array([ 1.65727594,  0.57165239,  0.22892833]), 119.63800000000001], [array([ 1.65727014,  0.57169039,  0.22896053]), 119.65600000000001]]
print "Optimizing parameters"

#Solution for squares, with modified method 1 (contract towards best point): 
# 105 calcluations, seems better
#37 [[array([ 1.50395754,  0.790558  ,  0.29451553]), 104.958]
#28 [[array([ 1.67707731,  0.64722404,  0.32430135]), 104.133]
#48 [[array([ 1.59205129,  0.67690261,  0.2689539 ]), 104.522],
#Seems that variability is rather high


#Modified method 2 (from wiki): seems to be much better
# fucntion: rozen
# alpha: 1
# 86 [[array([ 2.26602031,  0.36498643,  0.63100675]), 146.62299999999999]  #seems significantly better than my variant
# 30(ver2) [[array([ 2.17580357,  0.35562111,  0.53142468]), 145.071]  #seems rather stable
# 31 [[array([  2.20029552,  0.36179379,  0.59620686])

#Now try to optimize all 4 parameters (alpha: free)
# attemt1 (failed to save: alpha ~1.05)  (twice as better as my variant!)
#46 [[array([ 1.04790838,  2.18949427,  0.34636163,  0.57666648]), 148.31
#132 [[array([ 1.059264 ,  2.1508992,  0.3533248,  0.5397824]), 146.667]
#110 [[array([ 1.05065564,  2.19484943,  0.35800333,  0.56064853]), 148.06100

#same for the square func
#method: v2
#21 [[array([ 0.97511044,  2.12315183,  0.32567782,  0.44882097]), 77.122]
#20 [[array([ 0.98642397,  2.1298101 ,  0.33289832,  0.49989075]), 76.84399
#31 [[array([ 0.98158264,  2.2743252 ,  0.3349084 ,  0.64969353]), 76.778
#24 [[array([ 0.99196533,  2.25865748,  0.33434997,  0.62421759]), 77.07

#different initial pos (1.5, 1.1, 0.7, 0.3)
#37 [[array([ 0.99470678,  1.57100473,  0.32992636,  0.39790565]), 77.358
#61 [[array([ 0.97494508,  1.37388074,  0.33523377, -0.10256428]), 77.349  !!! funny
#65 [[array([ 1.00018651,  1.74326079,  0.30861197,  1.05231781]), 77.456  #ridiculous... square function is not sensible to the global contract? possibly.
#180 [[array([ 2.04652345,  1.11035161,  0.02426816,  0.49870365]), 48.0],  !!!!! significantly more effective!? 
 #use this value as start point, try 2: 

#trying the same (strange) inits with rozen
#49 [[array([  2.13384888e+00,   1.21700826e+00,   5.05957229e-04, 4.39888727e-01]), 61.305999999999997],
##!!!!!! it is really faster! NO. Lots of failures
#Now it is clear. close to zero values created false convergence
#123 [[array([ 1.32362842,  0.74890924,  0.05716207,  0.71546631]), 157.679], - not so clear though... value is not so bi
#35 [[array([ 2.10222466,  1.14145968,  0.00937996,  0.45152785]), 162.35
#with big batsu value:
#56 [[array([ 0.98732492,  2.68539877,  0.34964007, -0.26937981]), 147.965],
#115 [[array([ 1.02232913,  2.79133331,  0.37254107,  0.71378888]), 144.256],
#76 [[array([ 1.48445077,  0.81436007,  0.38821398, -1.30527821]), 184.364]
#176 [[array([ 0.9342499 ,  2.22449753,  0.32439144,  0.31579934]), 148.30 
#152 [[array([ 1.49549295,  0.66181571,  0.41582991,  0.49406306]), 184.1
#160 [[array([ 1.08052808,  5.37196411,  0.36682051, -0.35024956]), 151.898]


#print tester( numpy.array([ 1.87956539,  0.91016129,  0.06175785,  0.34229446] ))
#print tester( numpy.array([ 1.93388551,  1.01786417, -0.00351137,  0.36330087] ))
#print tester( numpy.array([ 2.14170167,  1.16202514, -0.00280121,  0.46095659] ))
#raise ValueError

#For mod func
# [[array([ 0.97209269,  2.23579502,  0.341861  ,  0.6000684 ]), 149.25325
# 158 [[array([ 0.98592633,  1.97139171,  0.35617417,  0.35820286]), 147.2
# 68  [[array([ 1.00860798,  2.21640423,  0.33800125,  0.60315894]), 147.72
def make_simplex( point, step ):
    n=len(point)
    def rvec():
        return numpy.array([x+(random()*2-1)*step for x in point])

    simplex=[ rvec() for i in xrange(n+1) ]
    return simplex

steps, slots = contracting_simplex2(
    make_simplex( [ 2.04652345,  1.11035161,  0.02426816,  0.49870365], 1.0 ),
    ofun,
    eps=0.5,
    max_calculations = 300,
    params=[ 1.05,  2.17,  0.35, 0.56]
    )

print steps, slots
