#negative curvature eometry flight

import numpy
from OpenGL.GL import *
from OpenGL.arrays import ArrayDatatype as ADT

from OpenGL.GLUT import *
#from OpenGLContext.arrays import array
import sys, time 
from math import sin,cos,sqrt,pi,sinh,cosh,exp
import tkFileDialog

from numpy import dot
from random import random, randint

class Smoother:
    def __init__(self, size, t0=0.05):
        self.u = numpy.zeros(size)
        self.x = numpy.zeros(size)
        self.k = t0

    def step(self, dt):
        k = pow(self.k, dt)
        self.x *= k
        self.x += self.u*(1.0-k)

    def get(self,idx):
        return self.x[idx]
    
    def set(self, idx, v):
        self.u[idx] = v

        


def mmove(x,y,z):
    "movement matrix"
    xx=x*x
    yy=y*y
    zz=z*z

    r2=xx+yy+zz

    if r2 == 0:
        return numpy.eye(4)
    
    r=sqrt(r2)

    cr=cosh(r)
    sr=sinh(r)

    xy=x*y
    xz=x*z
    yz=y*z
    
    rval = numpy.array([[xx,xy,xz,0],[xy,yy,yz,0],[xz,yz,zz,0],[0,0,0,r2]])*((cr-1)/r2) + \
         numpy.array([[0,0,0,x],[0,0,0,y],[0,0,0,z],[x,y,z,0]])*(sr/r) + \
         numpy.eye(4)

    return rval    

def rotij(i,j,phi):
    assert(i>=0 and i<3 and j>=0 and j<3)
    sp = sin(phi)
    cp = cos(phi)
    r=numpy.eye(4)
    r[i,i]=cp
    r[j,j]=cp
    r[i,j]=-sp
    r[j,i]=sp
    return r

def prepare_vertices(v):
    "Gets list (matrix) of vertices and prepares it for drawing"
    "it is not good just return x,y,z, because they can be too big (they grow exponentially while distance grows linearly"

    r=numpy.sqrt(v[0,:]**2+v[1,:]**2+v[2,:]**2)
    v[:,:] = v/r*numpy.arcsinh(r)*-30.0
    v[3]=1    


class Navigator:
    def __init__(self):
        self.view = numpy.eye(4) #view matrix

    def move(self, dx, dy, dz):
        self.view[:] = dot(mmove(-dx,-dy,-dz), self.view)

    def rotate(self,i,j,angle):
        self.view[:] = dot(rotij(i,j,-angle), self.view)


MSG_OPEN_GEOM = 1
MSG_EXIT = 99





# set up a light 
lightOnePosition = (0, 0, -10, 1)
lightOneColor = (0.99, 0.99, 0.99, 0.0) 


SM_STRAFE_LR=0
SM_STRAFE_UD=1
SM_MOVE_FB=2
SM_ROT_LR = 3
SM_ROT_UD = 4
SM_ROT_CENTER=5
NUM_SMOOTHERS = 6

