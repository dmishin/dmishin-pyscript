
class undoiter:
    "Undoable iterator class"
    def __init__(self, base_iter):
        self._itr = base_iter
        self._undo_stack=[]
    def next(self):
        "Returns next item from iterator"
        if self._undo_stack:
            return self._undo_stack.pop()
        return self._itr.next()
    def __iter__(self):
        return self
    def undo(self,val):
        "Push the value back to iterator"
        self._undo_stack.append(val)


