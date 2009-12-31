import random

def insert_sorted(lst, value, key):
    value_key = key(value)
    def insert_pos(a, b):
        l = b-a
        if l == 0:
            return a+1
        if l == 1:
            if key(lst[a])>value_key:
                return a
            else:
                return a+1
        mid = (b+a)/2
        if key(lst[mid])> value_key:
            return insert_pos(a,mid)
        else:
            return insert_pos(mid,b)
    pos = insert_pos(0, len(lst))
    lst.insert(pos, value)

def test_insert_sorted():
    for i in range(100):
        lst = []
        for j in range(10):
            insert_sorted(lst, random.random(), lambda x: x)
        l1 = lst[:]
        l1.sort
        if l1!=lst:
            print l1, lst
            raise ValueError, "Failed"

            
import numtheor

def enumerate_tuples(tuple_size):
    tuples = [] #list of ( (tuple), value)
    primes = numtheor.primes(100000)
    max_index = len(primes)

    def key_func( tpl ):
        return sum([primes[i] for i in tpl])
    t0 = tuple(range(tuple_size))
    def put_tuple( tpl ):
        key = key_func( tpl )
        insert_sorted( tuples, (tpl, key), lambda t: t[1] )

    put_tuple( t0 )

    while tuples:
        t, key = tuples.pop( 0 )#get the value wit least key func
        v = list(t)
        yield [primes[i] for i in t], key
#Produce all sub-values
        lst = list(t)
        for i in xrange(tuple_size-1):
            if lst[i+1]-lst[i]>1:
                lst[i]+=1
                put_tuple( tuple(lst))
                lst[i]-=1
        if lst[-1]<max_index:
            lst[-1]+=1
            put_tuple( tuple( lst ))
            lst[-1]-=1


if __name__=="__main__":
    test_insert_sorted()

    for tpl, key in enumerate_tuples(3):
        print tpl,"-->",key





