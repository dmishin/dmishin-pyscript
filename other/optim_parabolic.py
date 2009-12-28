def fmax(f, x0, x1, x2, st):
	y0=f(x0)
	y1=f(x1)
	y2=f(x2)
	while st>0:
		dy1=y1-y0
		dy2=y2-y0
		dx1=x1-x0
		dx2=x2-x0
		D=2*(dy1*dx2-dy2*dx1)
		if abs(D)<1e-6: break
		x3=x0-(dy2*dx1*dx1-dy1*dx2*dx2)/D;
		y3=f(x3)
		print x3,"-->", y3
		x0,x1,x2=x1,x2,x3
		y0,y1,y2=y1,y2,y3
		st -= 1
	return x2,y2
def fmaxc(f, x0, x1, x2, st):
	y0=f(x0)
	y1=f(x1)
	y2=f(x2)
	while st>0:
		dy1=y1-y0
		dy2=y2-y0
		dx1=x1-x0
		dx2=x2-x0
		D=2*(dy1*dx2-dy2*dx1)
		if abs(D)<1e-6: break
		x3=x0-(dy2*dx1*dx1-dy1*dx2*dx2 + dy1*dy2*(y1-y2))/D*0.5;
		y3=f(x3)
		print x3,"-->", y3
		x0,x1,x2=x1,x2,x3
		y0,y1,y2=y1,y2,y3
		st -= 1
	return x2,y2
def fmaxsp(f, x0, x1, x2, st):
	y0=f(x0)
	y1=f(x1)
	y2=f(x2)
	while st>0:
		a  = (y2+y0-2*y1)/4
		b = y1-y0
		t0 = -b/a/2

		x3 = (x2+x0-2*x1)/4*t0**2+(x1-x0)*t0+x0
		y3=f(x3)
		
		print x3,"-->", y3
		x0,x1,x2=x1,x2,x3
		y0,y1,y2=y1,y2,y3
		st -= 1
	return x2,y2
