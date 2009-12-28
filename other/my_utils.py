
def srepl(string, replacements):
    "Multiple replace all"
    if not replacements:
        return string
    orig, new = replacements[0]
    return new.join([srepl(s, replacements[1:]) for s in string.split(orig)])

def __test__srepl():
    assert(srepl("", ()) == "")
    assert(srepl("ab", (("a","b"),("b","a"))) == "ba")
    assert(srepl("auau au",(("au",""),) ) == " ")
    assert(srepl("hello",(("h","fu"),("ell", "ck"),("o","!"))) == "fuck!")
    assert(srepl("hello", (("ab", "none"), ("llo","oll"))) == "heoll")


def flatten(lst):
    rval = list()
    for i in lst:
        if hasattr(i, "__iter__"):
            rval.extend(flatten(i))
        else:
            rval.append(i)
    return rval

def __test__flatten():
    assert(flatten([]) == [])
    assert(flatten([1,2,3]) == [1,2,3])
    assert(flatten([[[[1]]]]) == [1])
    assert(flatten([[[[]]],[],[],[[[]]]]) == [])
    assert(flatten([1,[2,3],[4],[5,[6]]]) == [1,2,3,4,5,6])


def __test():
    tst = "__test__"
    for varname, var in globals().items():
        if varname[:len(tst)] == tst:
            try:
                print "Running", varname
                var()
                print "  Passed"
            except Exception, e:
                print "  Failed!", e
    
