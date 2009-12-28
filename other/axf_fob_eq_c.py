def sfun(a,b,c):
	def f(x):
		bn_x = x
		den=1
		s=0
		n=0
		sgn=1
		while n<100:
			den = den*a(bn_x)
			ds = sgn*c(bn_x)/den
			s += ds
			if abs(ds)<1e-6:
				break
			n+=1
			bn_x = b(bn_x)
			sgn=-sgn
		return s
	return f