##############################################################################
#
# Main widget for Mnemosyne <Peter.Bienstman@UGent.be>
#
##############################################################################

import sys, os
from gtk_import import *
from main_frm import *

# from import_dlg import *
# from export_dlg import *
# from add_items_dlg import *
# from edit_item_dlg import *
# from clean_duplicates import *
# from statistics_dlg import *
# from edit_items_dlg import *
# from activate_categories_dlg import *
# from config_dlg import *
# from product_tour_dlg import *
# from tip_dlg import *
# from about_dlg import *
# from sound import *
# from card_prop import *
from message_boxes import *
from mnemosyne.core import *

prefix = os.path.dirname(__file__)



##############################################################################
#
# Tooltip texts
#
##############################################################################

tooltip = [["","","","","",""],["","","","","",""]]

def install_tooltip_strings(self):

    global tooltip
    
    tooltip[0][0] = \
        self.trUtf8("You don't remember this card yet.")
    tooltip[0][1] = \
        self.trUtf8("Like '0', but it's getting more familiar.").append(\
        self.trUtf8(" Show it less often."))
    tooltip[0][2] = tooltip[0][3] = tooltip[0][4] = tooltip[0][5] = \
        self.trUtf8("You've memorised this card now,").append(\
        self.trUtf8(" and will probably remember it for a few days."))

    tooltip[1][0] = tooltip[1][1] = \
        self.trUtf8("You have forgotten this card completely.")
    tooltip[1][2] = \
        self.trUtf8("Barely correct answer. The interval was way too long.")
    tooltip[1][3] = \
        self.trUtf8("Correct answer, but with much effort.").append(\
        self.trUtf8(" The interval was probably too long."))
    tooltip[1][4] = \
        self.trUtf8("Correct answer, with some effort.").append(\
        self.trUtf8(" The interval was probably just right."))
    tooltip[1][5] = \
        self.trUtf8("Correct answer, but without any").append(\
        self.trUtf8(" difficulties. The interval was probably too short."))



##############################################################################
#
# MainDlg
#
##############################################################################

class MainDlg(MainFrm):

    ##########################################################################
    #
    # __init__
    #
    ##########################################################################
    
    def __init__(self):
        
        MainFrm.__init__(self)

        self.state = "EMPTY"
        self.item = None
        
        self.q_sound_played = False
        self.a_sound_played = False
        
        self.sched  = widget( "status_label_1")
        self.notmem = widget( "status_label_2")
        self.all    = widget( "status_label_3")
        
        self.grade_buttons = []

        for answer_idx in range(6):
            self.grade_buttons.append( self.wTree.get_widget("btn_answer_%d"%answer_idx) )

        try:
            run_plugins()
        except MnemosyneError, e:
            messagebox_errors(self, e)
                
        if filename == None:
            filename = get_config("path")
            
        try:
            load_database(filename)
        except MnemosyneError, e:
            messagebox_errors(self, LoadErrorCreateTmp())
            filename = os.path.join(os.path.split(filename)[0],"___TMP___.mem")
            new_database(filename)
        
        self.newQuestion()
        self.updateDialog()

