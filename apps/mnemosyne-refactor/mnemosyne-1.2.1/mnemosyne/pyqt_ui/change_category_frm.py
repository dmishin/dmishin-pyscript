from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'change_category_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class ChangeCategoryFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("ChangeCategoryFrm")


        ChangeCategoryFrmLayout = QVBoxLayout(self,11,6,"ChangeCategoryFrmLayout")

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

        layout17 = QHBoxLayout(None,0,6,"layout17")

        self.ok_button = QPushButton(self,"ok_button")
        layout17.addWidget(self.ok_button)
        spacer3 = QSpacerItem(50,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout17.addItem(spacer3)

        self.cancel = QPushButton(self,"cancel")
        self.cancel.setAutoDefault(0)
        layout17.addWidget(self.cancel)
        layout18.addLayout(layout17)
        ChangeCategoryFrmLayout.addLayout(layout18)

        self.languageChange()

        self.resize(QSize(352,81).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.cancel,SIGNAL("clicked()"),self.reject)

        self.setTabOrder(self.categories,self.ok_button)
        self.setTabOrder(self.ok_button,self.cancel)


    def languageChange(self):
        self.setCaption(self.__tr("Change category"))
        self.textLabel1.setText(self.__tr("Category:"))
        self.ok_button.setText(self.__tr("&OK"))
        self.ok_button.setAccel(QKeySequence(self.__tr("Alt+O")))
        self.cancel.setText(self.__tr("&Cancel"))
        self.cancel.setAccel(QKeySequence(self.__tr("Alt+C")))


    def __tr(self,s,c = None):
        return qApp.translate("ChangeCategoryFrm",s,c)
