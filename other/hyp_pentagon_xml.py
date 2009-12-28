
#Hyperbolic space pentagon XML generator
from math import sin, cos, pi

r = (4.0/5.0)**(0.25) #radius of a pentagon
r = r

z=0

print "<geometry name=\"pentagon\">"

print '  <point id="c" x="0" y="0" z="%s"/>'%z

for i in range(5):
	x=r*cos(2*i*pi/5.0)
	y=r*sin(2*i*pi/5.0)
	print '  <point id="%d" x="%s" y="%s" z="%s"/>'%(i, x, y, z)
	
for i in range(5):
	print '  <triangle a="c" b="%d" c="%d"/>'%(i, (i+1)%5)
	
print "</geometry>"
