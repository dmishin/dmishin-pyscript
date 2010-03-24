from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'export_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class ExportFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("ExportFrm")


        ExportFrmLayout = QVBoxLayout(self,11,6,"ExportFrmLayout")

        layout8 = QVBoxLayout(None,0,6,"layout8")

        self.textLabel_5 = QLabel(self,"textLabel_5")
        layout8.addWidget(self.textLabel_5)

        self.fileformats = QComboBox(0,self,"fileformats")
        self.fileformats.setEditable(0)
        self.fileformats.setAutoCompletion(1)
        self.fileformats.setDuplicatesEnabled(0)
        layout8.addWidget(self.fileformats)

        self.textLabel1 = QLabel(self,"textLabel1")
        layout8.addWidget(self.textLabel1)

        layout22 = QHBoxLayout(None,0,6,"layout22")

        self.filename = QLineEdit(self,"filename")
        layout22.addWidget(self.filename)

        self.browse_button = QPushButton(self,"browse_button")
        layout22.addWidget(self.browse_button)
        layout8.addLayout(layout22)

        self.textLabel4 = QLabel(self,"textLabel4")
        layout8.addWidget(self.textLabel4)

        layout21 = QHBoxLayout(None,0,6,"layout21")

        self.categories = QListBox(self,"categories")
        self.categories.setSelectionMode(QListBox.Extended)
        layout21.addWidget(self.categories)

        self.all_button = QPushButton(self,"all_button")
        self.all_button.setAutoDefault(0)
        layout21.addWidget(self.all_button)
        layout8.addLayout(layout21)

        self.reset_box = QCheckBox(self,"reset_box")
        layout8.addWidget(self.reset_box)

        layout20 = QHBoxLayout(None,0,6,"layout20")

        self.ok_button = QPushButton(self,"ok_button")
        self.ok_button.setDefault(1)
        layout20.addWidget(self.ok_button)
        spacer2 = QSpacerItem(219,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout20.addItem(spacer2)

        self.cancel_button = QPushButton(self,"cancel_button")
        self.cancel_button.setDefault(1)
        layout20.addWidget(self.cancel_button)
        layout8.addLayout(layout20)
        ExportFrmLayout.addLayout(layout8)

        self.languageChange()

        self.resize(QSize(411,349).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.cancel_button,SIGNAL("clicked()"),self.reject)

        self.setTabOrder(self.filename,self.browse_button)
        self.setTabOrder(self.browse_button,self.categories)
        self.setTabOrder(self.categories,self.all_button)
        self.setTabOrder(self.all_button,self.ok_button)
        self.setTabOrder(self.ok_button,self.cancel_button)


    def languageChange(self):
        self.setCaption(self.__tr("Export to file"))
        self.textLabel_5.setText(self.__tr("File format:"))
        self.textLabel1.setText(self.__tr("File to export to:"))
        self.browse_button.setText(self.__tr("&Browse"))
        self.textLabel4.setText(self.__tr("Categories to export:"))
        self.all_button.setText(self.__tr("&Select all"))
        self.reset_box.setText(self.__tr("Reset learning data on export"))
        self.ok_button.setText(self.__tr("&OK"))
        self.ok_button.setAccel(QKeySequence(self.__tr("Alt+O")))
        self.cancel_button.setText(self.__tr("&Cancel"))


    def __tr(self,s,c = None):
        return qApp.translate("ExportFrm",s,c)
