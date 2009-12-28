import numpy
import pickle

#reading the data

#fname = "ashes-big.rle"
#fname = "/home/dim/highlife-ashes"
#ofile = 'hlife-data.pickle'
fname = "/home/dim/ashes_b3s238.rle"
ofile = 'life-b3s238-data.pickle'

fl = open(fname,'r')
title = fl.readline()

SZ = 2048

arr=numpy.zeros((SZ,SZ), numpy.double)
sums = arr.copy()



			

			
def set_cell(x,y,v):
	if x<SZ:
		arr[y,x]=v

		
y=0
r=0	
x=0

while True:
	ln = fl.readline()
	if not ln: break
	for c in ln.strip():
		if '0'<=c and c<='9':
			r=r*10+ord(c)-ord('0')
			continue
			
		
		if c=='o':
			if r==0: r=1
			for x1 in xrange(x,x+r):
				set_cell(x1,y,1.0)
			x+=r
			r=0
			continue
			
		if c=='b':
			if r==0: r=1
			x+=r
			r=0
			continue
			
		if c=='$':
			x=0
			y+=1
			r=0
			if y==SZ:
				y=0
				#sums+=arr
				#break
				sums+=abs(numpy.fft.fft2(arr))
				arr.fill(0)
				print "Frame Processed"
			continue

fl.close()
#now writing the data

print "Writing the data"
pickle.dump(sums,open(ofile,'w'))
print "Done"
