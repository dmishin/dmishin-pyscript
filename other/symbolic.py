import math

#table of fucntions, available for float evaluator
functions={
    "+":lambda *args: sum(args),
    "-":lambda *args: -args[0] if len(args)==1 else args[0]-sum(args[1:]),
    "*":lambda *args: reduce(lambda x,y: x*y, args),
    "/": lambda x,y: x/y,
    "^": lambda x,y: x**y
    }
    

def sxed(*funcs):
    "Wrap function with XS(...(SX(arg))), i.e., make it work with strings instead of S-expressions"
    def wrapped(sexpr):
        x = SX(sexpr)
        for f in funcs[::-1]:
            x = f(x)
        return XS(x)
    return wrapped


def import_module(module):
    "Make all functions from module available for fp-evaluator"
    for fun_name in dir(module):
        fun = getattr(math, fun_name)
        if hasattr(fun,"__call__"):
            functions[fun_name]=fun

import_module(math)


def isatom(expr):
    "Returns True ofr atomic expressions: numbers, variables, operations"
    return not hasattr(expr, "__iter__")

opers = set(["+","*","-"])
associative = set(["+","*"])

def infix(expr):
    "convers S-epression to infix notation"
    if isatom(expr): return str(expr)
    op=expr[0]
    if op in "+*/":
        return "(%s)"%op.join(map(infix, expr[1:]))
    elif op == "-":
        if len(expr)==2: return "(-%s)"%infix(expr[1])
        else:
            return "(%s)"%("-".join(map(infix, expr[1:])))
    else:
        return "%s(%s)"%(op, ",".join(map(infix, expr[1:])))


def mexp_up(func, expr):
    "Map expression transform, applying it first to upper level, then to children"
    m1 = func(expr)
    if isatom(m1): return m1
    return m1[:1]+[mexp_up(func, o) for o in m1[1:]]

def mexp_down(func, expr):
    "Map expression transform, applying it first to children, then to upper level"
    if isatom(expr): return func(expr)
    return func(expr[0:1]+[mexp_down(func, o) for o in expr[1:]])

def mapped_up(func):
    def mapped(expr):
        return mexp_up(func, expr)
    return mapped

def mapped_down(func):
    "Decorator for mapping expression transform from child to parent"
    def mapped(expr):
        return mexp_down(func, expr)
    return mapped

def atom_ignore(mapper):
    "Decorator, that makes expression transformation function ignoring atom declarations"
    def mapped(expr):
        if isatom(expr):
            return expr
        return mapper(expr)
    return mapped

@atom_ignore
def simsim(expr):
    if expr[0] not in associative: return expr
    op=expr[0]
    rval=[op]
    for x in expr[1:]:
        if not isatom(x) and x[0]==op:
            rval.extend(x[1:])
        else:
            rval.append(x)
    return rval

    
simsimm=mapped_down(simsim)

@atom_ignore
def sort(expr):
    return expr[:1]+list(sorted(expr[1:]))
sortm = mapped_down(sort)

@atom_ignore
def expand(expr):
    op=expr[0]
    if op!='*': return expr
    operands=expr[1:]
    def expanded(operands):
        op=operands[0]
        if isatom(op) or op!='+':
            yield op
            
@atom_ignore
def collect(expr):
    pass


@atom_ignore
def copy(expr):
    return expr[:1]+map(copy, expr[1:])

def evalf(expr, args):
    if isatom(expr):
        try:
            return args[expr]
        except KeyError:
            return expr
    else:
        op=expr[0]
        args=[evalf(e, args) for e in expr[1:]]
        try:
            return functions[op](*args)
        except KeyError:
            raise ValueError("Dont know how to evaluate:%s"%infix(expr))
        
class SExprSyntaxError (Exception):
    pass
    
def SX(sexpr):
    def lex():
        pos = 0
        begin = 0
        try:
            while True:
                c = sexpr[pos]
                if c in "()":
                    if begin != pos:
                        yield sexpr[begin:pos]
                    yield c
                    pos += 1
                    begin = pos
                elif c in " \t\n\r":
                    if begin != pos:
                        yield sexpr[begin:pos]
                        pos += 1
                        begin = pos
                    else:
                        pos += 1
                        begin = pos
                else:
                    pos += 1
        except IndexError:
            return

    def parse_sym(char):
        try:
           return int(char)
        except ValueError:
            pass
        try:
            return float(char)
        except ValueError:
            pass
        return char
            
        
    def parse(lexed):
        sym = lexed.next()
        if sym == ')':
            return None
        
        if sym != '(':
            return parse_sym(sym) #atom
        else:
            lst = []
            while True:
                sublist = parse(lexed)
                if sublist == None: break
                lst.append(sublist)
            return lst
    try:
        return parse(lex())
    except StopIteration:
        raise SExprSyntaxError("Unmatched opening braces")
    
