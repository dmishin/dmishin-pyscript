from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'edit_items_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class EditItemsFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("EditItemsFrm")


        EditItemsFrmLayout = QVBoxLayout(self,11,6,"EditItemsFrmLayout")

        layout11 = QVBoxLayout(None,0,6,"layout11")

        layout16 = QHBoxLayout(None,0,6,"layout16")

        self.to_find = QLineEdit(self,"to_find")
        layout16.addWidget(self.to_find)

        self.find_button = QPushButton(self,"find_button")
        self.find_button.setDefault(1)
        layout16.addWidget(self.find_button)
        layout11.addLayout(layout16)

        self.textLabel1 = QLabel(self,"textLabel1")
        layout11.addWidget(self.textLabel1)

        self.item_list = QListView(self,"item_list")
        self.item_list.addColumn(self.__tr("Question"))
        self.item_list.addColumn(self.__tr("Answer"))
        self.item_list.addColumn(self.__tr("Category"))
        self.item_list.setMouseTracking(0)
        self.item_list.setSelectionMode(QListView.Extended)
        self.item_list.setAllColumnsShowFocus(1)
        self.item_list.setShowSortIndicator(1)
        self.item_list.setDefaultRenameAction(QListView.Accept)
        layout11.addWidget(self.item_list)

        layout8 = QHBoxLayout(None,0,6,"layout8")
        spacer1 = QSpacerItem(530,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout8.addItem(spacer1)

        self.close_button = QPushButton(self,"close_button")
        self.close_button.setAutoDefault(0)
        layout8.addWidget(self.close_button)
        layout11.addLayout(layout8)
        EditItemsFrmLayout.addLayout(layout11)

        self.languageChange()

        self.resize(QSize(776,473).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.setTabOrder(self.to_find,self.find_button)
        self.setTabOrder(self.find_button,self.item_list)
        self.setTabOrder(self.item_list,self.close_button)


    def languageChange(self):
        self.setCaption(self.__tr("Edit deck"))
        self.find_button.setText(self.__tr("&Find"))
        self.find_button.setAccel(QKeySequence(self.__tr("Alt+F")))
        self.textLabel1.setText(self.__tr("To edit a card, right-click on it."))
        self.item_list.header().setLabel(0,self.__tr("Question"))
        self.item_list.header().setLabel(1,self.__tr("Answer"))
        self.item_list.header().setLabel(2,self.__tr("Category"))
        self.close_button.setText(self.__tr("E&xit"))
        self.close_button.setAccel(QKeySequence(self.__tr("Alt+X")))


    def __tr(self,s,c = None):
        return qApp.translate("EditItemsFrm",s,c)
