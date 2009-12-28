from numpy import matrix, array, zeros
from math import sin, cos



class segment:
    def __init__(self):
        self.len = 1.0
        self.angle = 0.0
        self.mass=1.0

        self.pos = matrix([0.0,0.0]).transpose()
        #self.vel = matrix([0.0,0.0]).transpose()

        self.angle = 0.0

        self.tform=zeros(2)
        
                            
        pass
    

    
