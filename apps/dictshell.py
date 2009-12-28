#!/usr/bin/python
# -*- coding: UTF-8 -*-
import pygtk
pygtk.require('2.0')
import gtk
import os
import re
import ConfigParser as CFG
import pickle

#TODO:
# [] Font zoom in/zoom out
# [] Font select
# [] Dictionary select
# [] Strategy select
# [] Native database client
# [] Store window size
# [] store history
# [] Font depends on dataase or server
# [] better text navigation

helptext="""Simple dictionary client.
keyboard shortcuts:
  Ctrl+H - show this help
  Enter (in the input field): Search word in the dictionaries
  Ctrl+Enter (in the input field): Match word in the dictionaries
  Ctrl+Enter (in the answer field): Copy selected text to the input field
  
  Ctrl+D - select active database
  Ctrl+S - select search strategy
  
  Ctrl+B - go back in the history
  Ctrl+F - go forth in the history
"""
ICON_PATH = "/home/dim/Documents/arrows1.png"


def run_command(cmd):
    "Run the OS command and read its output"
    try:
        stream = os.popen( cmd, "r" )
        answer = stream.read()
        stream.close()
        return answer
    except Exception, msg:
        return str(msg)

    

class DicServer:
    def __init__(self):
        self.dic_name_re = re.compile("^\s(\w+)\s+(\S.*\S)") #regexp for parsing dictionary names list
        
    def get_strategies(self):
        ans = run_command("dict -S")
        lines = ans.split("\n")
        strats=[]
        for ln in lines[1:]:
            match = self.dic_name_re.match(ln)
            if match:
                strats.append((match.group(1), match.group(2)))
        return strats
        
    def get_dictionaries(self):
        ans = run_command("dict -D")
        lines = ans.split("\n")
        dics=[]
        for ln in lines[1:]:
            match = self.dic_name_re.match(ln)
            if match:
                dics.append((match.group(1), match.group(2)))
        return dics
        
    def define(self, word, dictionary = None):
        if dictionary:
            return run_command('dict -d "%s" "%s"'%(dictionary, word))
        else:
            return run_command('dict "%s"'%word)
            
    def match(self, word, method="substring"):
        return run_command("dict -s %s -m \"%s\""%(method, word))
        
        
class PersistentData:
    def __init__(self):
        self.position=(200,200)
        self.size = (200, 200)
        
