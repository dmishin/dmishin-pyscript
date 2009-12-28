from itertools import imap
#my decorators

def memoize(func):

    def memoized(*args):
        try:
            return memoized.memo[args]
        except KeyError:
            val = func(*args)
            memoized.memo[args]=val
            return val

    memoized.memo = dict()

    def clear():
        "Clear memo"
        memoized.memo.clear()
    memoized.clear = clear
    
    if func.__doc__:
        memoized.__doc__="[memoized] "+func.__doc__
        
    return memoized



def mapped(func):
    def decorated(*args):
        return map(func, *args)
    if func.__doc__:
        decorated.__doc__ = "[mapped] "+func.__doc__
    return decorated

def imapped(func):
    def decorated(*args):
        return imap(func, *args)
    if func.__doc__:
        decorated.__doc__ = "[iterator mapped] "+func.__doc__
    return decorated


def watched(func):
    def decorated(*args, **kwd):
        print "Call %s with args:"%func.__name__, args, kwd,
        rval = func(*args, **kwd)
        print "-->", rval
        return rval
    if func.__doc__:
        decorated.__doc__ = "[watched] "+func.__doc__
    return decorated


def attr(**attrs):
    def decorator(func):
        def decorated(*args, **kwds):
            return func(*args, **kwds)
        for name, value in attrs.items():
            setattr(decorated, name, value)
        decorated.__doc__ = func.__doc__
        return decorated
    return decorator

