from numtheor import gcd, sqrti

def sisq(num,den,dmin, dmax):
    dmin=max(dmin, sqrti(den/num))
    dmax_step = min(dmax, den*2/num+2) #if 1/2/(d+1) < num/den
    
    for d in xrange(dmin, dmax_step):#no need to search further
        
        d2=d*d
        #num/den > 1/d^2
        if num*d2 >= den:
            num1=num*d2-den
            if num1==0:
                yield [d]
                return
            den1=den*d2
                
##            k=gcd(den,d2)
##            num1 /= k
##            den1 /= k
            
            for seq in sisq(num1, den1, d+1, dmax):
                yield seq+[d]

def task_152():
    #test
    print "Test: below 45"
    for seq in sisq(1,2, 2, 36):
        print seq        

task_152()
