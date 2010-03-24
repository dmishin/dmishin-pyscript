##############################################################################
#
# activate_all_categories.py <Peter.Bienstman@UGent.be>
#
##############################################################################

from mnemosyne.core import *



##############################################################################
#
# Plugin to activate all the categories on startup.
#
##############################################################################

class ActivateAllCategories(Plugin):

    def description(self):
        return "Activate all categories."

    def load(self):
        register_function_hook("after_load", self.activate_cats)

    def unload(self):
        unregister_function_hook("after_load", self.activate_cats)

    def activate_cats(self):
        for c in get_categories():
            c.active = True

p = ActivateAllCategories()
p.load()
