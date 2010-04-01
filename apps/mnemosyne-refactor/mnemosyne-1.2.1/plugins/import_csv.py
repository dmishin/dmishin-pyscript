##############################################################################
#
# import_csv.py <Peter.Bienstman@UGent.be>
#
##############################################################################

from mnemosyne.core import *



##############################################################################
#
# import_cvs
#
#   Question and answers an a single line, separated by a comma.
#
##############################################################################

def import_csv(filename, default_cat, reset_learning_data=False):
    
    global items

    imported_items = []

    # Parse txt file.

    avg_easiness = average_easiness()

    f = None
    try:
        f = file(filename)
    except:
        try:
            f = file(filename.encode("latin"))
        except:
            raise LoadError()
    
    for line in f:
        
        try:
            line = unicode(line, "utf-8")
        except:
            try:
                line = unicode(line, "latin")
            except:
                raise EncodingError()

        line = line.rstrip()
        line = process_html_unicode(line)
        
        if len(line) == 0:
            continue

        if line[0] == u'\ufeff': # Microsoft Word unicode export oddity.
            line = line[1:]

        parts = line.split(',')

        try:
            q, a = parts[0].lstrip(), parts[1].lstrip()
            
            item = Item()

            item.q = q
            item.a = a         
        
            item.easiness = avg_easiness
            item.cat = default_cat
            item.new_id()
                    
            imported_items.append(item)
        except:
            raise MissingAnswerError(info=line)

    return imported_items



##############################################################################
#
# Plugin to register and unregister this fileformat.
#
##############################################################################

class ImportCSV(Plugin):

	def description(self):
		return "Import CSV files."

	def load(self):
		register_file_format("Comma separated values",
                     filter="Text files (*.csv *.CVS)",
                     import_function=import_csv,
                     export_function=False)

	def unload(self):
		unregister_file_format("Comma separated values")

p = ImportCSV()
p.load()

