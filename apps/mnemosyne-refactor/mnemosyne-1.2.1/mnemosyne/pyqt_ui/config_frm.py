from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'config_frm.ui'
#
# Created: Sun Apr 5 08:11:32 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class ConfigurationFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("ConfigurationFrm")

        self.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.MinimumExpanding,0,0,self.sizePolicy().hasHeightForWidth()))
        self.setSizeGripEnabled(1)

        ConfigurationFrmLayout = QVBoxLayout(self,11,6,"ConfigurationFrmLayout")

        layout14 = QVBoxLayout(None,0,6,"layout14")

        self.textLabel4 = QLabel(self,"textLabel4")
        self.textLabel4.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.MinimumExpanding,0,0,self.textLabel4.sizePolicy().hasHeightForWidth()))
        self.textLabel4.setAlignment(QLabel.AlignVCenter)
        layout14.addWidget(self.textLabel4)

        self.buttonGroup1 = QButtonGroup(self,"buttonGroup1")
        self.buttonGroup1.setColumnLayout(0,Qt.Vertical)
        self.buttonGroup1.layout().setSpacing(6)
        self.buttonGroup1.layout().setMargin(11)
        buttonGroup1Layout = QVBoxLayout(self.buttonGroup1.layout())
        buttonGroup1Layout.setAlignment(Qt.AlignTop)

        layout15 = QVBoxLayout(None,0,6,"layout15")

        layout17 = QHBoxLayout(None,0,6,"layout17")

        self.button_QA_font = QPushButton(self.buttonGroup1,"button_QA_font")
        layout17.addWidget(self.button_QA_font)

        self.button_list_font = QPushButton(self.buttonGroup1,"button_list_font")
        layout17.addWidget(self.button_list_font)
        layout15.addLayout(layout17)

        layout11 = QHBoxLayout(None,0,6,"layout11")

        self.textLabel1_2 = QLabel(self.buttonGroup1,"textLabel1_2")
        layout11.addWidget(self.textLabel1_2)

        self.font_increase = QSpinBox(self.buttonGroup1,"font_increase")
        self.font_increase.setMaxValue(200)
        self.font_increase.setLineStep(1)
        self.font_increase.setValue(0)
        layout11.addWidget(self.font_increase)

        self.textLabel1_2_2 = QLabel(self.buttonGroup1,"textLabel1_2_2")
        layout11.addWidget(self.textLabel1_2_2)
        layout15.addLayout(layout11)
        buttonGroup1Layout.addLayout(layout15)
        layout14.addWidget(self.buttonGroup1)

        self.buttonGroup2 = QButtonGroup(self,"buttonGroup2")
        self.buttonGroup2.setColumnLayout(0,Qt.Vertical)
        self.buttonGroup2.layout().setSpacing(6)
        self.buttonGroup2.layout().setMargin(11)
        buttonGroup2Layout = QHBoxLayout(self.buttonGroup2.layout())
        buttonGroup2Layout.setAlignment(Qt.AlignTop)

        layout24 = QVBoxLayout(None,0,6,"layout24")

        layout22 = QHBoxLayout(None,0,6,"layout22")

        self.uploadLogs = QCheckBox(self.buttonGroup2,"uploadLogs")
        layout22.addWidget(self.uploadLogs)
        layout24.addLayout(layout22)

        layout23 = QHBoxLayout(None,0,6,"layout23")
        layout24.addLayout(layout23)
        buttonGroup2Layout.addLayout(layout24)
        layout14.addWidget(self.buttonGroup2)

        self.buttonGroup3 = QButtonGroup(self,"buttonGroup3")
        self.buttonGroup3.setColumnLayout(0,Qt.Vertical)
        self.buttonGroup3.layout().setSpacing(6)
        self.buttonGroup3.layout().setMargin(11)
        buttonGroup3Layout = QHBoxLayout(self.buttonGroup3.layout())
        buttonGroup3Layout.setAlignment(Qt.AlignTop)

        layout20 = QVBoxLayout(None,0,6,"layout20")

        self.checkDuplicates = QCheckBox(self.buttonGroup3,"checkDuplicates")
        layout20.addWidget(self.checkDuplicates)

        self.duplicatesCats = QCheckBox(self.buttonGroup3,"duplicatesCats")
        layout20.addWidget(self.duplicatesCats)
        buttonGroup3Layout.addLayout(layout20)
        layout14.addWidget(self.buttonGroup3)

        self.buttonGroup4 = QButtonGroup(self,"buttonGroup4")
        self.buttonGroup4.setColumnLayout(0,Qt.Vertical)
        self.buttonGroup4.layout().setSpacing(6)
        self.buttonGroup4.layout().setMargin(11)
        buttonGroup4Layout = QVBoxLayout(self.buttonGroup4.layout())
        buttonGroup4Layout.setAlignment(Qt.AlignTop)

        layout13 = QHBoxLayout(None,0,6,"layout13")

        self.textLabel1 = QLabel(self.buttonGroup4,"textLabel1")
        layout13.addWidget(self.textLabel1)

        self.grade_0_items = QSpinBox(self.buttonGroup4,"grade_0_items")
        self.grade_0_items.setMaxValue(100)
        self.grade_0_items.setLineStep(5)
        self.grade_0_items.setValue(5)
        layout13.addWidget(self.grade_0_items)
        buttonGroup4Layout.addLayout(layout13)

        self.randomise = QCheckBox(self.buttonGroup4,"randomise")
        buttonGroup4Layout.addWidget(self.randomise)
        layout14.addWidget(self.buttonGroup4)

        layout30 = QHBoxLayout(None,0,6,"layout30")

        self.button_ok = QPushButton(self,"button_ok")
        self.button_ok.setAutoDefault(1)
        self.button_ok.setDefault(1)
        layout30.addWidget(self.button_ok)
        spacer8 = QSpacerItem(180,20,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout30.addItem(spacer8)

        self.button_defaults = QPushButton(self,"button_defaults")
        layout30.addWidget(self.button_defaults)

        self.button_cancel = QPushButton(self,"button_cancel")
        self.button_cancel.setAutoDefault(1)
        layout30.addWidget(self.button_cancel)
        layout14.addLayout(layout30)
        ConfigurationFrmLayout.addLayout(layout14)

        self.languageChange()

        self.resize(QSize(428,411).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.button_ok,SIGNAL("clicked()"),self.accept)
        self.connect(self.button_cancel,SIGNAL("clicked()"),self.reject)


    def languageChange(self):
        self.setCaption(self.__tr("Mnemosyne configuration"))
        self.textLabel4.setText(self.__tr("Hover with the mouse over the buttons to get extra information."))
        self.buttonGroup1.setTitle(self.__tr("Appearance"))
        self.button_QA_font.setText(self.__tr("Set Q/A font"))
        QToolTip.add(self.button_QA_font,self.__tr("Set the font used when showing single questions and answers"))
        self.button_list_font.setText(self.__tr("Set list font"))
        QToolTip.add(self.button_list_font,self.__tr("Set the font used when showing the list of all the cards in the deck"))
        self.textLabel1_2.setText(self.__tr("Increase size of non-latin characters by"))
        QToolTip.add(self.textLabel1_2,self.__tr("Sometimes complex glyphs are hard to read at the default latin font size"))
        QToolTip.add(self.font_increase,self.__tr("Sometimes complex glyphs are hard to read at the default latin font size"))
        self.textLabel1_2_2.setText(self.__tr("point(s)"))
        QToolTip.add(self.textLabel1_2_2,self.__tr("Sometimes complex glyphs are hard to read at the default latin font size"))
        self.buttonGroup2.setTitle(self.__tr("Log uploading"))
        self.uploadLogs.setText(self.__tr("Upload anonymous logs"))
        QToolTip.add(self.uploadLogs,self.__tr("Share anonymous statistics to contribute to memory research"))
        self.buttonGroup3.setTitle(self.__tr("Duplicate handling"))
        self.checkDuplicates.setText(self.__tr("Check for duplicates when adding new cards"))
        self.duplicatesCats.setText(self.__tr("Allow duplicates in different categories"))
        self.buttonGroup4.setTitle(self.__tr("Repetition process"))
        self.textLabel1.setText(self.__tr("Number of grade 0 cards to hold in your hand:"))
        QToolTip.add(self.textLabel1,self.__tr("Note: this is NOT the number of grade 0 cards you want to memorise *per day*"))
        self.randomise.setText(self.__tr("Learn new cards in random order"))
        self.button_ok.setText(self.__tr("&OK"))
        self.button_ok.setAccel(QKeySequence(QString.null))
        self.button_defaults.setText(self.__tr("Defaults"))
        self.button_cancel.setText(self.__tr("&Cancel"))
        self.button_cancel.setAccel(QKeySequence(QString.null))


    def __tr(self,s,c = None):
        return qApp.translate("ConfigurationFrm",s,c)
