from itertools import imap, count

def mul(m1,m2, lazy=False):
    if m1.width!=m2.height:
        raise ValueError, "Matrix sizes do not match"
    k_range = range(m1.width)
    def sumat(i,j):
        return sum([m1[i,k]*m2[k,j] for k in k_range])
    if not lazy:
        return mtx(m1.height, m2.width, sumat)
    else:
        return vmtx(m1.height, m2.width, sumat)

def trace(m):
    return sum(diag(m))

def diag(m):
    h,w = m.size()
    return [m[i,i] for i in range(min(h,w))]
    

def inv(m, one = 1.0, zero = 0.0):
    m = copy(m)
    h,w = m.size()
    if h!=w:
        raise ValueError, "Matrix must be square"
    
    im = copy(eye(h))
    
    def max_row_idx(col):
        maxidx = col
        maxval = abs(m.data[col][col])
        
        for row in range(col+1,h):
            v = abs(m.data[row][col])
            if v>maxval:
                maxval = v
                maxidx = row
        return maxidx
                
    
    for i in range(h): #column
        #find max in column
        row_max = max_row_idx(i)
##        print "==="
##        print m
##        print "col", i, "max row", row_max
        m.line_swap(i, row_max)
        im.line_swap(i, row_max)
##        print "swapped"
##        print m
        #normalize row
        A_ii = m[i,i]
        if not A_ii:
                    raise ZeroDivisionError, "Matrix can not be inverted"
                
        m.line_iscale(i, A_ii)
        im.line_iscale(i, A_ii)

        #make zeros
        for j in range(h):
            if j!=i:
                im.line_combine(i, -m[j,i], j)
                m.line_combine(i, -m[j,i], j)
##        print "Zeroized"
##        print m
##        print "=============="
    return im 
            

def add(*mts):
    def sumat(i,j):
        return sum([m[i,j] for m in mts])
    sz=mts[0].size()
    for m in mts:
        if m.size() != sz:
            raise ValueError, "matrix sizes not match"
    return mtx(sz[0],sz[1], sumat)
               
def copy(m):
    h,w = m.size()
    return mtx(h,w, m.get)

def const_values(v):
    def val(i,j):
        return v

def none_values(i,j):
    return None

class base_mtx:
    def __init__(self):
        pass
    
    def issq(self):
        w,h=self.size()
        return w==h
        
    def get(self, i,j):
        return self[i,j]
        
    def __str__(self):
        h,w = self.size()
        return "\n".join(["["+", ".join([str(self[i,j]) for j in range(w)])+"]" for i in range(h)])
    def __repr__(self):
        h,w = self.size()
        return "mtx(%d,%d,%s)"%(h,w,\
            "["+",".join(\
                ["["+",".join([repr(self[i,j]) for j in range(w)])+"]" for i in range(h)]\
            )+"]")
    
    def __add__(self, m):
        if isinstance(m, base_mtx):
            return add(self, m)
        else:
            h,w = self.size()
            return add(self, ones(h,w,m))
    def __radd__(self, m):
        return self.__add__(m)

    def __rmul__(self, m):
        if isinstance(m, base_mtx):
            return mul(m,self)
        else:
            h,w = self.size()
            return mtx(h,w,lambda i,j: self[i,j]*m)

    def col(self,i, lazy = False):
        h,w = self.size()    
        if not lazy:
            return [self[j,i] for j in range(h)]
        else:
            return _lazy_list(lambda j: self[j,i], h)
            
    
class _lazy_list:
    "List, based on function"
    def __init__(self, func, length = None):
        self.func = func
        self.length = length
    def __getitem__(self, idx):
        return self.func(idx)
    def __iter__(self):
        return imap(lambda idx:self.func(idx), count())
    def __len__(self):
        return self.length

def mxmap(func, m, lazy = False):
    mtx_type = mtx if not lazy else vmtx
    h,w = m.size()
    return mtx_type(h,w,lambda i,j:func(m[i,j]))
    
    