#TODO: timer
#        self.timer = QTimer(self)
#        self.connect(self.timer, SIGNAL("timeout()"), soundmanager.update)
#        self.timer.start(250, False)

        
    ##########################################################################
    #
    # fileNew
    #
    ##########################################################################
    def __addFileTypeFilters( self, gtk_chooser_dialog ):
        flt = gtk.FileFilter()
        flt.set_name( "All files" )
        flt.add_pattern("*")
        gtk_chooser_dialog.add_filter( flt )
        
        flt = gtk.FileFilter()
        flt.set_name( "Mnemosyne files" )
        flt.add_pattern( "*.mem" )
        gtk_chooser_dialog.add_filter( flt )
        
    
    def fileNew(self):

        pause_thinking()

        out = self.showFileDialog( gtk.FILE_CHOOSER_ACTION_SAVE )

        if out != "":
            if out[-4:] != ".mem":
                out += ".mem"
                
            if os.path.exists(out):
                if not queryOverwriteFile(self, out):
                    unpause_thinking()
                    return

            unload_database()
            self.state = "EMPTY"
            self.item = None
            new_database(out)
            load_database(get_config("path"))

        self.updateDialog()
        
        unpause_thinking()
            
    ##########################################################################
    #
    # fileOpen
    #
    ##########################################################################
    def showFileDialog( self, dlg_type ):
        chooser = gtk.FileChooserDialog(title=self.trUtf8("Mnemosyne databases (*.mem)"), action=dlg_type,
                                  buttons=(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_OPEN,gtk.RESPONSE_OK))
        chooser.set_current_folder( get_basedir() )
        #Set the file filters...
        self.__addFileTypeFilters( chooser )

        resp = chooser.run()
        if resp == gtk.RESPONSE_OK:
            return = chooser.get_filename()
        else:
            return ""

    def fileOpen( self, widget ):

        pause_thinking()
                
        oldPath = expand_path(get_config("path"))

        out = self.showFileDialog( gtk.FILE_CHOOSER_ACTION_OPEN )

        if out != "":

            try:
                unload_database()
            except MnemosyneError, e:
                messagebox_errors(self, e)

            self.state = "EMPTY"
            self.item = None

            try:
                load_database(out)
            except MnemosyneError, e:
                messagebox_errors(self, e)
                unpause_thinking()
                return

            self.newQuestion()

        self.updateDialog()
        unpause_thinking()
                
    ##########################################################################
    #
    # fileSave
    #
    ##########################################################################
    
    def fileSave( self, widget ):

        pause_thinking()

        path = get_config("path")
        
        try:
            save_database(path)
        except MnemosyneError, e:
            messagebox_errors(self, e)

        unpause_thinking()

    ##########################################################################
    #
    # fileSaveAs
    #
    ##########################################################################

    def fileSaveAs( self, widget ):
        
        pause_thinking()

        oldPath = expand_path(get_config("path"))
        out = self.showFileDialog( gtk.FILE_CHOOSER_ACTION_SAVE )
                         
        if out != "":
            
            if out[-4:] != ".mem":
                out += ".mem"

            if os.path.exists(out) and out != get_config("path"):
                if not queryOverwriteFile(self, out):
                    unpause_thinking()
                    return
                
            try:
                save_database(out)
            except MnemosyneError, e:
                messagebox_errors(self, e)
                unpause_thinking()
                return            

        self.updateDialog()
        unpause_thinking()
            
    ##########################################################################
    #
    # Import
    #
    ##########################################################################

    def Import( self, widget ):

        pause_thinking()
        
        from xml.sax import saxutils, make_parser
        from xml.sax.handler import feature_namespaces

        dlg = ImportDlg(self)
        dlg.exec_loop()
         
        if self.item == None:
            self.newQuestion()

        self.updateDialog()
        unpause_thinking()
        
    ##########################################################################
    #
    # export
    #
    ##########################################################################

    def export( self, widget ):
        
        pause_thinking()

        dlg = ExportDlg(self)
        dlg.exec_loop()
                
        unpause_thinking()
        
    ##########################################################################
    #
    # fileExit
    #
    ##########################################################################

    def fileExit( self, widget ):
        
        self.close()
        
    ##########################################################################
    #
    # addItems
    #
    ##########################################################################

    def addItems( self, widget ):
        
        pause_thinking()
        
        dlg = AddItemsDlg(self)
        dlg.exec_loop()
        
        if self.item == None:
            self.newQuestion()
            
        self.updateDialog()
        unpause_thinking()
        
    ##########################################################################
    #
    # editItems
    #
    ##########################################################################
    
    def editItems( self, widget ):
        
        pause_thinking()
        
        dlg = EditItemsDlg(self)
        dlg.exec_loop()
        rebuild_revision_queue()
        
        if not in_revision_queue(self.item):
            self.newQuestion()
        else:
            remove_from_revision_queue(self.item) # It's already being asked.

        self.updateDialog()
        unpause_thinking()
        
    ##########################################################################
    #
    # cleanDuplicates
    #
    ##########################################################################
    
    def cleanDuplicates( self, widget ):
        
        pause_thinking()
        
        self.statusBar().message(self.trUtf8("Please wait..."))
        clean_duplicates(self)
        rebuild_revision_queue()
        
        if not in_revision_queue(self.item):
            self.newQuestion()
            
        self.updateDialog()
        unpause_thinking()

    ##########################################################################
    #
    # showStatistics
    #
    ##########################################################################
    
    def showStatistics( self, widget ):
        
        pause_thinking()
        dlg = StatisticsDlg(self.item, self)
        dlg.exec_loop()
        unpause_thinking()
        
    ##########################################################################
    #
    # editCurrentItem
    #
    ##########################################################################
    
    def editCurrentItem( self, widget ):
        
        pause_thinking()
        dlg = EditItemDlg(self.item, self)
        dlg.exec_loop()
        self.updateDialog()
        unpause_thinking()

    ##########################################################################
    #
    # deleteCurrentItem
    #
    ##########################################################################

    def deleteCurrentItem( self, widget ):
        
        pause_thinking()
        
        status = ShowMsgBox(None,
                    "Mnemosyne",
                    "Delete current card?",
                    (gtk.STOCK_YES, gtk.RESPONSE_YES, gtk.STOCK_NO, gtk.RESPONSE_NO) )
        
        if status == gtk.RESPONSE_YES:
            delete_item(self.item)
            self.newQuestion()
            
        self.updateDialog()
        unpause_thinking()

    ##########################################################################
    #
    # activateCategories
    #
    ##########################################################################

    def activateCategories( self, widget ):
        
        pause_thinking()
        
        dlg = ActivateCategoriesDlg(self)
        dlg.exec_loop()
        
        rebuild_revision_queue()
        if not in_revision_queue(self.item):
            self.newQuestion()
        else:
            remove_from_revision_queue(self.item) # It's already being asked.
            
        self.updateDialog()
        unpause_thinking()

    ##########################################################################
    #
    # showToolbar
    #
    ##########################################################################

    def showToolbar(self, active):
        
        pause_thinking()
        if active:
            set_config("hide_toolbar", False)
        else:
            set_config("hide_toolbar", True)
        self.updateDialog()
        unpause_thinking()
        
    ##########################################################################
    #
    # configuration
    #
    ##########################################################################

    def configuration( self, widget ):
        
        pause_thinking()
        dlg = ConfigurationDlg(self)
        dlg.exec_loop()

        rebuild_revision_queue()
        if not in_revision_queue(self.item):
            self.newQuestion()
        else:
            remove_from_revision_queue(self.item) # It's already being asked.
            
        self.updateDialog()
        unpause_thinking()
            
    ##########################################################################
    #
    # closeEvent
    #
    ##########################################################################

    def closeEvent(self, event):
        #TODO
        try:
            save_config()
            backup_database()
            unload_database()
        except MnemosyneError, e:
            messagebox_errors(self, e)
            event.ignore()
        else:
            event.accept()

    ##########################################################################
    #
    # productTour
    #
    ##########################################################################
    
    def productTour( self, widget ):
        
        pause_thinking()
        dlg = ProductTourDlg(self)
        dlg.exec_loop()
        unpause_thinking()
        
    ##########################################################################
    #
    # Tip
    #
    ##########################################################################
    
    def Tip( self, widget ):
        
        pause_thinking()
        dlg = TipDlg(self)
        dlg.exec_loop()
        unpause_thinking()
        
    ##########################################################################
    #
    # helpAbout
    #
    ##########################################################################
    
    def helpAbout( self, widget ):
        
        pause_thinking()
        dlg = AboutDlg(self)
        dlg.exec_loop()
        unpause_thinking()

    ##########################################################################
    #
    # newQuestion
    #
    ##########################################################################

    def newQuestion(self, learn_ahead = False):
        
        if number_of_items() == 0:
            self.state = "EMPTY"
            self.item = None
        else:
            self.item = get_new_question(learn_ahead)
            if self.item != None:
                self.state = "SELECT SHOW"
                self.style = card_props(self.item.filtered_q())
            else:
                self.state = "SELECT AHEAD"
                self.style = card_props("")

        self.q_sound_played = False
        self.a_sound_played = False
        
        start_thinking()

    ##########################################################################
    #
    # showAnswer
    #
    ##########################################################################

    def showAnswer( self, widget ):

        if self.state == "SELECT AHEAD":
            self.newQuestion(learn_ahead = True)
        else:
            stop_thinking()
            self.state = "SELECT GRADE"
        self.updateDialog()
        
    ##########################################################################
    #
    # gradeAnswer
    #
    ##########################################################################

    def gradeAnswer(self, grade):
        
        interval = process_answer(self.item, grade)
        self.newQuestion()
        self.updateDialog()

        if get_config("show_intervals") == "statusbar":
            #TODO
