##############################################################################
#
# Support for simple card attributes <timbob@bigpond.com>
# Based on Peter Bienstman's sound support module.
#
##############################################################################

import re

##############################################################################
#
# card_props
#
#  Extract properties from a <card style=".."/> tag and return them
#  as a dictionary. Property pairs are delimited by a semi-colon. Names are
#  separated by values with a colon. e.g.
#	<card style="overlay: true; background: red"/>
#
##############################################################################

re_card_props = re.compile(r'<card\s+style\s*=\s*"([^"]*)"\s*/?>')
def card_props(text):

    d = {
	    "answerbox" : "normal"
	}
    m = re_card_props.search(text)

    if m != None:
	try: stylestr = m.group(1)
	except IndexError: stylestr = ""

	stylepairs = stylestr.split(";")
	d = {}
	for sp in stylepairs:
            try:
                name, value = sp.split(":",1)
		d[name.strip().lower()] = value.strip()
            except:
                pass

    return d

