from math import *

h_fu = 0.1
h_kyuu = 0.3
h_chuu=0.1
h_ryuu=0.1

fu_0 = 1.0
kyu_0 = 1.0
chuu_0 = 1.0
ryuu_0 = 4.0

H0 = 1.0

def shelf(left, right, center, width):
    k = (right-left)*0.5
    b = (right+left)*0.5
    def func(x):
        return tanh((x-center)/width*3.0)*k + b
    return func


f_fu = shelf(-fu_0, fu_0, 0, h_fu)

f_kyuu = shelf(2*kyu_0, 0, 0, h_kyuu)
    
f_chuu = shelf(chuu_0, -chuu_0, H0, h_chuu)

f_ryuu = shelf(-ryuu_0, ryuu_0, 0, h_ryuu)


def dhdt((h,H), t):

    dh = f_fu(H-h)-f_kyuu(h)*f_ryuu(h)*H
    dH = f_chuu(H) - f_ryuu(h)*H

    return (dh,dH)


