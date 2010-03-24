##############################################################################
#
# add_menu_item.py <Peter.Bienstman@UGent.be>
#
##############################################################################

from mnemosyne.core import *
from mnemosyne.pyqt_ui.plugin import * 
from qt import *

main_widget = get_main_widget()



##############################################################################
#
# Plugin to add a simple menu item.
#
##############################################################################

class HelloWorld(Plugin):

    def description(self):
        return "Insert hello world menu item."

    def load(self):
        main_widget.helpMenu.insertItem("Hello world",self.hi,0,666)

    def unload(self):
        main_widget.helpMenu.removeItem(666)
        
    def hi(self):
        QMessageBox.information(None, "Mnemosyne", "Hello world!")

p = HelloWorld()
p.load()