class Main:
    def __init__(self,run=True, startup_heometry = None):
        self.title="Hyperbolic geometry"
        self.nav = Navigator()
        self.nav.move(0,0,-2)

        if startup_heometry:
            self.geom = startup_heometry
        else:
            print "Creating random geometry"
            self.geom = make_random_heometry()

        self.lastmouse=[None, None]
        self.last_time = None


        self.smoother = Smoother(NUM_SMOOTHERS)


        self.speed = 0.3
        self.rot_speed = 0.05
        
        if run:
            self.run()

        
    def idle(self):
        dt = self.get_dt()

        self.move(dt)
        
        glutPostRedisplay()
        
    def move(self, dt):
        "Calculate movement"
        self.smoother.step(dt)
        ds=dt*self.speed
        
        self.nav.move(
            self.smoother.get(SM_STRAFE_LR)*ds,\
            self.smoother.get(SM_STRAFE_UD)*ds,\
            self.smoother.get(SM_MOVE_FB)*ds)

        
        self.nav.rotate(0,2,self.smoother.get(SM_ROT_LR)*self.rot_speed)
        self.nav.rotate(1,2,self.smoother.get(SM_ROT_UD)*self.rot_speed)
        self.nav.rotate(0,1,self.smoother.get(SM_ROT_CENTER)*self.rot_speed)

        
    def visible(self, vis):
        if vis == GLUT_VISIBLE:
            glutIdleFunc(self.idle)
        else:
            glutIdleFunc(None)

    def get_dt(self):
        t = time.time()
        if self.last_time == None:
            rval = 0.0
        else:
            rval = t-self.last_time
        self.last_time = t
        return rval
    def create_menu(self):
        glutCreateMenu (self.menu_handler)
        glutAddMenuEntry ("Open geometry", MSG_OPEN_GEOM)
        glutAddMenuEntry ("Exit", MSG_EXIT)
        glutAttachMenu (GLUT_RIGHT_BUTTON)

    def init(self):
        self.init_controls()
        
        global glutDisplayFunc, glutMotionFunc, glutVisibilityFunc
        # initialize glut 
        glutInit(sys.argv)
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
        glutCreateWindow(self.title)
        glutDisplayFunc(self.render)
        glutMouseFunc(self.mousebtn)
        glutMotionFunc(self.mouse)
        glutVisibilityFunc(self.visible)

        #kbd controls
        glutKeyboardFunc(self.keyfunc)
        glutSpecialFunc(self.keyfunc)
        glutKeyboardUpFunc(self.keyfunc_up)
        glutSpecialUpFunc(self.keyfunc_up)
        
        
        # create popup menu */
        self.create_menu()

        # initialize GL */
        glClearDepth (1.0)
        glEnable (GL_DEPTH_TEST)
        glClearColor (0.0, 0.0, 1.0, 0.0)
        glShadeModel (GL_SMOOTH)

        glMatrixMode (GL_PROJECTION)
        # roughly, measured in centimeters */
        glFrustum (-9.0, 9.0, -9.0, 9.0, 50.0, 150.0)
        glMatrixMode(GL_MODELVIEW)

        # initialize lighting */
        glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition)
        glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor)
        glEnable (GL_LIGHT0)

        glEnable (GL_LIGHTING)
        glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE)
        glEnable (GL_COLOR_MATERIAL)

    def run(self):
        self.init()
        glutMainLoop ()
        self.close()

    def render(self):
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        
        self.geom.apply( self.nav.view )

        glLoadIdentity()					# Reset The View 
        self.geom.render()
        glutSwapBuffers ()

    def mousebtn(self, button, state, x,y):
        if button == GLUT_LEFT_BUTTON:
            if state == GLUT_DOWN:
                self.lastmouse[:]=(x,y)
            else:
                self.lastmouse[:]=(None, None)
        
    def mouse(self, x, y):
        if self.lastmouse[0]!=None:
            dx=x-self.lastmouse[0]
            dy=y-self.lastmouse[1]
            self.mouserel(dx,-dy)
        self.lastmouse[:]=(x,y)
                

    def mouserel(self, dx, dy):
        #self.nav.move(dx*0.001, dy*0.001,0)
        self.nav.rotate(0,2,-dx*0.001)
        self.nav.rotate(1,2,-dy*0.001)
        pass
    
    def close(self):
        print "CLosing"
        pass

    def init_controls(self):
        self.controls={
            "s": (SM_STRAFE_UD,  1.0),
            "w": (SM_STRAFE_UD, -1.0),
            "a": (SM_STRAFE_LR,  1.0),
            "d": (SM_STRAFE_LR, -1.0),
            "q" : (SM_ROT_CENTER,  1.0),
            "e" : (SM_ROT_CENTER,  -1.0),
            
            GLUT_KEY_LEFT : (SM_ROT_LR,  -0.02),
            GLUT_KEY_RIGHT: (SM_ROT_LR,  0.02),
            
            GLUT_KEY_UP   : (SM_MOVE_FB,  1.0),
            GLUT_KEY_DOWN : (SM_MOVE_FB,  -1.0),
            }

    def key2control(self, key):
        "Transforms keypress to control indevx and value"
        try:
            return self.controls[key]
        except KeyError:
            return None,None
        
    def keyfunc_up(self,key,x,y):
        ctl, val = self.key2control(key)
        if ctl!= None:
            self.smoother.set(ctl, 0)
        
    def keyfunc(self,key,x,y):
        ctl, val = self.key2control(key)
        if ctl!= None:
            self.smoother.set(ctl, val)

    def menu_handler (self, msg):
        if msg == MSG_EXIT:
            sys.exit(0)
        if msg == MSG_OPEN_GEOM:
            #loading geometry
            filename = tkFileDialog.askopenfilename()
            if filename:
                print "Opening file:",filename
                try:
                    self.geom = load_geometry(filename)
                except Exception,e:
                    print "Failed to load geometry", e
                
                
        return 0

        

        


