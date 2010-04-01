##############################################################################
#
# last_grade.py <Peter.Bienstman@UGent.be>
#
##############################################################################

from mnemosyne.core import *



##############################################################################
#
# Plugin to append the last grade to the answer.
#
##############################################################################

class LastGrade(Plugin):

	def description(self):
		return "Append last grade to answer."

	def load(self):
		register_function_hook("filter_a", self.add_grade)

	def unload(self):
		unregister_function_hook("filter_a", self.add_grade)

	def add_grade(self, text, card):
		return text + "<br>Last grade: " + str(card.grade)

p = LastGrade()
p.load()
