import os
fl = open("Makefile.am","w")
fl.write("bin_PROGRAMS = alife\n")
fl.write("alife_SOURCES = ")
for f in os.listdir("./"):
    fn, ext = os.path.splitext( f )
    if ext in [".cpp", ".h"]:
        fl.write("\\\n\t%s "%f)
fl.write("\n")
#alife_glut.cpp item-acceptor.cpp mobile.cpp grid.cpp main.cpp simulator.cpp world.cpp motor.cpp \
#	glut_viewport.cpp random_brain.cpp brain.cpp sensor.cpp food.cpp matrix_brain.cpp
fl.write("alife_LDFLAGS = -lglut -lboost_thread-mt\n")
fl.close()
