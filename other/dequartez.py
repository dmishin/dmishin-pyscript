class deqext:
    "Dequartez extension of a field"

    def __init__(self,a,b,c,q):
        self.a = a
        self.b = b
        self.c = c
        self.q = q

    def __str__(self):
        print "[(%s+%s*sqrt(%s))/%s]"%(self.a, aelf.b, self.q, self.c)

    def __add__(self, b):
        if isinstance(b,deqext):
            if b.q != self.q:
                raise ValueError("Extensions of diferent fields!")
            else return deqext(self.a+b.a, self.b+b.b, self.q)
        elif isinstance(b, (int, long)):
            return deqext(self.a+b, self.b, self.q)

    def __sub__(self, b):
        if isinstance(b,deqext):
            if b.q != self.q:
                raise ValueError("Extensions of diferent fields!")
            else return deqext(self.a-b.a, self.b-b.b, self.q)
        elif isinstance(b, (int, long)):
            return deqext(self.a-b, self.b, self.q)

    def __mul__(self, x):
        if isinstance(x, (int, long)):
            return deqext(self.a*x, self.b*x, self.q)
        elif isinstance(x, deqext):
            if self.q != x.q:
                raise ValueError("Different extensions!")
            a1 = self.a*x.a+self.b*x.b*self.q
            b1 = self.a*x.b + self.b*x.a
            return deqext(a1,b1,self.q)
    def det(self):
        return self.a*self.a - self.b*self.b*self.q
    def conj(self):
        return deqext(self.a, -self.b, self.q)
    def __div__(self, x):
        if isinstance(x,(int, long)):
            return 
        return self*x.conj() 
