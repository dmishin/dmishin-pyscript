import pygtk
pygtk.require('2.0')
import gtk
import memdic

class DictTextView(gtk.TextView):
	def __init__(self, buffer=None):
		gtk.TextView.__init__(self, buffer)
		self.connect("key-press-event",self.onKeyPress)
		self.set_editable(False)
	
	def onKeyPress(self,widget, evt, data=None):
		#print "answer key", evt
		if evt.type == gtk.gdk.KEY_PRESS:
			print "keypress", evt.string
		#	self.stop_emission("key-press-event")
		

class DictionaryApp:
	def __init__(self):
		#create controls
		self.createControls()
		
		self.bindEvents()
		self.dict=memdic.load_xml_dic("dic.xml")
		self.maxUnrollMatches=10
		
		
	def bindEvents(self):
		#window close event
		self.window.connect("delete_event",self.delete_event)
		
		#enter event
		self.input.connect("key-press-event",self.onKeyPress)
		
	def doDefine(self,word):
		definitions=self.dict.define(word)
		if definitions!=None:
			self.showText(definitions)
		else:
			#try to match:
			self.doMatch(word)
			self.showText("No definitions found")
			
	def doMatch(self, word):
		matches=self.dict.match(word) #getting list of matches
		
		if len(matches)==0:
			self.showText("No mathes")
		elif len(matches)<self.maxUnrollMatches:
			self.unrollMathes(matches)
		else: #too many mathes
			self.listMathes(matches)
		
	def unrollMathes(self, matches):
		"Shows mathes with their text. Used when number of mathes is not too big"
		
		def unrolled():
			for m in matches:
				yield "%s in %s\n%s"%(m.get_word(),m.dict.get_name(),m.get_text());
		
		self.showText("\n===============\n".join(unrolled()));
			
	def listMathes(self,matches):
		"Lists only matched words"
		
		dicname=""
		ans=[];
		for m in matches:
			if m.dict.get_name()!=dicname:
				dicname=m.dict.get_name()
				ans.append("Matched in %s:\n"%dicname)
			ans.append("\t%s\n"%m.get_word())
		self.showText("".join(matches))
	
		
	def showText(self, text):
		"Showing specified text in window"
		textBuffer = gtk.TextBuffer()
		textBuffer.set_text(text)
		self.answer.set_buffer(textBuffer)
	
	def onKeyPress(self, widget, evt, data=None):
		
		print "KeyPress: ", evt, data
		
		if widget==self.input:
			
			if gtk.gdk.keyval_from_name('Return')==evt.keyval:
				if evt.state & gtk.gdk.SHIFT_MASK:#if shift-enter
				#match
					self.doMatch(self.input.get_text())
				else:
					self.doDefine(self.input.get_text())				
	
	def delete_event(self, widget, event, data=None):
		gtk.main_quit()
		return False
		
	def createControls(self):
		self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		
		vbox=gtk.VBox(False,0)
		
		#self.answer=gtk.Button("Here would be dictionary answer")
		self.answer= DictTextView(buffer=None)
		self.answer.show()
		
		vbox.pack_start(self.answer, True, True, 0)
		
		hbox=gtk.HBox(False,0)
		
		prompt = gtk.Label(">>")
		hbox.pack_start(prompt,False, False, 0)
		
		self.input= gtk.Entry()
		hbox.pack_start(self.input,True, True, 0)
		
		vbox.pack_start(hbox, False, False,0)
		
		self.window.add(vbox)
		
		#showing
		hbox.show()
		vbox.show()
		self.answer.show()
		self.input.show()
		prompt.show()
		
		self.window.resize(300,600)
		


		self.input.grab_focus()
		self.window.show()
		
		
	def run(self):
		gtk.main()
		
d=DictionaryApp()
d.run()
