def strikeout_transform(seq,N=None):
    "Foe every not-excluded x, exclude all x+seq[i]"
    if N == None:
        N = len(seq)
    sieve = [True]*N
    rval = []
    for i in xrange(len(sieve)):
        if sieve[i]:
            rval.append(i)
            for dx in seq:
                j = i+dx
                if j>=N:
                    break
                else:
                    sieve[j] = False
    return rval
    
