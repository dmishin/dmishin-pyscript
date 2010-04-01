##############################################################################
#
# always_shuffle_queue.py <Peter.Bienstman@UGent.be>
#
##############################################################################

from mnemosyne.core import *
import random


##############################################################################
#
# Plugin to always shuffle the queue, and override the prioritising for
# scheduled cards.
#
##############################################################################

class Shuffle(Plugin):

    def description(self):
        return "Always shuffle scheduled cards"

    def load(self):
        mnemosyne.core.register_function_hook("after_scheduled_queue",
                                              self.shuffle)

    def unload(self):
        mnemosyne.core.unregister_function_hook("after_scheduled_queue",
                                                self.shuffle)

    def shuffle(self, revision_queue):
        random.shuffle(revision_queue)
        return revision_queue

p = Shuffle()
p.load()