class HeomObject:
    def __init__(self):
        self.transform = numpy.eye(4)
        self.vertices = None
        self.triangles = None
        self.rendered_vertices = None
        self.children=[]
    
    def set_vertices(self,  verts ):
        "Set vertices array. argument must be a list of floating-point quadruples"
        
        self.vertices = numpy.array( verts ).T #transposed, because vertices must be columns
        dimension, num_vertices = self.vertices.shape
        self.num_vertices = num_vertices
        if dimension != 4: raise ValueError("DImension must be 4")
        self.rendered_vertices = numpy.zeros( self.vertices.shape )
    
    def set_triangles(self, triangles ):
        self.triangles = triangles
        for a,b,c in self.triangles:
            assert(a>=0)
            assert(b>=0)
            assert(c>=0)
    
    def apply(self, view_transform):
        "Apply object transform and view transform to the vertices array"
        local_tfm = dot(view_transform, self.transform)
        if self.vertices != None:
            self.rendered_vertices[:] = dot( local_tfm, self.vertices)
        
        for child in self.children:
            child.apply(local_tfm)
        
    def render(self):
        if self.rendered_vertices != None and self.triangles != None:
            draw_transformed_points(self.rendered_vertices, self.triangles)      #draw the vertices, if any
        for child in self.children:
            child.render()
    
    def add_tfm(self, tfm_matrix):
        self.transform[:] = dot(tfm_matrix, self.transform)


def complete_point(x,y,z):
    t = sqrt(1+x**2+y**2+z**2)
    return (x,y,z,t)
    

def draw_transformed_points (verts, triangles):
    def draw_vertex(idx):
        glVertex3f(verts[0,idx],verts[1,idx],verts[2,idx])
    
    prepare_vertices(verts)
    #################
    glBegin(GL_TRIANGLES)
    for a,b,c in triangles:
        draw_vertex(a)
        draw_vertex(b)
        draw_vertex(c)
    glEnd()
    #################
    
def make_random_heometry():
    h = HeomObject()
    vertices = [complete_point(random()*2-1, random()*2-1, random()*2-1) for i in xrange(100)]
    triangles = [(randint(0, len(vertices)-1),randint(0, len(vertices)-1), randint(0, len(vertices)-1)) for i in xrange(100)]
    h.set_vertices( vertices )
    h.set_triangles( triangles )
    return h
            




