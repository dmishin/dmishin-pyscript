



def ispoint(z):
    return z[0]**2+z[1]**2+z[2]**2<0.5


def trng(z1,z2,z3):
    pass

def flip(z, axis):
    z1=z[:]
    z1[axis]=-z1
    return z1



def transp(lst, idx):
    n=len(lst)
    if n <= 1:
        return tuple(lst)
    i=idx%n
    l1 = lst[:]
    del l1[i]
    return (lst[i],)+transp(l1, idx/n)

transps=[transp([0,1,2],i) for i in range(6)]

def tfm(idx):
    tsp = idx%6
    sgn = idx/6
    def getsign(v):
        return 1 if v%2==0 else -1
    
    s=(getsign(sgn), getsign(sgn>>1), getsign(sgn>>2))
    
    t=transps[tsp]
    def func(z):
        return tuple( [z[t[i]]*s[i] for i in range(3)] )
    return func


pm_one=(-1,1)
def code2map(code):
    idx = 0
    m = dict()
    for i in pm_one:
        for j in pm_one:
            for k in pm_one:
                m[(i,j,k)]=code[idx]
                idx+=1
    return m

def map2code(m):
    def gen():
        for i in pm_one:
            for j in pm_one:
                for k in pm_one:
                    yield m[(i,j,k)]
    return tuple(gen())

def apply_tfm(code, tfm):
    m = code2map(code)
    m1=dict([(tfm(z),v) for z, v in m.items()])
    return map2code(m1)

def add_trng(code, triangles, trng_map):
    for i in range(6*8):
        t=tfm(i)
        c=apply_tfm( code, t)
        trs1=map(lambda tr: map(t, tr), triangles)
        trng_map[c] = trs1

tmap = dict()
add_trng((1,0,0,0, 0,0,0,0), [((1,0,0),(0,1,0),(0,2,0))], tmap)
