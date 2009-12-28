#stereo


def xor(a,b):
    return (a and b) or (not a and not b)


def z0_intersect(p1, p2):
    "Intersection of a line connecting points p1 and p2, with plane z=0"

    z0 = p1[2]
    z1 = p2[2]
    if z0==z1:
        p=0.5
    else:
        p = -z0/(z1-z0)
    if p>=0.0 and p<=1.0:
        q=1.0 - p
        return (p1[0]*q+p2[0]*p, p1[1]*q+p2[1]*p)

def triangle_intersect(p1, p2, p3):
    p1_lower = p1[2]<0.0

    i1 = 
    if xor(p1_lower, p2[2]<0.0):
        return z0_intersect(p1,p2)
    if xor(p1_lower, p3[2]<0.0):
        return z0_intersect(p1,p2)
    
def build_stereogram_L2R_func(vertices, triangles, d_picture, x0_picture,
                              x1_picture, phi, d_eye, approx_steps = 1024):
    "vertices: array of 3d vertices 4xN  \n"
    "triangles: array of triples (int, int, int)\n"
    "d_picture - distance to picture plane \n
    "x?_picture - horizontal postion of a picture plane\n"
    "d_eye - distance from center to the eye\n"
    "approx_steps - size of approximation array generated"

    
    #for each triangle, find its intersection with plane z=0
    