class MainApp:
    def __init__(self):
        
        self.config = CFG.RawConfigParser()
        self.load_persistent_data()
        
        self.dictionary = DicServer()
        dics = self.dictionary.get_dictionaries()
        print "Found %d dictionaries:"%len(dics)
        for dic,dic_name in dics:
            print "* %s [%s]"%(dic_name, dic)
        strats = self.dictionary.get_strategies()
        print "Server supports following search strategies:"
        for strat, strat_name in strats:
            print "* %s [%s]"%(strat_name, strat)
            
        self.history = list()
        self.max_history = 100
        self.current_history_position = -1
        
        # create a new window
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.move( *self.persistent.position )
        self.window.resize( *self.persistent.size )
        try:
            icon = gtk.gdk.pixbuf_new_from_file(ICON_PATH)
            self.window.set_icon(icon)
        except Exception, msg:
            print "Failed to load icon:", msg

        
        # When the window is given the "delete_event" signal (this is given
        # by the window manager, usually by the "close" option, or on the
        # titlebar), we ask it to call the delete_event () function
        # as defined above. The data passed to the callback
        # function is NULL and is ignored in the callback function.
        self.window.connect("delete_event", self.delete_event)
        
        # Here we connect the "destroy" event to a signal handler.  
        # This event occurs when we call gtk_widget_destroy() on the window,
        # or if we return FALSE in the "delete_event" callback.
        self.window.connect("destroy", self.destroy)
    
        # Sets the border width of the window.
        #self.window.set_border_width(5)
        
        self.create_ui()

    
        # and the window
        self.window.show()
        
    def load_persistent_data(self):
        data_path = os.path.expanduser('~/.pydictshell.persistent')
        try:
            fl = open( data_path, "r")
            self.persistent = pickle.load( fl )
            fl.close()
        except Exception, msg:
            print "Persistent data failed to load %s, using defaults"%msg
            self.persistent = PersistentData()

    def save_persistent_data(self):
        data_path = os.path.expanduser('~/.pydictshell.persistent')
        try:
            fl = open( data_path, "w")
            pickle.dump( self.persistent, fl )
            fl.close()
        except Exception, msg:
            print "Persistent data failed to save %s, using defaults"%msg

        
    def load_cfg(self):
        cfg_path = os.path.expanduser('~/.pydictshell')
        if os.path.exists( cfg_path ):
            self.config.read ( cfg_path )
                
    def main(self):
        # All PyGTK applications must have a gtk.main(). Control ends here
        # and waits for an event to occur (like a key press or mouse event).
        gtk.main()
        
    def delete_event(self, widget, event, data=None):
        # If you return FALSE in the "delete_event" signal handler,
        # GTK will emit the "destroy" signal. Returning TRUE means
        # you don't want the window to be destroyed.
        # This is useful for popping up 'are you sure you want to quit?'
        # type dialogs.
        print "delete event occurred"
        self.persistent.size = self.window.get_size()
        self.persistent.position = self.window.get_position()
        self.save_persistent_data()
        # Change FALSE to TRUE and the main window will not be destroyed
        # with a "delete_event".
        return False

    def destroy(self, widget, data=None):
        self.save_persistent_data()
        gtk.main_quit()

    def store_history(self, word, answer):
        item = (word, answer)
        if self.history:
            if self.history[-1] == item:
                #nothing to store
                return
        self.history.append( item )
        if len(self.history)> self.max_history:
            del self.history[0]
        self.current_history_position = -1
        
    def go_back(self):
        try:
            word, text = self.history[self.current_history_position]
            self.current_history_position -= 1
            self.textinput.set_text(word)
            if text:
                self.set_text(text)
            else:#if history contains no text, then re-request it. TODO: store request type too?
                self.do_exec()
                
        except IndexError:
            print "End of this history list reached"
            return
            
    def go_forth(self):
        if self.current_history_position >= -1:
            print "Can not go forth, already at the top"
            return
            
        try:
            self.current_history_position += 1            
            word, text = self.history[self.current_history_position]
            self.textinput.set_text(word)
            if text:
                self.set_text(text)
            else:#if history contains no text, then re-request it. TODO: store request type too?
                self.do_exec(word)
        except IndexError:
            print "Oupsie"
            return
        
    def create_ui(self):
        
        buffer = gtk.TextBuffer()
        buffer.set_text("Dictionary frontend")        
        self.text_buffer = buffer

        sw = gtk.ScrolledWindow()
        sw.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)

        textview = gtk.TextView(buffer)
        textview.set_editable( False )
        sw.add(textview)
        textview.show()
        
        
        box = gtk.VBox(False, 0)
        box.pack_start(sw, expand=True )
        sw.show()
        
        
        lower_box = gtk.HBox(False,0)
        if 1:
            textinput = gtk.Entry(max=0)
            lower_box.pack_start(textinput, expand= True)
            textinput.show()
                
        box.pack_start(lower_box,expand=False)
        lower_box.show()
        
        box.show()
        
        self.window.add(box)
        
        textinput.connect("key-press-event", self.do_enter, None)
        textview.connect("key-press-event", self.handle_answer_keypress, None)
        self.window.connect("key-press-event", self.global_keypress, None)
        
        textinput.grab_focus()
        self.textinput  = textinput
        self.textview = textview
    
    def global_keypress(self, widget, event, *data):
        print event, data
        if event.type == gtk.gdk.KEY_PRESS:            
            if event.state & gtk.gdk.CONTROL_MASK:
                if event.keyval == gtk.keysyms.h:
                    self.show_help()
                    return
                if event.keyval == gtk.keysyms.b:
                    self.go_back()
                    return
                    
                if event.keyval == gtk.keysyms.f:
                    self.go_forth()
                    return


    
    def show_help(self):
        self.set_text(helptext)
        
    def do_enter(self, widget, event, *data):
        #print event, data
        if event.type == gtk.gdk.KEY_PRESS:
            if event.keyval == gtk.keysyms.Return:
                if event.state & gtk.gdk.CONTROL_MASK:
                    self.do_exec_match()
                else:
                    self.do_exec()
            
                
    def do_exec_match(self):
        text = self.textinput.get_text()
        answer = self.dictionary.match( text.strip() ,"substring")
        self.set_text( answer )
        self.store_history( text, answer)
            
    def do_exec(self):
        text = self.textinput.get_text()
        answer = self.dictionary.define( text.strip() )
        self.set_text( answer )
        self.store_history( text, answer)
        
    def set_text(self, text):
        self.text_buffer.set_text( text)        
            
    def add_text(self, txt):
        startiter, enditer = self.text_buffer.get_bounds()
        self.text_buffer.insert( enditer, txt)
    
    def handle_answer_keypress(self, widget, event, *data):
        "Handler for the keypress events in the view"
        if event.type == gtk.gdk.KEY_PRESS:
            if event.keyval == gtk.keysyms.Tab:
                self.textinput.grab_focus()
                return True
                
            if event.keyval == gtk.keysyms.Return:
                if 1:# event.state & gtk.gdk.CONTROL_MASK:
                    sel = self.text_buffer.get_selection_bounds()
                    if sel:
                        word = self.text_buffer.get_text(sel[0], sel[1])
                        if word:
                            self.textinput.set_text(word)
                            self.textinput.grab_focus()
                    return True #to cancel the event
        

# If the program is run directly or passed as an argument to the python
# interpreter then create a HelloWorld instance and show it
if __name__ == "__main__":
    hello = MainApp()
    hello.main()
