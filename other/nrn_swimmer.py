import nrn
import os
from math import sin, cos, pi, sqrt
from numpy import array, matrix
import pygame

def scprod(v1,v2):
    return sum(v1*v2)

class swimmer:
    def __init__(self):
        self.net = nrn.net()
        self.X=array([0.0,0.0])
        self.V=array([0.0,0.0])

        self.phi=0.0
        self.B=0.0
        
        self.m=1.0
        self.I=2.0
        self.dempfing = 0.4 #liquid friction
        
        self.motors=[0,0,0,0]
        self.motor_powers=[1.0,1.0,1.0,1.0]
        self.motor_attachment=array([(-1.0,-1.0),(1.0,-1.0),(0.0,1.0),(0.0,1.0)])
        self.motor_vector=array([(0.0,1.0),(0.0,1.0),(0.8,-0.2),(-0.8,-0.2)])
        
        self.init_network()

        self.v_loc=array([0,0])
        
    def sim(self, dt):
        self.sim_network(dt)
        self.move(dt)
        

    def move(self, dt):
        self.X += self.V * dt
        self.phi += self.B * dt

        self.V *= (1.0-dt*self.dempfing)
        self.B *= (1.0-dt*self.dempfing)

        #use motor data

        self.v_loc[:]=[0.0,0.0]
        
        for i in xrange(len(self.motors)):
            m_x = self.motor_attachment[i]
            m_v = self.motor_vector[i]
            
            k_move = scprod(m_x,m_v)/scprod(m_x, m_x)

            k_rot = m_x[0]*m_v[1]-m_x[1]*m_v[0]

            self.v_loc += (k_move*self.motors[i]/self.m*dt)*m_x

            self.B += (k_rot*motors[i]/self.I*dt)

        if self.phi>pi:
            self.phi -= pi*2
        elif self.phi <-pi:
            self.phi += pi*2
            
        s=sin(self.phi)
        c=cos(self.phi)

        self.V[0] += self.v_loc[0]*c-self.v_loc[1]*s
        self.V[1] += self.v_loc[0]*s+self.v_loc[1]*c
        
            

    def sim_network(self):
        #self.net.sim(dt)
        pass

    def init_network(self):
        pass



class swimmer_pool:
    def __init__(self):
        self.swimmers=list()

    def sim(self, dt):
        for s in self.swimmers:
            s.sim(dt)

    def draw(self, screen, image):
        pass


os.environ['SDL_VIDEO_CENTERED'] = '1'
 
pygame.init()

pool = swimmer_pool()

swimmer = pygame.image.load("swimmer.bmp")

angle=0

try:
    screen = pygame.display.set_mode((600, 600))
 
    # This should show a blank 200 by 200 window centered on the screen
    pygame.display.flip()
 
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            break
        screen.fill((0,0,0))
        pool.draw(screen,swimmer)
        r = pygame.transform.rotate(swimmer, angle)
        screen.blit(r,pygame.rect.Rect(10,10,100,100))
        pygame.display.flip()

        angle+=1
    
finally:
    pygame.quit()  # Keep this IDLE friendly
    
