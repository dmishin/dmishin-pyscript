#!/usr/bin/env python

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

#now we have both gtk and gtk.glade imported
#Also, we know we are running GTK v2

class testgui:
    def __init__(self):
        gladefile = "MnemosineGtk.glade"
        title = "wnd_main"
        self.wTree=gtk.glade.XML (gladefile, title)
        handlers = {
            "on_quit": self.do_quit,
            "answer_clicked": self.on_answer,
            "on_wnd_main_destroy" : (gtk.mainquit),
            }
        self.wTree.signal_autoconnect( handlers )

    def on_answer(self, widget):
        print "Answer clicked:",widget

    def do_quit(self, widget):
        gtk.main_quit()

    def test_handler(self, widget):
        pass

if __name__ == '__main__':
    app = testgui()
    gtk.main()
