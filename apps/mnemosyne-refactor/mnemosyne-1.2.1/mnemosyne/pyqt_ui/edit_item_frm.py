from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'edit_item_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class EditItemFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("EditItemFrm")


        EditItemFrmLayout = QVBoxLayout(self,11,6,"EditItemFrmLayout")

        layout18 = QVBoxLayout(None,0,6,"layout18")

        layout1 = QHBoxLayout(None,0,6,"layout1")

        self.textLabel1 = QLabel(self,"textLabel1")
        layout1.addWidget(self.textLabel1)

        self.categories = QComboBox(0,self,"categories")
        self.categories.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.Fixed,0,0,self.categories.sizePolicy().hasHeightForWidth()))
        self.categories.setEditable(1)
        self.categories.setAutoCompletion(1)
        self.categories.setDuplicatesEnabled(0)
        layout1.addWidget(self.categories)
        layout18.addLayout(layout1)

        self.textLabel2 = QLabel(self,"textLabel2")
        layout18.addWidget(self.textLabel2)

        self.question = QTextEdit2(self,"question")
        self.question.setAcceptDrops(1)
        self.question.setTextFormat(QTextEdit.PlainText)
        layout18.addWidget(self.question)

        self.textLabel2_2 = QLabel(self,"textLabel2_2")
        layout18.addWidget(self.textLabel2_2)

        self.answer = QTextEdit2(self,"answer")
        self.answer.setAcceptDrops(1)
        self.answer.setTextFormat(QTextEdit.PlainText)
        layout18.addWidget(self.answer)

        button_layout = QHBoxLayout(None,0,6,"button_layout")

        self.ok_button = QPushButton(self,"ok_button")
        button_layout.addWidget(self.ok_button)
        spacer2 = QSpacerItem(50,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        button_layout.addItem(spacer2)

        self.preview_button = QPushButton(self,"preview_button")
        button_layout.addWidget(self.preview_button)
        spacer3 = QSpacerItem(50,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        button_layout.addItem(spacer3)

        self.cancel = QPushButton(self,"cancel")
        self.cancel.setAutoDefault(0)
        button_layout.addWidget(self.cancel)
        layout18.addLayout(button_layout)
        EditItemFrmLayout.addLayout(layout18)

        self.languageChange()

        self.resize(QSize(352,307).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.cancel,SIGNAL("clicked()"),self.reject)

        self.setTabOrder(self.categories,self.question)
        self.setTabOrder(self.question,self.answer)
        self.setTabOrder(self.answer,self.ok_button)
        self.setTabOrder(self.ok_button,self.cancel)


    def languageChange(self):
        self.setCaption(self.__tr("Edit card"))
        self.textLabel1.setText(self.__tr("Category:"))
        self.textLabel2.setText(self.__tr("Question:"))
        self.textLabel2_2.setText(self.__tr("Answer:"))
        self.ok_button.setText(self.__tr("&OK"))
        self.ok_button.setAccel(QKeySequence(self.__tr("Alt+O")))
        self.preview_button.setText(self.__tr("&Preview"))
        self.preview_button.setAccel(QKeySequence(self.__tr("Ctrl+P")))
        self.cancel.setText(self.__tr("&Cancel"))
        self.cancel.setAccel(QKeySequence(self.__tr("Alt+C")))


    def __tr(self,s,c = None):
        return qApp.translate("EditItemFrm",s,c)
