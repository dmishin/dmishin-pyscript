#variable precision trigonometry functions
import gmpy
import numpy
from matplotlib import pyplot
import pickle
import math

N=1000

pi=gmpy.pi(N)

pi2=pi/2
pi2_minus = -pi2
pix2 = pi*2
pi_minus = -pi

mpone=gmpy.mpf(1,N)

eps=gmpy.mpf(2)**(-N)


def sin_taylor(x):
	x2 = x*x
	s=0
	i=2
	an = x
	sgn = 1
	while True:
		s = s + sgn*an
		an = an*x2/(i*(i+1))
		i += 2
		sgn = -sgn		
		
		if an<eps:
			return s

def cos_taylor(x):
	x2 = x*x
	s=1
	i=3
	an = x2/2
	sgn = -1
	while True:
		s = s + sgn*an
		an = an*x2/(i*(i+1))
		i += 2
		sgn = -sgn		
		
		if an<eps:
			return s
			

def mpsin(x):
	if pi_minus<=x and x<=pi:
		return sin_taylor(x)
	
	if x<0: 
		return -mpsin(-x)
		
	f= x - gmpy.floor(x/pix2)*pix2 #0..2
	
	assert(f>=0)
	assert(f<=pix2)
	
	if f<pi:
		return sin_taylor(f)
	else:
		return -sin_taylor(f - pi)

	
#print pi, eps
def ssin(x):
	s = sin_taylor(x)
	c = cos_taylor(x)
	a = mpone
	
	r = 0
	while True:
		#print a, float(s), float(c)
		r = r + a*s
		if a<eps:
			break
		s2 = 2*(s*c)
		c2 = c*c - s*s
		n = (s2**2+c2**2).sqrt()
		
		s=s2/n
		c=c2/n
		
		a = a/2
		
	return r
		

#print "1.32983327628731085044041828620650638770765078454685228565427"
#print ssin((36*pi)/127)

#print "1.32983327628731085044041828620650638770765078454685228565427"
#print ssin((36*pi)/127 - gmpy.mpf(1e-54, N))

def dquot(x0,fx0,h):
	x = x0+gmpy.mpf(h,N)
	f = ssin(x)
	return float((f-fx0)/h)
	
def deepzoomplot(a,b):
	#plotting differential quotient for very deep zoom
	h = numpy.logspace(a,b,2000)
	x0 = (36*pi)/127
	y0 = ssin(x0)
	
	def dq(x):
		return dquot(x0,y0,-x)	
		
	y = numpy.vectorize(dq)(h)
	
	pyplot.semilogx(h, y)
	pyplot.show()

	#saving data	
	
	ofile = open("ssin_quotient_negative_[%d;%d].pickle"%(a,b), "w")
	pickle.dump((h,y), ofile)
	ofile.close()


#deepzoomplot(-27,-26)
#deepzoomplot(-29,-28)
#deepzoomplot(math.log10(1.96*1e-29), math.log10(2.01*1e-29))
print str(ssin((36*pi)/127))[:80]
print str(ssin((36*pi)/127-gmpy.mpf(1.998e-29,N)))[:80]


def ssin_test(x):
	s = sin_taylor(x)
	c = cos_taylor(x)
	a = mpone
	
	r = 0
	idx = 0
	ssins = [None]*7
	
	while True:
		
		if idx<7:
			print idx, float(s), float(c)
		else:
			print idx, float(s), float(c), float(s-ssins[idx%7])
		
	
		if idx<	7:
			ssins[idx] = s
			
		idx = idx+1

		if idx>2000:
			break
			
		s2 = 2*(s*c)
		c2 = c*c - s*s
		n = (s2**2+c2**2).sqrt()
		
		s=s2/n
		c=c2/n
		
		
	return r
#ssin_test((36*pi)/127)
