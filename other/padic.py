
class padic:
    def __init__(self, p, value):
        self.p = p
        self.init_integer(value)
        
    def init_integer(self, v):
        cnt = 0
        while v%self.p == 0:
            v = v/self.p
            cnt += 1
        self.k = cnt
        
