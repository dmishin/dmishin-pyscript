from qt import *

# This is a wrapper for the QLabel class which implements a scrollbar.
# "Christopher Gilbreth" <cngilbreth@gmail.com>

class ScrollLabel(QScrollView):
    def __init__(self, parent = None, name = None):
        QScrollView.__init__(self, parent, name)
        
        self.setResizePolicy(QScrollView.Manual)
        
        self.label = QLabel(None,"label")
        self.label.setPaletteBackgroundColor(QColor(255,255,255))
        self.label.setAlignment(Qt.AlignVCenter | Qt.AlignHCenter)
        
        self.addChild(self.label)
        self.setText("")

        layout = QVBoxLayout()
        layout.addWidget(self.label)
        #self.setLayout(layout)
    
    
    ##########################################################################
    #
    # resizeContents
    #
    ##########################################################################
    
    def resizeContents(self, width, height):
        size = QSize(width, height).expandedTo(self.label.minimumSizeHint())
        self.label.resize(size)
        QScrollView.resizeContents(self, size.width(), size.height())
    
    
    ##########################################################################
    #
    # viewportResizeEvent
    #
    ##########################################################################
        
    def viewportResizeEvent(self, event):
        self.resizeContents(self.visibleWidth(), self.visibleHeight())
     
    ##########################################################################
    #
    # QLabel Wrapper Functions
    #
    ##########################################################################
    
    # These ones are actually used
    
    def setText(self, string):
		self.label.setText(string)
		self.label.adjustSize()
		self.viewportResizeEvent(None)
     
    def setFont(self, font):
        self.label.setFont(font)
        
    def setAlignment(self, alignment):
        self.label.setAlignment(alignment)
    
    # Others (might as well put them in)
    
    def text(self):
        return self.label.text()
    
    def pixmap(self):
        return self.label.pixmap()
    
    def picture(self):
        return self.label.picture()
        
    def movie(self):
        return self.label.movie()
        
    def textFormat(self):
        return self.label.textFormat()
        
    def setTextFormat(self, format):
        self.label.setTextFormat(format)
        
    def alignment(self):
        return self.label.alignment()
        
    def indent(self):
        return self.label.indent()
        
    def setIndent(self, indent):
        self.label.setIndent(indent)
        
    def hasScaledContents(self):
        return self.label.hasScaledContents()
        
    def setScaledContents(self, scale):
        self.label.setScaledContents(scale)
    
    def setPixmap(self, pixmap):
        self.label.setPixmap(pixmap)
        
    def setPicture(self, picture):
        self.label.setPicture(picture)
        
    def setMove(self, movie):
        self.label.setMove(movie)
        
    def setNum(self, num):
        self.label.setNum(num)
        
    
    
    
