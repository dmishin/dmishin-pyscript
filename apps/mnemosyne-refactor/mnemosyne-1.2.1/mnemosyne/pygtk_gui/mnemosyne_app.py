

class GTranslator:
    def __init__(self, app):
        pass
    def load( self, qm_file, path ):
        pass

class MnemosyneApp:
    """
    """
    def installTranslator(self, translator):
        print "Installed translator:", translator
        self.translator = translator
        
    def __init__(self, argv):
        """
        """
        self.argv = argv
        self.translator = None
        
        
    def trUtf8(self, text ):
        return text

    def setMainWidget(self, wgt):
        self.main_widget = wgt
