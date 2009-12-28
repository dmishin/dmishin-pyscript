import math

def memoize(func):
    memo=dict()
    def memoized(*args):
        try:
            return memo[args]
        except KeyError:
            v = func(*args)
            memo[args]=v
            return v
    return memoized

TMAP = [[None]*101]*101

@memoize
def pwin_move12(s1,s2):
    if s1>=100:
        return 0
    if s2>=100:
        return 1
    advance = 100-s2 #how many point need
    tmax = int(math.ceil(math.log(advance,2)))+1
    #2^(tmax-1) >= advance

    

    #Win chance for the case when Player1 moved actually
    pbest1 = None
    for t in range(1,tmax+1):
        p_adv = 0.5**t #probability for advance
        adv = 2**(t-1)
        pwin = p_adv*pwin_move12(s1+1,s2+adv)+(1-p_adv)*pwin_move12(s1+1,s2)
        pbest1 = max(pwin, pbest1)


    PSELF = 0.0 #walue to win if both not move
    iters = 0
    while True:
        #find the best T for the case Player1 not moved
        pbest0 = None
        pbest_candidate = None
        for t in range(1,tmax+1):
            p_adv = 0.5**t #probability for advance
            adv = 2**(t-1)

                
            pwin_base = p_adv*pwin_move12(s1,s2+adv) #+ (1-p_adv)*PSELF
            pwin_candidate = pwin_base + (1-p_adv)*PSELF
            
            if pwin_candidate > pbest_candidate:
                pbest_candidate = pwin_candidate
                pbest0 = (pwin_base, 0.5*(1-p_adv)) #guaranteed part of best probab and the self-coeff


        pwin = 0.5*(pbest0[0] + pbest1)/(1-pbest0[1])

        #check, whether it is final pself iteration
        if pwin == PSELF:
            break
        else:
            PSELF = pwin #one more iteration
            iters += 1
            if iters > 1000: raise Exception("looks like we hung")            
            
    #best T is found
    return PSELF


print pwin_move12(99,99)