#            self.statusBar().message(self.trUtf8("Returns in ").append(\
#                str(interval)).append(self.trUtf8(" day(s).")))
            
    ##########################################################################
    #
    # next_rep_string
    #
    ##########################################################################

    def next_rep_string(self, days):
        
        if days == 0:
             return '\nNext repetition: today.'
        elif days == 1:
            return '\nNext repetition: tomorrow.'
        else: 
            return QString("\nNext repetition in %d days" %days )
        
    ##########################################################################
    #
    # updateDialog
    #
    ##########################################################################
    
    def updateDialog( self ):

        # Update caption.
        
        database_name = os.path.basename(get_config("path"))[:-4]
        caption_text = u"Mnemosyne - " + database_name
        self.setCaption(caption_text)

        # Update menu bar.

        if get_config("only_editable_when_answer_shown") == True:
            if self.item != None and self.state == "SELECT GRADE":
                self.editCurrentItemAction.setEnabled(True)
            else:
                self.editCurrentItemAction.setEnabled(False)
        else:
            if self.item != None:
                self.editCurrentItemAction.setEnabled(True)
            else:
                self.editCurrentItemAction.setEnabled(False)            
            
        self.deleteCurrentItemAction.setEnabled(self.item != None)
        self.editItemsAction.setEnabled(number_of_items() > 0)

        # Update toolbar.
        
        if get_config("hide_toolbar") == True:
            self.toolbar.hide()
            self.showToolbarAction.setOn(0)
        else:
            self.toolbar.show()
            self.showToolbarAction.setOn(1)

        # Update question and answer font.
 
