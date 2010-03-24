from gtk_import import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'main_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!
class GAction:
    def __init__(self, handler=None):
        self.enabled = False
        self.handler = handler
        
    def setEnabled(self, val):
        self.enabled = not not val

    def isEnabled( self ):
        return self.enabled

    def __call__(self, *params ):
        if self.enabled and self.handler:
            self.handler( *params )

class MainFrm():
    def __init__(self):
        gladefile = "MnemosineGtk.glade"
        title = "wnd_main"
        self.wTree=gtk.glade.XML (gladefile, title)
        self.acExit=GAction( self.fileExit )

        self.editCurrentItemAction = GAction( self.editCurrentItem )
        self.deleteCurrentItemAction = GAction( self.deleteCurrentItem )
        self.editItemsAction = GAction( self.editItems )
        handlers = {
            "on_quit": self.acExit,
            "on_file_new": self.FileNew,
            "on_file_open": self.fileOpen,
            "on_file_saveas": self.fileSaveAs,
            "on_help_about": self.helpAbout,
            "on_file_import": self.Import,
            "on_file_export": self. export,
            "answer_clicked": self.on_answer,
            "on_wnd_main_destroy" : (gtk.mainquit),

            "on_mnu_edit_current_activate": self.editCurrentItemAction,
            "on_mnu_delete_current_activate": self.deleteCurrentItemAction,
            "on_mnu_edit_deck_activate": self.editItemsAction,
            }

        self.wTree.signal_autoconnect( handlers )
        
        def widget( name ):
            return self.wTree.get_widget( name )

        self.question = widget( "txt_question" )
        self.question_label = widget( "lbl_question" )
        self.answer = widget( "txt_answer" )
        self.answer_label = widget( "lbl_answer" )
        self.grade_4_button = widget( "btn_answer_4" )


    def setCaption( self, text ):
        self.wTree.get_widget("wnd_main").set_title( text )


    def fileNew( self, widget ):
        print "MainFrm.fileNew(): Not implemented yet"

    def fileOpen( self, widget ):
        print "MainFrm.fileOpen(): Not implemented yet"

    def fileSave( self, widget ):
        print "MainFrm.fileSave(): Not implemented yet"

    def fileSaveAs( self, widget ):
        print "MainFrm.fileSaveAs(): Not implemented yet"

    def fileExit( self, widget ):
        print "MainFrm.fileExit(): Not implemented yet"

    def helpAbout( self, widget ):
        print "MainFrm.helpAbout(): Not implemented yet"

    def addItems( self, widget ):
        print "MainFrm.addItems(): Not implemented yet"

    def Import( self, widget ):
        print "MainFrm.Import(): Not implemented yet"

    def export( self, widget ):
        print "MainFrm.export(): Not implemented yet"

    def editItems( self, widget ):
        print "MainFrm.editItems(): Not implemented yet"

    def cleanDuplicates( self, widget ):
        print "MainFrm.cleanDuplicates(): Not implemented yet"

    def editCurrentItem( self, widget ):
        print "MainFrm.editCurrentItem(): Not implemented yet"

    def deleteCurrentItem( self, widget ):
        print "MainFrm.deleteCurrentItem(): Not implemented yet"

    def activateCategories( self, widget ):
        print "MainFrm.activateCategories(): Not implemented yet"

    def showToolbar( self, widget ):
        print "MainFrm.showToolbar(): Not implemented yet"

    def configuration( self, widget ):
        print "MainFrm.configuration(): Not implemented yet"

    def productTour( self, widget ):
        print "MainFrm.productTour(): Not implemented yet"

    def showAnswer( self, widget ):
        print "MainFrm.showAnswer(): Not implemented yet"

    def gradeAnswer( self, widget ):
        print "MainFrm.gradeAnswer(): Not implemented yet"

    def nextQuestion( self, widget ):
        print "MainFrm.nextQuestion(): Not implemented yet"

    def learnAhead( self, widget ):
        print "MainFrm.learnAhead(): Not implemented yet"

    def replaySound( self, widget ):
        print "MainFrm.replaySound(): Not implemented yet"

    def showStatistics( self, widget ):
        print "MainFrm.showStatistics(): Not implemented yet"

    def Tip( self, widget ):
        print "MainFrm.Tip(): Not implemented yet"

    def __tr(self,s,c = None):
        print "Translate not supported yet"
