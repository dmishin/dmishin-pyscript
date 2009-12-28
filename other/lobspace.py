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

class Geometry:
    "Geometry arrays"
    def __init__(self):
        self.verts = numpy.zeros((4,1))
        self.random()

        
    def random(self,n=120):
        "create random geometry"
        v = (numpy.random.rand(4,n)-0.5)*4
        #normalize
        #t^2-x^2-y^2-z^2 = 1
        v[3]=numpy.sqrt(v[0]**2+v[1]**2+v[2]**2+1)
        self.verts = v



        


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
    def __init__(self,run=True):
        self.title="Hyperbolic geometry"
        self.nav = Navigator()
        self.nav.move(0,0,-2)

        self.geom = Geometry()

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

            verts=numpy.dot(self.nav.view, self.geom.verts)

            prepare_vertices(verts)
            
            glLoadIdentity()					# Reset The View 


            #################
            glBegin(GL_TRIANGLES)
            for i in xrange(verts.shape[1]):
                glVertex3f(verts[0,i],verts[1,i],verts[2,i])
                if i%2==0:
                    glColor3f(1.0,1.0,1.0)
                else:
                    glColor3f(.5,1.0,.1)
            glEnd()
            #################
            
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
            
            GLUT_KEY_LEFT : (SM_ROT_LR,  -0.2),
            GLUT_KEY_RIGHT: (SM_ROT_LR,  0.2),
            
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
                self.geom = load_geom(filename)
                #print geom.verts
                
        return 0

        
#########
import xml.parsers.expat

def float_attr(dic, attr, default=0.0):
    return float(dic.get(attr,default))

def parse_geometry(fl, put_vertex):
    def put(x,y,z):
        t = sqrt(x**2+y**2+z**2+1)
        put_vertex(x,y,z,t)
        
    def elem_start(name, attribs):
        if name == "point":
            try:
                x=float_attr(attribs,"x",0.0)
                y=float_attr(attribs,"y",0.0)
                z=float_attr(attribs,"z",0.0)
                put(x,y,z)
            except Exception, e:
                print "Warning:",e
            return
        if name == "geometry":
            geom_name = attribs.get("name", "Unnamed geometry")
            return
        print "Unkonwn tag!"

    #parsing
    p = xml.parsers.expat.ParserCreate()
    p.StartElementHandler = elem_start

    p.ParseFile(fl)

        
                
            
def load_geom(fl):
    "Load geometry from XML file"
    geom=[[],[],[],[]]
    def append(*coords):
        for i,x in enumerate(coords):
            geom[i].append(x)
            
    try:
        if isinstance(fl, basestring):
            fl = open(fl, "r")
        parse_geometry(fl, append)
        fl.close()
    except Exception,e:
        print "Error parsing:",e
    geom = numpy.matrix(geom)

    rval = Geometry()
    rval.verts=geom
    return rval

if __name__ == "__main__":
    m = Main()



class HeomObject:
    def __init__(self):
        self.transform = numpy.eye(4)
        self.vertices = None
        self.triangles = None
        
        
    
    