########################
#XML loading
import pull_xml_parser as PXML
import undoiter
def load_geometry( xmlfile ):
    if isinstance(xmlfile, basestring):
        xmlfile = open(xmlfile, "r")
        
    parser = PXML.PullParser( xmlfile )
    xml_iter = undoiter.undoiter( iter(parser) )
    
    def parse_scene(xml_iter):
        "Parse the scene node"
        item = xml_iter.next()
        if item.kind != PXML.TAG_OPEN or item.tag!= "scene":
            raise ValueError, "Unexpected tag:%s"%item
        scene_name = item.attrs.get("name","unnamed")
        objects = parse_objects_tree( xml_iter )
        item = xml_iter.next()
        if item.kind != PXML.TAG_CLOSE or item.tag!= "scene":
            raise ValueError, "Unexpected tag:%s"%item
        return scene_name, objects
            
    def parse_objects_tree(xml_iter):
        objects = []
        
        while True:
            item = xml_iter.next()
            if item.kind == PXML.TAG_OPEN:
                if item.tag == "object":
                    xml_iter.undo(item)
                    obj = parse_object( xml_iter )
                    objects.append( obj )
                elif item.tag == "tfm" or item.tag=="transform":
                    xml_iter.undo(item)
                    obj_list = parse_transform( xml_iter )
                    objects.extend( obj_list )
                else:
                    raise ValueError, "Unexpected item:%s"%item
            else:
                xml_iter.undo(item)
                return objects
                
    def parse_object( xml_iter ):
        item = xml_iter.next()
        if not (item.kind == PXML.TAG_OPEN and item.tag == "object"):
            raise ValueError, "Expected object tag but dfound something strange:%s"%item
        
        vertices=[]
        triangles = []
        obj = HeomObject()
    
        while True:
            item = xml_iter.next()
            if item.kind == PXML.TAG_CLOSE:
                break
            elif item.kind == PXML.TAG_OPEN:
                xml_iter.undo(item)
                if item.tag=="vertices":
                    vertices.extend( parse_vertices( xml_iter ) )
                elif item.tag=="triangles":
                    triangles.extend( parse_triangles( xml_iter ) )
                elif item.tag=="children":
                    children = parse_objects_tree( xml_iter )
                    obj.children.extend(children)
                else:
                    raise ValueError, "Unexpected tag inside object:%s"%item
        if vertices and triangles:
            obj.set_vertices( vertices )
            obj.set_triangles( triangles )
        else:
            print "Warning: empty object"
        return obj
        
    def parse_vertices (xml_iter ):
        item = xml_iter.next()
        if item.kind != PXML.TAG_OPEN and item.tag != "vertices":
            raise ValueError("Unexpected tag")
        vertices = list()
        while True:
            item = xml_iter.next()
            if item.kind == PXML.TAG_CLOSE:
                break
            elif item.kind== PXML.TAG_OPEN:
                if item.tag=="vertice" or item.tag=="v":
                    x = float(item.attrs.get("x","0"))
                    y = float(item.attrs.get("y","0"))
                    z = float(item.attrs.get("z","0"))
                    t = sqrt(1+x*x+y*y+z*z)
                    vertices.append( (x,y,z,t) )
                    item = xml_iter.next()
                    if item.kind != PXML.TAG_CLOSE: raise ValueError, item
                else:
                    raise ValueError, item
            else:
                raise ValueError, item
        return vertices
                    
    def parse_triangles( xml_iter):
        item = xml_iter.next()
        if item.kind != PXML.TAG_OPEN and item.tag != "triangles":
            raise ValueError("Unexpected tag")
        triangles = list()
        while True:
            item = xml_iter.next()
            if item.kind == PXML.TAG_CLOSE:
                break
            elif item.kind== PXML.TAG_OPEN:
                if item.tag == "triangle" or item.tag=="t":
                    a = int(item.attrs.get("a"))
                    b = int(item.attrs.get("b"))
                    c = int(item.attrs.get("c"))
                    triangles.append((a,b,c))
                    item = xml_iter.next()
                    if item.kind != PXML.TAG_CLOSE: raise ValueError, item
                else:
                    raise ValueError, item
            else:
                raise ValueError, item
        return triangles
        
    def parse_transform( xml_iter ):
        return []
    
    name, objects = parse_scene(xml_iter)
    print "Parsed scene %s, found %d root object(s)"%(name, len(objects))
    root = HeomObject()
    root.children = objects
    return root

def test_load_geom():
    test_scene="""
<scene name="Test scene">
    <object>
        <vertices>
            <v x="0" y="0"/>
            <v x="0" y="1"/>
            <v x="1" y="1"/>
            <v x="1" y="0"/>
        </vertices>
        <triangles>
            <t a="0" b="1" c="2"/>
            <t a="0" b="2" c="3"/>
        </triangles>
    </object>
</scene>"""
    from StringIO import StringIO as sio
    return load_geometry( sio(test_scene) )
    #format:
    #  #scene
#          <tfm offset="0;0;0">
 #         <tfm rotate="0;0;0">
  #        <tfm matrix="1;2;3;4;5;6;7...;16">
          
          
    #     <object offset="0;0;0">
    #     <object rotate
    
    #         <shift dx="" dy="" dz="">
    #         <
    #  <
    

if __name__ == "__main__":
    object = test_load_geom()
    m = Main(startup_heometry = object)
