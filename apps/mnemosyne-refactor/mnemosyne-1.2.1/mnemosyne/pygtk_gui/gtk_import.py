#this file is to be imported with import*
import sys

try:
    import pygtk
    #tell pyGTK, if possible, that we want GTKv2
    pygtk.require("2.0")
except Error, e:
    #Some distributions come with GTK2, but not pyGTK
    print "Error:", e
    pass

try:
    import gtk
    import gtk.glade
except Error, e:
    print "Error:", e
    print "You need to install pyGTK or GTKv2 ",
    print "or set your PYTHONPATH correctly."
    print "try: export PYTHONPATH=",
    print "/usr/local/lib/python2.2/site-packages/"
    sys.exit(1)