class mtx (base_mtx):

    def __init__(self, w, h, values=none_values):
        if hasattr(values, "__call__"):
            self.data=[[values(i,j) for j in xrange(w)] for i in xrange(h)]
        elif hasattr(values, "__getitem__"):
            self.data = values
        else:
            raise ValueError, "Values must be list or function"
        self.width = w
        self.height = h

    def __getitem__(self, (i, j)):
        return self.data[i][j]
    def __setitem__(self,(i,j),v):
        self.data[i][j]=v

    def size(self):
        return self.height, self.width
        
    def __mul__(self, m):
        if isinstance(m, base_mtx):
            return mul(self, m)
        else:
            h,w = self.size()
            return mtx(h,w,lambda i,j: self[i,j]*m)

    def line_combine(self, i,a,j):
            "Replace i'th line with a[i]*a + a[i]"
            Ai = self.data[i]
            Aj = self.data[j]
            self.data[j] = [Ai*a + Aj for Ai,Aj in zip(self.data[i],self.data[j])]
            
    def line_scale(self, i, k):
            self.data[i] = [x* k for x in self.data[i]]
    def line_iscale(self, i, k):
            self.data[i] = [x/k for x in self.data[i]]
            
    def line_swap(self, i,j):
            if i==j: return
            self.data[i], self.data[j] = self.data[j], self.data[i]

def eye(h, w=None, one=1,zero=0):
    if w==None: w=h
    return vmtx(h, w, lambda i,j: one if i==j else zero)


def zeros(h,w=None, zero=0):
    if w==None: w=h
    return vmtx(h,w,lambda i,j:zero)

def ones(h,w=None, one=1):
    return zeros(h,w,one)

class vmtx(base_mtx):
    def __init__(self, w, h, values=none_values):
        self.values = values        
        self.width = w
        self.height = h

    def __getitem__(self, (i, j)):
        return self.values(i,j)
    
    def size(self):
        return self.height, self.width

class transposed(base_mtx):
    def __init__(self, m):
        self.original = m
    def __getitem__(self, idx):
        return self.original.__getitem__(idx)
    def __setitem__(self, idx, v):
        self.original.__setitem__(idx, v)
    def size(self):
        return self.original.size()[::-1]

class minor(base_mtx):
    def __init__(self, m, cols, rows):
        self.cols=cols
        self.rows=rows
        self.orig = m
    def size(self):
        return len(rows),len(cols)
    def __getitem__(self, (i,j)):
        return self.orig[self.rows[i],self.cols[j]]
    def __setitem__(self, (i,j), v):
        self.orig[self.rows[i],self.cols[j]] = v

def det(m):
    w,h = m.size()
    if w!=h:
        raise ValueError, "matrix must be square"

    if w == 1:
        return m[0,0]
    
#class hjoin(base_mtx):
    #def __init__(self, *matrices):
        #self.mts = matrices
        #self.hrefs = list() #references to the columns
        
        #if len(mts)==0:
            #self.height = 0
            #self.width = 0
        #else:
            #h,w=matrices[0].size()
            #for m in matrices:
                #h1, w1 = m.size()
                #if h1 != h:
                    #raise ValueError, "Can not hjoin matrices: sizes are wrong"
                #w+=w1
            #self.height = h
            #self.width = w
    #def size(self):
        #return self.height, self.width
    #def __getitem__(self, (i,j)):
        #for m in self.matrices:
            #w,h = m.size()
            #if j<w:
                #return m[i,j]
            #j -= w
        #raise IndexError, (i, "Index too big")
    #def __setitem__(self, (i,j), v):
        #for m in self.matrices:
            #w,h = m.size()
            #if j<w:
                #m[i,j] = v
                #return
            #j -= w
        #raise IndexError, (i, "Index too big")
  
from rational import rat    
m=mtx(6,6,values=lambda i,j:rat(i+1,j+2) if i<=j else rat(0))
print inv(m,zero=rat(0), one=rat(1))
