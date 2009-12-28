from numpy import matrix, eye, zeros
from math import sin, cos, sinh, cosh, sqrt, pi, log
import sys

r = (4.0/5.0)**(0.25)
t0 = sqrt(r**2+1)

r1 = 0.2**(0.25) #xy distance from center to che center of pentagon side

pentaConter = 0

def acost(x):
	return log(sqrt(x*x+1)+x)
	
def shiftxmatrix(dist):
	c=cosh(dist)
	s=sinh(dist)
	m=matrix([\
		[c,0.0,s],
		[0.0,1.0,0.0],
		[s,0.0,c]])
	return m


#shift matrix for binary three
shift1 = shiftxmatrix(2* 0.626869662906178)

#shift matrix for space filling with tridents
shift0 = shiftxmatrix(2* -0.842482081462008)
#and it's inverse
ishift0 = shiftxmatrix(2* 0.842482081462008)

#rotate by +36
rotPi5=matrix(eye(3))
rotPi5[0,0]=rotPi5[1,1]=cos(pi/5)
rotPi5[0,1],rotPi5[1,0]=-sin(pi/5),sin(pi/5)

#rotate by -36
irotPi5=matrix(eye(3) )
irotPi5[0,0]=irotPi5[1,1]=cos(pi/5)
irotPi5[0,1],irotPi5[1,0]=sin(pi/5),-sin(pi/5)

#rotate by +72
rot2Pi5=matrix(eye(3))
rot2Pi5[0,0]=rot2Pi5[1,1]=cos(2*pi/5)
rot2Pi5[0,1],rot2Pi5[1,0]=-sin(2*pi/5),sin(2*pi/5)

#rotate by +72
irot2Pi5=matrix(eye(3))
irot2Pi5[0,0]=irot2Pi5[1,1]=cos(2*pi/5)
irot2Pi5[0,1],irot2Pi5[1,0]=sin(2*pi/5),-sin(2*pi/5)

#rotate by +72
rotPi=matrix(eye(3))
rotPi[0,0]=rotPi[1,1]=-1.0

rotPi2 = matrix([[0.0, -1.0, 0.0],[1.0,0.0,0.0],[0.0,0.0,1.0]])
irotPi2 = matrix([[0.0, 1.0, 0.0],[-1.0,0.0,0.0],[0.0,0.0,1.0]])
current_vertex_index = 0

def GeneratePentaPlane( ofile, num_levels, gap, separate_pentagons ):
	#preparations
	rPenta = r*gap;
	pentagonPoints = matrix([[0.0,0.0,1.0]]+\
			[ [cos(2*pi*i/5)*rPenta, sin(2*pi*i/5)*rPenta, sqrt(rPenta**2+1)] for i in range(5)]).transpose()
	#helper function definitions
	
	def exportPenta(T):
		"xport pentagon with given transform matrix"
		global pentaConter
		global current_vertex_index
		pentaConter += 1
		
		pnts=T*pentagonPoints
		if separate_pentagons:
			ofile.write("<object>\n")
			
		ofile.write(" <vertices>\n")
		for i in range(pnts.shape[1]):
			ofile.write('  <v x="%s" y="%s" z="%s"/>\n'%(pnts[0,i],pnts[1,i],0.0))
		ofile.write(" </vertices>\n")
		
		ofile.write(" <triangles>\n")
		for i in range(1,pnts.shape[1]):
			ofile.write('  <t a="%s" b="%s" c="%s"/>\n'%(\
			0+current_vertex_index,\
			i+current_vertex_index,\
			((i+1) if i+1<pnts.shape[1] else 1)+current_vertex_index))
		ofile.write(" </triangles>\n")
		
		if separate_pentagons:
			ofile.write("</object>\n")
		
		if not separate_pentagons:
			current_vertex_index += 6

	def exportPentaTree(T ,levels):
		"Exports binary tree of pentagons, beginning from specified one"
		exportPenta(T)
		if levels>0:
			T1=T*(rotPi5 * shift1)
			exportPentaTree(T1, levels-1)
			T1=T*(irotPi5 * shift1)
			exportPentaTree(T1, levels-1)
			
			#filling the gap
			exportTrident(T*(ishift0*rotPi),levels-1)

	def exportTrident(T, levels):
		exportPenta(T)
		if levels>0:
			#exporting tree-like parts
			exportPentaTree(T*(rotPi5*rot2Pi5*shift1), levels-1)
			exportPentaTree(T*(irotPi5*irot2Pi5*shift1), levels-1)
			exportPentaTree(T*(rotPi*shift1), levels-1)
			
			#filling angles
			exportTrident(T*(rotPi5*shift0), levels-1)
			exportTrident(T*(irotPi5*shift0), levels-1)

	def exportPlane(levels):
		exportTrident(shiftxmatrix(-acost(r)),levels) #main quadrant
		exportTrident(rotPi*shiftxmatrix(-acost(r)),levels)
		exportTrident(rotPi2*shiftxmatrix(-acost(r)),levels)
		exportTrident(irotPi2*shiftxmatrix(-acost(r)),levels)
		
	#Running the export code	
	ofile.write("<scene name=\"Pentafill\">\n\n")
	if not separate_pentagons:
		ofile.write("<object>\n")
	exportPlane( num_levels )
	if not separate_pentagons:
		ofile.write("</object>\n")
	print "Created plane of %d pentagons, %d vertices, %d triangles"%(pentaConter, pentaConter*6, pentaConter*5)
	ofile.write("</scene>\n")


if __name__=="__main__":
	from optparse import OptionParser
	parser = OptionParser()
	parser.add_option("-o", "--output", dest="outfile",
                  help="File to write data to, default is pentagons.xml")
                  
	parser.add_option("-N", "--num-iters", dest="iterations", type="int",
                  help="How many iterations to use. Warning: size grows exponentially, using more than 6 is not recommended")

	parser.add_option("-g", "--gap", dest="gap", type="float",
                  help="Gap between pentagons. 1 is plane without gaps, 0 is zero-size pentagons")

	parser.add_option("-s", "--separate", dest="separate", action="store_true", default=False,
                  help="Generate separate object for each pentagon. May be slow for big sizes.")
	
	(options, args) = parser.parse_args()
	
	outfile = options.outfile
	if outfile == None:
		print "File not specified, writing data to pentagons.xml"
		outfile="pentagons.xml"
	iters = options.iterations
	if iters == None:
		print "Iterations not specified, using 3"
		iters = 3
		
	if iters <0:
		print "Iterations number must be positive"
		exit(1)
		
	if iters > 6:
		print "Warning: many iterations, file may be really big!"
		
	gap = options.gap
	if gap == None:
		print "Gap size not specified, using 0.8"
		gap = 0.8
	if gap<0 or gap >1:
		print "Warning: recommended value for gap are in [0;1]"

	try:
		ofile=open(outfile,'w')
		GeneratePentaPlane( ofile, iters, gap, options.separate)
		ofile.close()
	except Exception, msg:
		print "Failed:", msg
		exit(1)
