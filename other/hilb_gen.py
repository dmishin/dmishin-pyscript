import PIL.Image
import numpy
import psyco
psyco.full()

def gen( lvl ):
	if lvl == 0:
		for c in [1,0,3]: yield c
		return
	if lvl%2==1:
		k=[0,1,2]
	else:
		k = [1,0,3]
		
	for a in gen(lvl-1):
		yield a
	yield k[0]
	for a in gen(lvl-1):
		yield (5-a)%4
	yield k[1]
	for a in gen(lvl-1):
		yield (5-a)%4
	yield k[2]
	for a in gen(lvl-1):
		yield (a+2)%4

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
test2(10,"c:\\dmishin\\temp\\out.png","c:\\dmishin\\temp\\out-fft.png")

    