#TODO       
#        if get_config("QA_font") != None:
#            font = QFont()
#            font.fromString(get_config("QA_font"))
#        else:
#            font = self.show_button.font()

#        self.question.setFont(font)
#        self.answer.setFont(font)

        # Size for non-latin characters.

        increase_non_latin = get_config("non_latin_font_size_increase")
        non_latin_size = font.pointSize() + increase_non_latin
        
        # Update question and answer alignment.
        
        if get_config("left_align") == True:
            alignment = Qt.AlignAuto    | Qt.AlignVCenter | Qt.WordBreak
        else:
            alignment = Qt.AlignHCenter | Qt.AlignVCenter | Qt.WordBreak

        self.question.setAlignment(alignment)
        self.answer.setAlignment(alignment)

        # Hide/show the question and answer boxes
        
        if self.state == "SELECT SHOW":
            self.question.show()
            self.question_label.show()
            if self.style["answerbox"] == "overlay":
                self.answer.hide()
                self.answer_label.hide()
        elif self.state == "SELECT GRADE":
            self.answer.show()
            self.answer_label.show()
            if self.style["answerbox"] == "overlay":
                self.question.hide()
                self.question_label.hide()
        else:
            self.question.show()
            self.question_label.show()
            self.answer.show()
            self.answer_label.show()
        
        # Update question label.
        
        question_label_text = self.trUtf8("Question:")
        if self.item!=None and self.item.cat.name!=self.trUtf8("<default>"):
            question_label_text += " " + self.item.cat.name
        self.question_label.setText(question_label_text)

        # Update question content.
        
        if self.item == None:
            self.question.setText("")
        else:
            text = self.item.filtered_q()

            if self.q_sound_played == False:
                play_sound(text)
                self.q_sound_played = True
                
            if increase_non_latin:
                text = set_non_latin_font_size(text, non_latin_size)

            self.question.setText(text)

        # Update answer content.
        
        if self.item == None or self.state == "SELECT SHOW":
            self.answer.setText("")
        else:
            text = self.item.filtered_a()

            if self.a_sound_played == False:
                play_sound(text)
                self.a_sound_played = True
                
            if increase_non_latin:
                text = set_non_latin_font_size(text, non_latin_size)
            self.answer.setText(text)

        # Update 'show answer' button.
        
        if self.state == "EMPTY":
            show_enabled, default, text = 0, 1, self.trUtf8("Show answer")
            grades_enabled = 0
        elif self.state == "SELECT SHOW":
            show_enabled, default, text = 1, 1, self.trUtf8("Show answer")
            grades_enabled = 0
        elif self.state == "SELECT GRADE":
            show_enabled, default, text = 0, 1, self.trUtf8("Show answer")
            grades_enabled = 1
        elif self.state == "SELECT AHEAD":
            show_enabled, default, text = 1, 0, \
                                     self.trUtf8("Learn ahead of schedule")
            grades_enabled = 0

        self.show_button.setText(text)
        self.show_button.setDefault(default)
        self.show_button.setEnabled(show_enabled)

        # Update grade buttons.

        if self.item != None and self.item.grade in [0,1]:
            i = 0 # Acquisition phase.
        else:
            i = 1 # Retention phase.     
            
        self.grade_4_button.setDefault(grades_enabled)
        self.grades.setEnabled(grades_enabled)
        
