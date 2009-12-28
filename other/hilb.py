import math
import PIL.Image
import numpy
import psyco
psyco.full()

def save_array(float_array, fname):
    data = numpy.array( cap(255)(float_array), dtype = numpy.int8)
    img = PIL.Image.fromarray( data, mode="L")
    img.save(fname)

def h2d(x,y):
    if x<0 or x>1 or y<0 or y>1:
        raise ValueError, "Range"
    h=0.25
    s=0
    while h>1e-16:
        p1=x<0.5
        p2=y<0.5
        if p1:
            if p2: #1
                x1=2*y
                y1=2*x
                #s+=0
            else: #2
                x1=2*x
                y1=2*y-1
                s+=h
        else:
            if p2: #4
                x1=1-2*y
                y1=2-2*x
                s+=3*h
            else: #3
                x1=2*x-1
                y1=2*y-1
                s+=2*h
        x=x1
        y=y1
        h *= 0.25
    return s
    

def h2d_bis(x,y):
    if x<0 or x>1 or y<0 or y>1:
        raise ValueError, "Range"
    h=0.25
    s=0
    while h>1e-16:
        p1=x<0.5
        p2=y<0.5
        if p1:
            if p2: #1
                x1=2*y
                y1=2*x
                #s+=0
            else: #2
                x1=1-2*x
                y1=2*y-1
                s+=h
        else:
            if p2: #4
                x1=1-2*y
                y1=2-2*x
                s+=3*h
            else: #3
                x1=2*x-1
                y1=2*y-1
                s+=2*h
        x=x1
        y=y1
        h *= 0.25
    return s
def sierp(x,y):
    if x<0 or x>1 or y<0 or y>1:
        raise ValueError, "Range"
    h=0.25
    s=0
    while h>1e-16:
        x = x*2-1
        y = y*2-1
        if x<0:
            if y<0: 
                pass
            else:
                s+=h*1
        else:
            if y<0:
                s+=3*h
            else:
                s+=2*h
        x=abs(x)
        y=abs(y)
        h *= 0.25
    return s

def peano2d(x,y):
    if x<0 or x>1 or y<0 or y>1:
        raise ValueError, "Range"
    s=0.0
    h=1.0/9
    while h>1e-14:
        x *= 3
        y *= 3 
        if x<1:
            if y<1:
                s+=0
            elif y<2:
                x=1-x
                y=y-1
                s+=h
            else:
                y=y-2
                s+=h*2
        elif x<2:
            x-=1
            if y<1:
                y=1-y
                s+=5*h
            elif y<2:
                x=1-x
                y=2-y
                s+=4*h
            else:
                y=y-2
                y=1-y
                s+=3*h
        else:
            x-=2
            if y<1:
                s+=6*h
            elif y<2:
                y=y-1
                x=1-x
                s+=7*h
            else:
                y-=2
                s+=8*h
        h/=9
    return s
def z2d(x,y):
    "Z-curve"
    if x<0 or x>1 or y<0 or y>1:
        raise ValueError, "Range"
    s=0.0
    h=0.5
    while h>1e-14:
        x+=x
        if x>=1:
            s+=h
            x-=1
        h*=0.5
        
        y+=y
        if y>=1:
            s+=h
            y-=1
        h*=0.5
        
    return s


def rem(x):
    return x-math.floor(x)
def saw(x):
    return 1-abs(1-2*rem(x*0.5))
def sqsaw(x):
    return D[int(rem(x)*lD)]
def isin(x):
    return math.sin(x*math.pi*2.0)*0.5+0.5

def cap(v):
    return numpy.vectorize(lambda x: max(min(x,v),0))

def generate_image(W, H, periods, path, func, postprocess=(lambda arr:numpy.log(arr)*20+30), curve=h2d):
    iw = 1.0/W
    ih = 1.0/H

    print "Generating image %dx%d"%(W,H)
    odata=numpy.zeros((H,W))
    percent = 0
    for x in xrange(W):
        xx=x*iw+0.5*iw
        p = (x*100)/W
        if p > percent:
            print "Done: %d%%"%p
            percent = p
        for y in xrange(H):
            yy=y*ih+0.5*ih
            s=curve(xx,yy)
            
            v=func(s*periods)*255
            odata[x,y]=v

    
    #fft the data
    print "postprocessing..."
    ldata = postprocess(odata)
    #ldata = postprocess(numpy.abs(numpy.fft.fft2(odata)))
    print "Saving"

    save_array(ldata, path+"out-fft.png")
    print "Done"

def auto_normalize(arr, percentile = 0.1, expand_ratio = 1.1, scale=255):
    h,w = arr.shape
    ia0,ia1 = int(w*percentile), int(w*(1.0-percentile))
    
    a0=[]
    a1=[]
    for line in arr:
        sorted = line.copy()
        sorted.sort()
        a0.append( sorted[ia0] )
        a1.append( sorted[ia1] )
    
    a0 = numpy.median(a0)
    a1 = numpy.median(a1)
    #expand the diapason
    a_mean = 0.5*(a0+a1)
    a_diff = 0.5*(a1-a0)
    if a_diff == 0:
        print "Warning: can't autoscale: exact zero"
        return arr
    a0 = a_mean - expand_ratio*a_diff
    a1 = a_mean + expand_ratio*a_diff
    print "Autoscale from %g..%g"%(a0, a1)
    #scale the data
    return (arr-a0)*(scale/(a1-a0))
    
def postprocess(arr):
    return arr
    
def postprocess_log(arr):
    return numpy.log(arr)*20+30

def comb(*funcs):
    def cfunc(x):
        for f in funcs:
            x=f(x)
        return x
    return cfunc

def abs_fft(arr):
    return numpy.abs(numpy.fft.fft2(arr))


outdir = "/home/dim/tmp/one-period-hilb"
#W=1024*2
#NUM_PERIODS = (W*H/8)*2**1

W=1024
H=W
NUM_PERIODS = 2**(20)/16

#abs_fft, numpy.log,
generate_image(W,H, NUM_PERIODS, outdir, rem, comb( auto_normalize), curve = sierp)
#print z2d(0.3,0.71)

