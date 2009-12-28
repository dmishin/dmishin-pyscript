#optimisation

def sqr(x):
	"y=x*x"
	return x*x


def rozen(X):
	x=X[0]
	y=X[1]
	return sqr(1-x)+100*sqr(y-sqr(x))


#vector math defintions

def zerov(n):
	return [0.0]*n
	
def onesv(n,v=1.0):
	return [v]*n


#def ziter(itra, itrb):
#	return lambda : (itra(), itrb())


def add(a,b,k=None):
	if k==None:
		a[:]=[ai+bi for ai,bi in zip(a,b)]
	else:
		a[:]=[ai+k*bi for ai,bi in zip(a,b)]
	return a

def lcomb(a,ka,b,kb):
	"a:=a*ka+b*kb"
	a[:]=[ai*ka+bi*kb for ai,bi in zip(a,b)]

def scale(a,k):
	a[:]=[ai*k for ai in a]
	return a

def flip(a):
	return scale(a,-1.0)

def dist2(a,b):
	"Returns squared distance between two vectors"
	return sum(map(lambda x,y:sqr(x-y),a,b))

#######################################
## nelder-mead method
#######################################
def nmead(func, x0, maxIter=1000, eps=1e-4, alpha=2.0, beta=0.7,gamma=0.5):
	"""Nelder-mead method
	"""
	
	N=len(x0)
	X=map(lambda a:x0[:], range(N+1))
	h=1.0
	
	#build initial symplex
	for i in range(N):
		X[i][i]+=h
	X[N][0]-=h
	
	#evaluate F in all points
	Y=map(func,X)

	#entering main loop
	iters=0
	while True:	
		if iters>=maxIter:
			minimumFound=False
			break
		iters+=1
		
		#looking for worst point, also calculating best value
		iMax, valMax, valMin=0, Y[0], Y[0]
		for i in xrange(1,N):
			if Y[i]>valMax:
				valMax,iMax=Y[i],i
			elif Y[i]<valMin:
				valMin=Y[i]
		
		#moving worst point to the beginning of the list
		X[0],X[iMax]=X[iMax],X[0]
		Y[0],Y[iMax]=Y[iMax],Y[0]

		print X
		print Y
		
		print "min=%s, max=%s"%(valMin, valMax)
		
		#now calculate centroid of all points except  worst one
		xc=zerov(N)
		for xi in X[1:]:
			add(xc,xi)
		scale(xc,1.0/N)
		
		#now flipping worst point (now it has index 0) through centroid
		xr=X[0][:]
		lcomb(xr,-1.0,xc,2.0)
		yr=func(xr)
		#done flipping. 
		print "xr=%s  yr=%s"%(xr,yr)
				
		#now check value of function in new point.
		
		#whether it MUCH better? (i.e. smaller than minimal)
		if yr<valMin:#yes, much better
			#exponentially extending point along this direction, using ALPHA as coefficient
			#X:=a*X+(1-a)*Xc
			xe=xr[:]
			lcomb(xe,alpha,xc,1.0-alpha)
			ye=func(xe)
			if ye<yr:
				#extend was successful
				print "extend"
				X[0],Y[0]=xe,ye
			else:
				#no, without extending things were better
				X[0],Y[0]=xr,yr
				print "reflect"
			continue
		
		#looks like it is even worse than previous point. Strong overhit?
		if yr>=valMax:
			#shrinking whole symplex, using GAMMA
			for x in X:
				lcomb(x,gamma,xc,1.0-gamma);
			Y=map(func,X)
			print "scale"
			if dist2(xc,X[0])<eps:
				minimumFound=True
				break
			continue
			
		#Ok, it is not MUCH better, but maybe it is just better?
		if yr<valMax:#yes, at least it is better than worst value
			#shrinking using BETA
			xs=xr[:]
			lcomb(xs,beta,xc,1.0-beta)
			ys=func(xs)
			if ys<yr:
				#successful contracting
				X[0],Y[0]=xs,ys
				print "contract"
			else:
				X[0],Y[0]=xr,yr
				print "reflect"
			continue
		#cycle finished
		raise Error, "This should not be reached"
	#lookng for the  best point in simplex
	yMin=Y[0]
	xMin=X[0]
	for x,y in zip(X[1:],Y[1:]):
		if y<yMin:
			xMin,yMin=x,y
	#return the value
	return (xMin,yMin,minimumFound)
		

ans=nmead(lambda x: sqr(x[1])+sqr(x[0]),[5.0,5.0],1006)
print "Answer found:", ans