#        QToolTip.setWakeUpDelay(0)

        for grade in range(0,6):

            # Tooltip.
            
            QToolTip.remove(self.grade_buttons[grade])
            
            if self.state == "SELECT GRADE" and \
               get_config("show_intervals") == "tooltips":
                QToolTip.add(self.grade_buttons[grade],
                      tooltip[i][grade].
                      append(self.next_rep_string(process_answer(self.item,
                                                  grade, dry_run=True))))
            else:
                QToolTip.add(self.grade_buttons[grade], tooltip[i][grade])

            # Button text.
                    
            if self.state == "SELECT GRADE" and \
               get_config("show_intervals") == "buttons":
                self.grade_buttons[grade].setText(\
                        str(process_answer(self.item, grade, dry_run=True)))
                self.grades.setTitle(\
                    self.trUtf8("Pick days until next repetition:"))
            else:
                self.grade_buttons[grade].setText(str(grade))
                self.grades.setTitle(self.trUtf8("Grade your answer:"))

            self.grade_buttons[grade].setAccel(QKeySequence(str(grade)))

        # Update status bar.
        
        self.sched .setText(self.trUtf8("Scheduled: ").append(QString(\
                            str(scheduled_items()))))
        self.notmem.setText(self.trUtf8("Not memorised: ").append(QString(\
                            str(non_memorised_items()))))
        self.all   .setText(self.trUtf8("All: ").append(QString(\
                            str(active_items()))))

        if self.shrink == True:
            self.adjustSize()

    ##########################################################################
    #
    # replaySound
    #
    ##########################################################################

    def replaySound( self, widget ):
        play_sound(self.item.filtered_q())
        if self.state == "SELECT GRADE":
            play_sound(self.item.filtered_a())
            
    def trUtf8( self, text ):
        #TODO: translation
        return text
