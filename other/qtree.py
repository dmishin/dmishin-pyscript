


def same(lst):
    v0=lst[0]
    for v in lst[1:]:
        if v != v0:
            return False
    return True

class block:
    def __init__(self, lvl, fill=None):
        self.cells=[fill]*4
        self.level = lvl
        self.parent = None
        
    def get(self, i, j):
        return self.cells[i*2+j]

    def set(self, i,j,v):
        self.cells[i*2+j]=v
        if isinstance(v, block):
            v.parent = self

    def isflat(self):
        if same(self.cells):
            return True, self.cells[0]
        else:
            return False, None



def qt_get(root, x, y, root_size):
    if x<0 or x>=root_size:
        raise IndexError("Index x=%d is out of cell"%x)
    if y<0 or y>=root_size:
        raise IndexError("Index y=%d is out of cell"%x)

    sub_size = root_size/2

    j = x/sub_size
    i = y/sub_size

    if sub_size == 1:
        return root.get(i,j)
    else:
        v=root.get(i,j)
        if isinstance(v, block):
            return qt_get(v, x%sub_size, y%sub_size, sub_size)
        else:
            return v


def qt_set(root, x, y, root_size, v):
    if x<0 or x>=root_size:
        raise IndexError("Index x=%d is out of cell"%x)
    if y<0 or y>=root_size:
        raise IndexError("Index y=%d is out of cell"%x)

    sub_size = root_size/2

    j = x/sub_size
    i = y/sub_size

    if sub_size == 1:
        root.set(i,j, v)
    else: #not 1
        v1=root.get(i,j)
        if not isinstance(v1, block):#need to create sub-cell
            if v == v1:
                return #no ned to set anything
            new_block = block(root.level+1, v1)
            root.set(i,j, new_block)
            v1 = new_block
            
        qt_set(v1, x%sub_size, y%sub_size, sub_size, v)
        #after setting, checking, whether block can be convolved

        can_convolve, value = v1.isflat()
        if can_convolve:
            root.set(i,j, value)

def qt_show(root, size):
    for i in range(size):
        ln=""
        for j in range(size):
            if qt_get(root, j, i, size):
                ln+="[]"
            else:
                ln+="--"
        print ln

def qt_showtree(root, lvl=0):

    def a(i,j):
        if isinstance(root.get(i,j), block):
            return "##"
        else:
            return ("%s  "%root.get(i,j))[:2]
        
    print "  "*lvl,"[%s %s]"%(a(0,0),a(0,1))
    print "  "*lvl,"[%s %s]"%(a(1,0),a(1,1))
    for c in root.cells:
        if isinstance(c, block):
            qt_showtree(c, lvl+1)
    


    

root=block(1,0)
size=1024

qt_set(root, 3,7, size, 1)
qt_set(root, 3,8, size, 1)
qt_set(root, 300,10, size, 1)
#qt_show(root, size)
qt_showtree(root)
    

def 