print SX("(+ 12 x 33.1 (/ 1 2) (+ 1 2 3))")

def XS(expr):
    "back conversiot from lsits to S-expression in string"
    if isatom(expr): return str(expr)
    return "("+" ".join(map(XS, expr))+")"

def isnum(atom):
    return isinstance(atom, (int, long, float))

def minus_expr(expr):
    "Calculate -expr for given expr, making easy simplifications"
    if isatom(expr):
        if isnum(expr):
            return -expr
        else:
            return ['-', expr]
    else:
        op = expr[0]
        narg = len(expr)-1
        if op=='-':
            if narg == 1: #(- x) ==> x
                return expr[1]
            else: #(- a b c d) => (+ (-a) b c d)
                return ['+', minus_expr(expr[1]) ] + expr[2:]
        if op == '+': 
            if narg == 1: #(+ a) ==> (- a)
                return minus_expr(expr[1])
            else:
                return ['-', minus_expr(expr[1])] + expr[2:] #(+ a b c d) ==> (- (-a) b c d)
        return ['-', expr]



@atom_ignore
def minus2plus(expr):
    "(a-b-c-d) => (a + (-b) + (-c) + ... ), sign unification"
    if expr[0]!='-': return expr
    narg = len(expr) - 1
    if narg == 0: return 0 #(-)
    elif narg == 1: return expr #(- a) ==> (- a)
    else: #(- a b c) ==> (+ a (-b) ... )
        rval=['+', expr[1]]
        for x in expr[2:]:
            rval.append(minus_expr(x))
        return rval

            
    
@atom_ignore
def evals(expr):
    "Evaluate as much as possible"
    #first make some simplifications
    expr=minus2plus(expr) #to simplify work of a simsimm
    expr = simsim(expr)
    
    op=expr[0]
    narg = len(expr)-1
    if op=="*":
        k=1
        rval=[op, 1]
        for x in expr[1:]:
            if isnum(x):
                k *= x
                if k == 0:
                    return 0 #zero appeared in multipliers
            else:
                rval.append(x)
        if k == 1:
            del rval[1]
        else:
            rval[1] = k
            
        if len(rval)<=2:
            return k
        else:
            return rval
    if op == "+":
        k=0
        rval=[op, 0]
        for x in expr[1:]:
            if isnum(x):
                k += x
            else:
                rval.append(x)
        if k == 0:
            del rval[1]
        else:
            rval[1] = k
            
        if len(rval)<=2: #empty
            return k
        else:
            return rval
    if op == "-":
        print "narg",narg
        if narg == 1:
            if isnum(expr[1]):
                return -expr[1]
            else:
                return expr
            
        else: # narg > 1:
            rval = [op]
            if isnum(expr[1]):
                s = expr[1]
                rval.append(0)
            else:
                s = 0
                rval.append(expr[1])
            for x in expr[2:]:
                if isnum(x):
                    s -= x
                else:
                    rval.append(x)
            #sum calculated
            if isnum(rval[1]):
                rval[1] = s
            else:
                rval.insert(2, -s)
            return rval
    
    #if nothing helped
    return expr

evalsm=mapped_down(evals)
    
print sxed(mapped_down(evals))("(+ a b (* x 2 0))")

def subst(expr, substitutions):
    "Substitute variables (or numbers) with expressions"
    if isatom(expr):
        try:
            return copy(substitutions[expr])
        except KeyError:
            return expr
    else:
        return expr[:1]+[subst(e,substitutions) for e in expr[1:]]
    

    
print infix(sortm(simsimm(["+","a","b",["+","c","d",['*',"e","f",["+","xx","yy"]]],"x"])))
print infix(sortm(simsimm(["+","a","b",["+","c","d",['+',"e","f",["+","xx","yy"]]],"x"])))

