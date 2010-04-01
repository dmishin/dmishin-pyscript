from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'activate_categories_frm.ui'
#
# Created: Sun Apr 5 08:11:32 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class ActivateCategoriesFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("ActivateCategoriesFrm")


        ActivateCategoriesFrmLayout = QHBoxLayout(self,11,6,"ActivateCategoriesFrmLayout")

        layout10 = QVBoxLayout(None,0,6,"layout10")

        self.textLabel4 = QLabel(self,"textLabel4")
        layout10.addWidget(self.textLabel4)

        self.cat_list = QListBox(self,"cat_list")
        self.cat_list.setSelectionMode(QListBox.Extended)
        layout10.addWidget(self.cat_list)

        layout8 = QHBoxLayout(None,0,6,"layout8")

        self.button_none = QPushButton(self,"button_none")
        layout8.addWidget(self.button_none)

        self.button_all = QPushButton(self,"button_all")
        self.button_all.setAutoDefault(0)
        layout8.addWidget(self.button_all)
        layout10.addLayout(layout8)

        layout31 = QHBoxLayout(None,0,6,"layout31")

        self.button_ok = QPushButton(self,"button_ok")
        self.button_ok.setAutoDefault(0)
        self.button_ok.setDefault(1)
        layout31.addWidget(self.button_ok)
        spacer3 = QSpacerItem(211,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout31.addItem(spacer3)

        self.cancel_button = QPushButton(self,"cancel_button")
        self.cancel_button.setDefault(1)
        layout31.addWidget(self.cancel_button)
        layout10.addLayout(layout31)
        ActivateCategoriesFrmLayout.addLayout(layout10)

        self.languageChange()

        self.resize(QSize(248,315).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.cancel_button,SIGNAL("clicked()"),self.close)

        self.setTabOrder(self.button_ok,self.cancel_button)
        self.setTabOrder(self.cancel_button,self.cat_list)
        self.setTabOrder(self.cat_list,self.button_all)


    def languageChange(self):
        self.setCaption(self.__tr("Activate categories"))
        self.textLabel4.setText(self.__tr("Select the categories to activate:"))
        self.button_none.setText(self.__tr("Activate none"))
        self.button_all.setText(self.__tr("Activate all"))
        self.button_ok.setText(self.__tr("&OK"))
        self.button_ok.setAccel(QKeySequence(self.__tr("Alt+O")))
        self.cancel_button.setText(self.__tr("&Cancel"))


    def __tr(self,s,c = None):
        return qApp.translate("ActivateCategoriesFrm",s,c)
