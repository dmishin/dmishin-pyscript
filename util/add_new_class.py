import os
import sys
import re

src_folder="src/"

classname = sys.argv[1]

print "Adding class name %s"%classname

name_splitter = re.compile("([A-Z][a-z0-9_]*)")

nm = filter( lambda s: s, name_splitter.split( classname ))

upcased = "_".join(map(lambda s:s.upper(), nm))
lowcased = upcased.lower()

fl = open(lowcased+".h", "w")
fl.write("""#pragma once
#ifndef __%s_INCLUDED__
#define __%s_INCLUDED__

class %s
{
public:
};

#endif
"""%(upcased, upcased, classname))
fl.close()

fl = open(lowcased+".cpp","w")
fl.write("""#include "%s"
//class %s
"""%(lowcased, classname))
fl.close()

os.system("git add %s.h"%lowcased)
os.system("git add %s.cpp"%lowcased)
