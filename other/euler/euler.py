from numtheor import *

def task97():
	p=10**10
	v= (pow_mod(2,7830457,p)*28433+1)%p
	print "answer is", v

#task97()

def task108():
	def numdivsx2(x):
		return reduce(lambda a,b: a*b,[f[1]*2+1 for f in grp(pfac(x))])
		
	#print findfirst(allnums(2),lambda x:numdivsx2(x)+1>=2000)
	print (numdivsx2(4)+1)/2

#task108()

#task()

def task18():
	"""maximal travel sum in triangle
	Done!
	"""
	
	sdata="""75
95 64
17 47 82
18 35 87 10
20 04 82 47 65
19 01 23 75 03 34
88 02 77 73 07 63 67
99 65 04 28 06 16 70 92
41 41 26 56 83 40 80 70 33
41 48 72 33 47 32 37 16 94 29
53 71 44 65 25 43 91 52 97 51 14
70 11 33 28 77 73 17 78 39 68 17 57
91 71 52 38 17 14 91 43 58 50 27 29 48
63 66 04 68 89 53 67 30 73 16 69 87 40 31
04 62 98 27 23 09 70 98 73 93 38 53 60 04 23"""
	data = [ [int(x) for x in l.split(' ')] for l in sdata.split('\n')]
	
	def max(a,b):
		if a>b: return a
		else: return b
	
	def merge_lines(l1,l2):
		return [max(a+b1,a+b2) for a,b1,b2 in zip(l1,l2[1:],l2[:-1])]
	
	
	return reduce(lambda a,b:merge_lines(b,a), data[::-1])
	
#print task18()

	

def task21():
	"Looking for amicable pairs under 10000"
	#http://projecteuler.net/index.php?section=problems&id=21
	
	D=map(sumdivs, range(1,10000+1))
	def d(x):
		if x==None: return None
		if x<=len(D):
			return D[x-1]
		else:
			return None
	#now looking for result:
	amicable = [ pair for pair in [(x,d(x)) for x in xrange(1,10000+1)] if d(pair[1])==pair[0] and pair[1]>pair[0] ][:100]
	print "all amicable pairs:", amicable
	return sum([ a+b for a,b in amicable])
	
	
#print task21()

def task28():
	def sumd(n):
		return 2*n*(n+1)*(n+2)/3-6*(n+1)*(n+1)+3*(n+1)-3
		
	print "Test: sum of 5x5 spiral:", sumd(5)
	print "Sum of diagonals of 1001x1001 spiral is", sumd(1001)
	
#task28()
	

#print filter(issquare, range(100+1))



def task57():
	def gener():
		p,q=1,1
		for idx in xrange(1000):
			p,q=p+2*q,p+q
			yield (p,q)
	
	print "Answer is:", len([x for x in gener() if len(str(x[0]))>len(str(x[1]))])

#task57()
	


def task122():
	
	steps=[0]+[1000]*199 #number of steps, needed to get the number
	
	assert len(steps)==200
	
	maxDepth=16
	
	def calculate(availValues=set([1]), step=1): #16 is guaranteed to give result
		#getting all new values we can get, using available ones
		if step>=maxDepth:
			return
		newVals=availValues.copy()
		for a in availValues:
			for b in availValues:
				if b>=a:
					s=a+b
					if s not in availValues and s<=200 and steps[s-1]>step:
						steps[s-1]=step
						newVals.add(s)
						calculate(newVals,step+1)
						newVals.remove(s)
	print "Starting recursive algorithm"
	calculate()
	print "Done"
	print "NUmber of steps for some values:", zip(xrange(1,201),steps)
	print "Sum is:", sum(steps)
	
	print steps



   

def task62():
	cperms=set()
	
	def nqperms(x):
		cperms.clear()
		l=list(str(x**3))
		for n in xrange(factorial(len(l))):
			tn=int(''.join(nthperm(l, n)))
			if tn not in cperms and iscube(tn):
				#print x,'-->',tn
				cperms.add(tn)
		return len(cperms)
		
	for x in itertools.count(3):
		if x%10==0:
			continue	
		if nqperms(x)>1:
			print x, 'Has', len(cperms),'cubic permutations'




def task70():
	lst=[]
	for x in xrange(1,10**7):
		f=phi(x)
		if isperm(x,f):
			print x, f
			lst.append((x,float(x)/float(f)))
	lst.sort(key=lambda x:x[1])
	print "best ratio for ",lst[0]
			

def task127():
	
	def dividable(a,lst):
		for d in lst:
			if a%d ==0: 
				return True
		return False
		
	rad_dic = dict()
	irad_mem = memoize(irad,rad_dic)
	
	def hits(c):
		rval=[]
		
		rf = uniq(pfac(c))
		r  = prod(rf) #radical of c
		
		dc = c/r #rad(a)*rad(b) < dc   a+b = c gcd(a,c)==0
		
		if dc==1:
			return rval 
		
		for a in range(1,c/2+1):
			if dividable(a,rf):
				continue #skipping GCD(a,c)!=1
			ra=irad_mem(a)
			if ra >= dc:
				continue 
			b=c-a
			rb = irad_mem(b)
			if ra*rb < dc:
				rval.append((a,b,c))
				
		return rval
			
	cnt = 0
	def sum_hits(below):
		s=0
		numhits = 0
		for c in xrange(1,below):
			for a,b,c in hits(c):
				s+=c
				numhits+=1
		print "Total hits below %d: %d"%(below,numhits)
		
		return s
	
	print "Sum of hits: %d"%sum_hits(10000)

task127()			
