def fzero(f, x0,x1, eps=1e-14):
    y0=f(x0)
    while True:
        y1=f(x1)
        if abs(y0)<=eps:
            return x0
        if y0==y1:
            x1=2*x1-x0
            continue
        else:
            ik=(x1-x0)/(y1-y0)
            x0=x1
            y0=y1
            #(x-x0)/ik+y0==0
            x1=x0-y0*ik
        
            
