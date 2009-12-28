from xml.sax import handler, make_parser

class MemDict:
	"Class, representing dictionary, stored in file"
	def __init__(self, name="memory"):
		self.data=dict()
		self.name=name

	def define(self,word):
		try:
			return self.data[word]
		except KeyError:
			return None
	
	def match(self,word):
		word=word.strip().lower()
		def finder():
			for key in self.data.keys():
				if key.find(word)!=-1:
					yield MemDicDef(self, key)
		return list(finder())
		
	def strategy(self, strat=None):
		"Sets or gets current search strategy. If parameter is none, strategy is getted, else setted"
		if strat==None: #get strategy
			return "substring" #the only strategy, supported by this dictionary
		else:#set strategy
			pass
	def get_name(self):
		return self.name

class MemDicDef:
	"Class, representing definition in in-memory dictionary"
	def __init__(self, dict, word):
		self.dict=dict
		self.word=word
		self.definition=None #definition is not loaded initially
	
	def get_text(self):
		"Retrieves definition text, if not loaded"
		if self.definition==None:
			self.definition=self.dict.define(self.word)
		return self.definition
	def get_word(self):
		"Returns word, the definition was get for"
		return self.word
		
	def __str__(self):
		return "\"%s\" in %s: [%s]"%(self.word, self.dict.get_name(), self.get())

class XMLDicthandler(handler.ContentHandler):
	def __init__(self,memdic):
		self.path=[]
		self.memdic=memdic
		
	def startElement(self, name, attrs):
		self.path.append(name)
		if self.path==["dic","item"]:
			self.word=""
			self.definition=""
		
	def endElement(self, name):
		if self.path[-1]!=name:
			raise ValueError, "XML file is corrupted"
		if self.path==["dic","item"]:
			self.memdic.data[self.word]=self.definition
			
		del self.path[-1]
		
	def characters(self, chrs):
		if self.path==["dic","item","def"]:
			self.definition+=chrs
		if self.path==["dic","item","word"]:
			self.word+=chrs
		
def load_xml_dic(file):
	"""
	Loads dictionary from XML file. Returns MemDict instance.\n
	Format is <dic><item><word>...</word><def>...</def></item></dic>\n
	throws ValueError, if the format is wrong
	"""
	memdic=MemDict()
	parser = make_parser()
	try:
		parser.setContentHandler(XMLDicthandler(memdic))
		parser.parse(file)
	except ValueError,msg:
		print "Argument error: ",msg
	return memdic



