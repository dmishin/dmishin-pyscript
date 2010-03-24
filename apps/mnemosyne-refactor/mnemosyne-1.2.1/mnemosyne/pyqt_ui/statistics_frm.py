from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'statistics_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class StatisticsFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("StatisticsFrm")

        self.setSizeGripEnabled(1)

        StatisticsFrmLayout = QVBoxLayout(self,11,6,"StatisticsFrmLayout")

        self.tabWidget = QTabWidget(self,"tabWidget")

        self.Widget8 = QWidget(self.tabWidget,"Widget8")
        Widget8Layout = QVBoxLayout(self.Widget8,11,6,"Widget8Layout")

        self.schedule_info = QTextBrowser(self.Widget8,"schedule_info")
        Widget8Layout.addWidget(self.schedule_info)
        self.tabWidget.insertTab(self.Widget8,QString.fromLatin1(""))

        self.Widget9 = QWidget(self.tabWidget,"Widget9")
        Widget9Layout = QVBoxLayout(self.Widget9,11,6,"Widget9Layout")

        self.grades_info = QTextBrowser(self.Widget9,"grades_info")
        Widget9Layout.addWidget(self.grades_info)
        self.tabWidget.insertTab(self.Widget9,QString.fromLatin1(""))

        self.TabPage = QWidget(self.tabWidget,"TabPage")
        TabPageLayout = QVBoxLayout(self.TabPage,11,6,"TabPageLayout")

        self.category_info = QListView(self.TabPage,"category_info")
        self.category_info.addColumn(self.__tr("Category"))
        self.category_info.addColumn(self.__tr("Cards"))
        TabPageLayout.addWidget(self.category_info)
        self.tabWidget.insertTab(self.TabPage,QString.fromLatin1(""))

        self.TabPage_2 = QWidget(self.tabWidget,"TabPage_2")
        TabPageLayout_2 = QVBoxLayout(self.TabPage_2,11,6,"TabPageLayout_2")

        self.current_info = QTextBrowser(self.TabPage_2,"current_info")
        TabPageLayout_2.addWidget(self.current_info)
        self.tabWidget.insertTab(self.TabPage_2,QString.fromLatin1(""))
        StatisticsFrmLayout.addWidget(self.tabWidget)

        Layout1 = QHBoxLayout(None,0,6,"Layout1")
        Horizontal_Spacing2 = QSpacerItem(20,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        Layout1.addItem(Horizontal_Spacing2)

        self.buttonOk = QPushButton(self,"buttonOk")
        self.buttonOk.setAutoDefault(1)
        self.buttonOk.setDefault(1)
        Layout1.addWidget(self.buttonOk)
        StatisticsFrmLayout.addLayout(Layout1)

        self.languageChange()

        self.resize(QSize(478,389).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.buttonOk,SIGNAL("clicked()"),self.accept)


    def languageChange(self):
        self.setCaption(self.__tr("Statistics"))
        self.tabWidget.changeTab(self.Widget8,self.__tr("Schedule"))
        self.tabWidget.changeTab(self.Widget9,self.__tr("Grades"))
        self.category_info.header().setLabel(0,self.__tr("Category"))
        self.category_info.header().setLabel(1,self.__tr("Cards"))
        self.tabWidget.changeTab(self.TabPage,self.__tr("Categories"))
        self.tabWidget.changeTab(self.TabPage_2,self.__tr("Current card"))
        self.buttonOk.setText(self.__tr("&OK"))
        self.buttonOk.setAccel(QKeySequence(QString.null))


    def __tr(self,s,c = None):
        return qApp.translate("StatisticsFrm",s,c)
