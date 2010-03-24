##############################################################################
#
# custom_tag.py <Peter.Bienstman@UGent.be>
#
##############################################################################

import os
from mnemosyne.core import *

tag_name = "my_tag"
tag_program = "/bin/cat"



##############################################################################
#
# Plugin to intercept a custom tag of the form <my_tag src="filename"> in
# the question and answer text, and open its content in a separate program.
#
# This plugin will run several times when a card is displayed, so we need to
# make sure that we run the program only once per card.
#
##############################################################################

class CustomTag(Plugin):

    def __init__(self):
        Plugin.__init__(self)
        self.last_filename = None

    def description(self):
        return "Intercept a custom tag."

    def load(self):
        register_function_hook("filter_q", self.custom_tag)
        register_function_hook("filter_a", self.custom_tag)
        
    def unload(self):
        unregister_function_hook("filter_q", self.custom_tag)
        unregister_function_hook("filter_a", self.custom_tag)

    def custom_tag(self, text, card):

        i = text.lower().find(tag_name + " src")

        while i != -1:

            start = text.find("\"", i)
            end   = text.find("\"", start+1)

            if end == -1:
                return text

            filename = text[start+1:end]

            if filename == self.last_filename:
                return text
            
            try:
                os.system(tag_program + " " + filename)
            except:
                print "Unable to open", filename

            self.last_filename = filename

            i = text.lower().find(tag_name + " src", i+1)

        return text

p = CustomTag()
p.load()

