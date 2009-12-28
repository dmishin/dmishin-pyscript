from numtheor import gcd


def ngon_sides(n):
    def simplify(a,b):
        d = gcd(a,b)
        return (a/d, b/d)
    
    return [simplify(k, n) for k in range(n)]

def task228():
    all_sides=set()

    for n in range(1864, 1909+1):
        for s in ngon_sides(n):
            all_sides.add(s)

    return len(all_sides)

print "Answer is:",task228
print "Solved OK"
