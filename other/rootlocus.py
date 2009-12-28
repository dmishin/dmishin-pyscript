import numpy as np
import matplotlib.pyplot as plt
from random import random

class DraggableRectangle:
    def __init__(self, rect):
        self.rect = rect
        self.press = None

    def connect(self):
        'connect to all the events we need'
        self.cidpress = self.rect.figure.canvas.mpl_connect(
            'button_press_event', self.on_press)
        self.cidrelease = self.rect.figure.canvas.mpl_connect(
            'button_release_event', self.on_release)
        self.cidmotion = self.rect.figure.canvas.mpl_connect(
            'motion_notify_event', self.on_motion)

    def on_press(self, event):
        'on button press we will see if the mouse is over us and store some data'
        if event.inaxes != self.rect.axes: return

        contains, attrd = self.rect.contains(event)
        if not contains: return

        #print 'event contains', self.rect.get_xy()
        x0, y0 = self.rect.get_x(), self.rect.get_y()
        self.press = x0, y0, event.xdata, event.ydata

    def on_motion(self, event):
        'on motion we will move the rect if the mouse is over us'
        if self.press is None: return
        if event.inaxes != self.rect.axes: return
        x0, y0, xpress, ypress = self.press
        dx = event.xdata - xpress
        dy = event.ydata - ypress
        #print 'x0=%f, xpress=%f, event.xdata=%f, dx=%f, x0+dx=%f'%(x0, xpress, event.xdata, dx, x0+dx)
        self.rect.set_x(x0+dx)
        self.rect.set_y(y0+dy)

        self.rect.figure.canvas.draw()


    def on_release(self, event):
        'on release we reset the press data'
        if self.press:
            replot_graph()
        self.press = None
        self.rect.figure.canvas.draw()
        
        

    def disconnect(self):
        'disconnect all the stored connection ids'
        self.rect.figure.canvas.mpl_disconnect(self.cidpress)
        self.rect.figure.canvas.mpl_disconnect(self.cidrelease)
        self.rect.figure.canvas.mpl_disconnect(self.cidmotion)


fig = plt.figure()
ax = fig.add_subplot(111)
ax.axis([-10,10,-10,10])
rects = []
drs = []

old_plot = []

NP=2

for i in range(1,NP*2+1):
    x=random()*10-5
    y=random()*10-5
    
    if i <= NP:
        clr = "red"
    else:
        clr = "blue"
    rect = plt.Rectangle((x,y),0.5,0.5, facecolor = clr)    
    
    ax.add_artist(rect)
    rects.append(rect)

for rect in rects:
    dr = DraggableRectangle(rect)
    dr.connect()
    drs.append(dr)

def replot_graph():
    z=[complex(r.get_x()+0.25, r.get_y()+0.25) for r in rects]
    
    while old_plot:
        fig = old_plot.pop()
        fig.remove()
    
    
    old_plot.extend(do_plot(ax, z))
    ax.axis([-10,10,-10,10])

def do_plot(ax, z):
    
    tt=np.linspace(0,1,100)
    yy = z[0]*(1-tt)+z[1]*(tt)
    z1=z[:NP]
    z2=z[NP:]
    
    P = np.poly(z1) 
    Q = np.poly(z2)
    Z0 = np.roots((P-Q)[1:])
    
    def Z(t):
        F=P*(1-t)+Q*t
        return np.roots(F)
        
    N = 100
    
    ZZ = np.zeros([NP, N], np.complex)
    for i in range(N):
        #ZZ[:,i]=Z(i/50.0-0.5).transpose()
        ZZ[:,i]=Z(i/99.0).transpose()

    rval = []
    rval.extend( ax.plot(Z0.real, Z0.imag, "gx") )
    rval.extend( ax.plot(ZZ.real.transpose(), ZZ.imag.transpose(), "r.") )
    return rval


plt.show()


    

