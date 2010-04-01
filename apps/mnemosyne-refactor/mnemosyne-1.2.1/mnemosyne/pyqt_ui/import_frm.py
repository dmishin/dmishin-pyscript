from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'import_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class ImportFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("ImportFrm")


        ImportFrmLayout = QVBoxLayout(self,11,6,"ImportFrmLayout")

        layout4 = QVBoxLayout(None,0,6,"layout4")

        self.textLabel_5 = QLabel(self,"textLabel_5")
        layout4.addWidget(self.textLabel_5)

        self.fileformats = QComboBox(0,self,"fileformats")
        self.fileformats.setEditable(0)
        self.fileformats.setAutoCompletion(1)
        self.fileformats.setDuplicatesEnabled(0)
        layout4.addWidget(self.fileformats)

        self.textLabel1 = QLabel(self,"textLabel1")
        layout4.addWidget(self.textLabel1)

        layout6 = QHBoxLayout(None,0,6,"layout6")

        self.filename = QLineEdit(self,"filename")
        layout6.addWidget(self.filename)

        self.browse_button = QPushButton(self,"browse_button")
        layout6.addWidget(self.browse_button)
        layout4.addLayout(layout6)

        self.textLabel2 = QLabel(self,"textLabel2")
        layout4.addWidget(self.textLabel2)

        self.categories = QComboBox(0,self,"categories")
        self.categories.setEditable(1)
        self.categories.setAutoCompletion(1)
        self.categories.setDuplicatesEnabled(0)
        layout4.addWidget(self.categories)

        self.textLabel3 = QLabel(self,"textLabel3")
        layout4.addWidget(self.textLabel3)

        self.reset_box = QCheckBox(self,"reset_box")
        layout4.addWidget(self.reset_box)

        layout5 = QHBoxLayout(None,0,6,"layout5")

        self.ok_button = QPushButton(self,"ok_button")
        self.ok_button.setDefault(1)
        layout5.addWidget(self.ok_button)
        spacer2 = QSpacerItem(201,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout5.addItem(spacer2)

        self.cancel_button = QPushButton(self,"cancel_button")
        self.cancel_button.setDefault(1)
        layout5.addWidget(self.cancel_button)
        layout4.addLayout(layout5)
        ImportFrmLayout.addLayout(layout4)

        self.languageChange()

        self.resize(QSize(396,322).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.cancel_button,SIGNAL("clicked()"),self.reject)

        self.setTabOrder(self.filename,self.browse_button)
        self.setTabOrder(self.browse_button,self.fileformats)
        self.setTabOrder(self.fileformats,self.categories)
        self.setTabOrder(self.categories,self.reset_box)
        self.setTabOrder(self.reset_box,self.ok_button)
        self.setTabOrder(self.ok_button,self.cancel_button)


    def languageChange(self):
        self.setCaption(self.__tr("Import from file"))
        self.textLabel_5.setText(self.__tr("File format:"))
        self.textLabel1.setText(self.__tr("File to import from:"))
        self.browse_button.setText(self.__tr("&Browse"))
        self.textLabel2.setText(self.__tr("Put cards without category into this category:"))
        self.textLabel3.setText(self.__tr("Note: all cards will be added to the current database."))
        self.reset_box.setText(self.__tr("Reset learning data on import"))
        self.ok_button.setText(self.__tr("&OK"))
        self.ok_button.setAccel(QKeySequence(self.__tr("Alt+O")))
        self.cancel_button.setText(self.__tr("&Cancel"))


    def __tr(self,s,c = None):
        return qApp.translate("ImportFrm",s,c)
