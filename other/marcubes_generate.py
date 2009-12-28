#Generate mesh, using marching cubes algorithm.
from math import sqrt

treshold = 1.0


def value_func(x,y,z):
    v=5.0-sqrt((x-10)**2+(y-10)**2+(z-10)**2)
    return v

def trsh(v):
    return v>treshold

def mid(x1, x2):
    # (1-t)*x1+t*x2 = treshold
    # t = (treshold - x1)/(x2-x1)
    return (treshold - x1)/(x2-x1)
    
def cube_coords():
    "Generator cur cube coordinates"
    for x in range(2):
        for y in range(2):
            for z in range(2):
                yield x,y,z

cube_coords=tuple(cube_coords())

def read_cube(x,y,z):
    "Read cube data as list of values"
    return [value_func(x+dx, y+dy, z+dz) for dx,dy,dz in cube_coords]

    
def process_plane(x_range, y_range, z, hrz_ribs, vrt_ribs, top_ribs):
    """x_range: (x0, x1) range for x cooord
    y_range: (y0, y1) range for y coord
    z - height of a plane
    
    hrz_ribs: dict: (x0,y0,x1,y1)->idx  maps horizontal(lower) rib coordinate to the index of the vertex, located in that rib
    vrt_ribs: dict: (x,y)->idx  maps vertical rib coordinates to vertex indices
    top_ribs: dict: (x0, y0, x1,y1)->idx same as hrz_ribs, but maps top-level rib planes (to be generated)
    """
    
    print "Processing plane z=%d, %d<=x<=%d; %d<=y<=%d"%(z, x_range[0],x_range[1], y_range[0],y_range[1])
    
    
    def get_rib_vertex(x1,y1,z1,x2,y2,z2):
        "Returns the index of a vertex, connecting two given nodes of grid"
        if z1==z2: #horizontal rib
            if z1==z: #lower rib
                pass
            elif z1==z+1: #upper rib
                pass
            else:
                raise Exception("Tried to get a rib for strange Z index %d"%z1)
        else: #vertical rib
            assert(abs(z2-z1)==1)
            
        
    
    
    for x in range(*x_range):
        for y in range(*y_range):
            #each cube vertex is:
            #(x,y,z), (z+1,y,z), (x,y+1,z), ..., (z+1, y+1, z+1)
            cube=read_cube(x,y,z)
            
    
