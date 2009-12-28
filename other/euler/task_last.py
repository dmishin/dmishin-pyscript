def sgn(x):
    if x>0 : return 1
    elif x<0: return -1
    return 0

def isright(A,B):
    "Return predicate(x,y), true for points right at libe AB"
    dx=B[0]-A[0]
    dy=B[1]-A[1]
    def rfun(x,y):
        return x*(-dy)+y*dx>=0
    return rfun

def isleft(A,B):
    return isright(B,A)
    
def is_convex_extension(A,B,C,D):
    "for part of N-gon, ..ABCD.. return predicate that is true, if part ABXCD is also convex"
    p1=isleft(A,B)
    p2=isright(B,C)
    p3=isleft(C,D)
    def pred(x,y):
        return p1(x,y) and p2(x,y) and p3(x,y)
    return pred

def area(points):
    s=0.0
    p0=points[-1]
    for p in points:
        dx=p[0]-p0[0]
        dy=p[1]-p0[1]
        s += -dy*p[0]+dx*p[1]
        p0=p
    return s*0.5



def enumerate_holes(points): 
    used=set()
    ngon=list()
    tried_holes=set()

    def abcd(idx):
        n=len(ngon)
        return ngon[(idx-1)%n],ngon[idx],ngon[(idx+1)%n],ngon[(idx+2)%n]

    def enum_req():
        "recursive enumeration"
        #enumerate all possible extensions
        ext_found = False
        for i_rib in xrange(len(ngon)):
            a,b,c,d=abcd(i_rib)
            pred = is_convex_extension(a,b,c,d)

            for idx, pnt in enumerate(points):
                if idx in used: continue
                if not pred(*pnt): continue
                #possible extension found
                #print "Extension found:",ngon, pnt
                ext_found = True
                #extend
                ngon.insert(i_rib+1, pnt)
                used.add(idx)
                #check, wheter it is already tried
                pts=tuple(used)
                if pts not in tried_holes:
                    tried_holes.add(pts)
                    #enumerate further extensions
                    for hole in enum_req():
                        yield hole
                #return to previous state
                del ngon[i_rib+1]
                used.remove(idx)
        
        if not ext_found:
            #it is maximal (unextendible) hole!
            yield ngon#, used
            
    
    for i in range(len(points)):
        used.add(i)
        ngon.append(points[i])
        print i
        for j in range(i+1, len(points)):
            used.add(j)
            ngon.append(points[j])

            for hole in enum_req():
                if len(hole)<=2: continue
                #print "hole:", hole
                yield hole
                
            del ngon[1]
            used.remove(j)
        del ngon[0]
        used.remove(i)

            
                        
def generator():
    x = 290797
    while True:
        x=(x*x)%50515093
        yield int(x%2000-1000)

def gpoints(N):
    points=list()
    itr=generator()
    while len(points)<N:
        x=itr.next()
        y=itr.next()
        points.append((x,y))
    return points
 

def solve(N):
    amax=None
    for ngon in enumerate_holes(gpoints(N)):
        a=area(ngon)
        if a<0:
            print "Oups!",a
#        if len(ngon)>2: print a, ngon
        if a > amax:
            amax=a
            #ngmax=ngon[:]
    return amax
