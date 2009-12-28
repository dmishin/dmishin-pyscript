import PIL.Image
import numpy
import psyco
import os
import math
psyco.full()

class TFM:
    def __init__(self):
        self.mtx = [0.0]*6
    def set(self, tfm):
        if isinstance(tfm, TFM):
            self.mtx[:] = tfm.mtx[:]
        else:
            self.mtx[:] = tfm.mtx[:]
            
    def mul_left(self, tfm):
        pass

def shift(dx, dy):
    return numpy.array([[1,0,dx],[0,1,dy],[0,0,1]])
def rot_90():
    return numpy.array([[0,1,0],[-1,0,0],[0,0,1]])
def rot(alpha):
    s = math.sin(alpha)
    c = math.cos(alpha)
    return numpy.array([[c,s,0],[-s,c,0],[0,0,1]])


def rot_270():
    return numpy.array([[0,-1,0],[1,0,0],[0,0,1]])
def rot_180():
    return numpy.array([[-1,0,0],[0,-1,0],[0,0,1]])
def flip_xy():
    return numpy.array([[0,1,0],[1,0,0],[0,0,1]])
def mirror_x():
    return numpy.array([[-1,0,0],[0,1,0],[0,0,1]])
def mdot(*mts):
    return reduce(numpy.dot, mts)
def eye():
    return numpy.eye(3)
def scale(k):
    return numpy.diag([k,k,1])
    
def vec(x,y):
    return numpy.array((x,y,1.0))

def convert(x,y, is_in, descend_matrices1, descend_matrices2, steps=64):
    def isin(v):
        return is_in(v[0],v[1])
        
    v = vec(x,y)

    ascend_matrices = map(numpy.linalg.inv, descend_matrices2)
    
    inv_mtx = eye()
    total_mtx = eye()
    
    for i in xrange(steps):
        found = False
        for mtx_desc, mtx_asc in zip(descend_matrices1, ascend_matrices):
            #try to descend
            v1 = numpy.dot(mtx_desc, v)
            if isin(v1): #descend successful
                v = v1
                total_mtx = numpy.dot(mtx_desc, total_mtx)
                inv_mtx = numpy.dot(inv_mtx, mtx_asc)
                found = True
                break
        if not found:
            raise ValueError, "Failed to descend at point %s"%v
    
    rval = numpy.dot(inv_mtx, vec(0,0))
    return rval[0], rval[1]

def test_convert():
    ofile = "out1.png"
    def in_square(x,y):
        return x>=0 and y>=0 and x<=1 and y<=1
    # 2 3
    # 0 1
    
    #desc_square = [mdot(scale(2), m) for m in [shift(0,0), shift(-0.5, 0), shift(0, -0.5), shift(-0.5, -0.5)]]
    desc_square = [mdot(scale(2), m) for m in [shift(0,0), mdot(rot_90(), shift(-1.0, 0)), mdot(rot_270(),shift(0, -1.0)), mdot(shift(-0.5, -0.5))]]
    desc_sphnx = [mdot(scale(2), m) for m in [
        mdot(mirror_x(), shift(-1,0)), 
        mdot(mirror_x(), shift(-2,0)),
        mdot(rot(120*math.pi/180), shift(-0.5, -math.sqrt(0.75)))  ,
        mdot(flip_x(), rot_180(), shift(-0.5, -math.sqrt(0.75)/2)) ]]
    
    def in_sphinx(x,y):
        if y<0: return False
        if 
    
    def in_trimino(x,y):
        return abs(x-0.5)<=0.5 and abs(y-0.5)<=0.5 and not(x>0.5 and y>0.5)
    
    desc_trimino = [mdot(scale(2), m) for m in [shift(0,0), mdot(rot_90(), shift(-1.0, 0)), mdot(rot_270(),shift(0, -1.0)), mdot(shift(-0.25, -0.25))]]

    def cvt(x,y):
        try:
            return convert(x,y, in_trimino, desc_trimino, desc_square,10)
            #return convert(x,y, in_square, desc_square, desc_trimino,10)
        except ValueError:
            return 0,0
            
            
    print cvt(0.003,0.01) #must print same as given

    #base_img = PIL.Image.open(r"c:\dmishin\vyh1.png")
    #base_img.load()
    #bW,bH = base_img.size

    W=1024
    H=W
    print "Generating image %dx%d"%(W,H)
    odata=numpy.zeros((H,W,3), numpy.uint8)
    percent = 0
    for X in xrange(W):
        n_percent = (X*100)/W
        if n_percent - percent >= 1:
            percent = n_percent
            print "Done: %d%%"%percent
        x = X/float(W)
        for Y in xrange(H):
            y=Y/float(H)
            x1,y1 = cvt(x,y)
            #odata[Y,X,:] = base_img.getpixel((int(x1*(bW-1)), int(y1*(bH-1))))
            #int(255*x1)
            
            odata[Y,X,1] = int(255*y1)
            odata[Y,X,2] = int(255*x1)
            odata[Y,X,0] = int(255*y1)

    img = PIL.Image.fromarray( odata, mode="RGB")
    img.save(ofile)
    os.system("start %s"%ofile)

    
test_convert()
    
    
def test(n, fname):
    W=2**n*2
    H=2**n*2
    print "Generating image %dx%d"%(W,H)
    odata=numpy.zeros((H,W), numpy.uint8)
    dirs=[(1,0),(0,1),(-1,0),(0,-1)]
    x=0
    y=0
    for d in gen(n-1):
        dx, dy = dirs[d]
        odata[x,y]=255
        odata[x+dx,y+dy]=255
        x+=dx*2
        y+=dy*2
    img = PIL.Image.fromarray( odata, mode="L")
    img.save(fname)
    return odata
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

def cap(v):
    return numpy.vectorize(lambda x: max(min(x,v),0))

def test2(n, fname, fname1):
    odata = test(n, fname)
    tfdata = auto_normalize(numpy.log(numpy.abs(numpy.fft.fft2(odata))))
    data = numpy.array( cap(255)(tfdata), dtype = numpy.int8)
    img = PIL.Image.fromarray( data, mode="L")
    img.save(fname1)


#test(8,"c:\\dmishin\\temp\\out.png")
#test2(10,"c:\\dmishin\\temp\\out.png","c:\\dmishin\\temp\\out-fft.png")

    
