from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'add_items_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class AddItemsFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("AddItemsFrm")


        AddItemsFrmLayout = QVBoxLayout(self,11,6,"AddItemsFrmLayout")

        layout8 = QVBoxLayout(None,0,6,"layout8")

        layout1 = QHBoxLayout(None,0,6,"layout1")

        self.textLabel1 = QLabel(self,"textLabel1")
        layout1.addWidget(self.textLabel1)

        self.categories = QComboBox(0,self,"categories")
        self.categories.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.Fixed,0,0,self.categories.sizePolicy().hasHeightForWidth()))
        self.categories.setEditable(1)
        self.categories.setAutoCompletion(1)
        self.categories.setDuplicatesEnabled(0)
        layout1.addWidget(self.categories)
        layout8.addLayout(layout1)

        self.q_label = QLabel(self,"q_label")
        layout8.addWidget(self.q_label)

        self.question = QTextEdit2(self,"question")
        self.question.setAcceptDrops(1)
        self.question.setTextFormat(QTextEdit.PlainText)
        layout8.addWidget(self.question)

        self.p_label = QLabel(self,"p_label")
        self.p_label.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.MinimumExpanding,0,0,self.p_label.sizePolicy().hasHeightForWidth()))
        layout8.addWidget(self.p_label)

        self.pronunciation = QTextEdit2(self,"pronunciation")
        self.pronunciation.setAcceptDrops(1)
        self.pronunciation.setTextFormat(QTextEdit.PlainText)
        layout8.addWidget(self.pronunciation)

        self.a_label = QLabel(self,"a_label")
        layout8.addWidget(self.a_label)

        self.answer = QTextEdit2(self,"answer")
        self.answer.setAcceptDrops(1)
        self.answer.setTextFormat(QTextEdit.PlainText)
        layout8.addWidget(self.answer)

        self.addViceVersa = QCheckBox(self,"addViceVersa")
        layout8.addWidget(self.addViceVersa)

        self.grades = QButtonGroup(self,"grades")
        self.grades.setSizePolicy(QSizePolicy(QSizePolicy.Preferred,QSizePolicy.Preferred,0,0,self.grades.sizePolicy().hasHeightForWidth()))
        self.grades.setAlignment(QButtonGroup.AlignVCenter | QButtonGroup.AlignLeft)
        self.grades.setRadioButtonExclusive(0)
        self.grades.setColumnLayout(0,Qt.Vertical)
        self.grades.layout().setSpacing(6)
        self.grades.layout().setMargin(11)
        gradesLayout = QHBoxLayout(self.grades.layout())
        gradesLayout.setAlignment(Qt.AlignTop)

        layout9 = QHBoxLayout(None,0,6,"layout9")

        self.pushButton1 = QPushButton(self.grades,"pushButton1")
        self.pushButton1.setAutoDefault(0)
        layout9.addWidget(self.pushButton1)

        self.pushButton1_2 = QPushButton(self.grades,"pushButton1_2")
        self.pushButton1_2.setAutoDefault(0)
        layout9.addWidget(self.pushButton1_2)

        self.line2 = QFrame(self.grades,"line2")
        self.line2.setPaletteForegroundColor(QColor(173,173,173))
        self.line2.setFrameShape(QFrame.VLine)
        self.line2.setFrameShadow(QFrame.Plain)
        self.line2.setFrameShape(QFrame.VLine)
        layout9.addWidget(self.line2)

        self.pushButton1_3 = QPushButton(self.grades,"pushButton1_3")
        self.pushButton1_3.setAutoDefault(0)
        layout9.addWidget(self.pushButton1_3)

        self.pushButton1_4 = QPushButton(self.grades,"pushButton1_4")
        self.pushButton1_4.setAutoDefault(0)
        layout9.addWidget(self.pushButton1_4)

        self.pushButton1_5 = QPushButton(self.grades,"pushButton1_5")
        self.pushButton1_5.setAutoDefault(0)
        layout9.addWidget(self.pushButton1_5)

        self.pushButton1_6 = QPushButton(self.grades,"pushButton1_6")
        self.pushButton1_6.setAutoDefault(0)
        layout9.addWidget(self.pushButton1_6)
        gradesLayout.addLayout(layout9)
        layout8.addWidget(self.grades)

        self.help_label = QLabel(self,"help_label")
        layout8.addWidget(self.help_label)

        layout5 = QHBoxLayout(None,0,6,"layout5")

        self.preview_button = QPushButton(self,"preview_button")
        self.preview_button.setAutoDefault(0)
        layout5.addWidget(self.preview_button)
        spacer1 = QSpacerItem(171,31,QSizePolicy.Expanding,QSizePolicy.Minimum)
        layout5.addItem(spacer1)

        self.cancel = QPushButton(self,"cancel")
        self.cancel.setAutoDefault(0)
        layout5.addWidget(self.cancel)
        layout8.addLayout(layout5)
        AddItemsFrmLayout.addLayout(layout8)

        self.languageChange()

        self.resize(QSize(339,471).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.cancel,SIGNAL("clicked()"),self.reject)

        self.setTabOrder(self.categories,self.question)
        self.setTabOrder(self.question,self.pronunciation)
        self.setTabOrder(self.pronunciation,self.answer)
        self.setTabOrder(self.answer,self.addViceVersa)
        self.setTabOrder(self.addViceVersa,self.pushButton1)
        self.setTabOrder(self.pushButton1,self.pushButton1_2)
        self.setTabOrder(self.pushButton1_2,self.pushButton1_3)
        self.setTabOrder(self.pushButton1_3,self.pushButton1_4)
        self.setTabOrder(self.pushButton1_4,self.pushButton1_5)
        self.setTabOrder(self.pushButton1_5,self.pushButton1_6)
        self.setTabOrder(self.pushButton1_6,self.preview_button)
        self.setTabOrder(self.preview_button,self.cancel)


    def languageChange(self):
        self.setCaption(self.__tr("Add cards"))
        self.textLabel1.setText(self.__tr("Category:"))
        self.q_label.setText(self.__tr("Question:"))
        self.p_label.setText(self.__tr("Pronunciation:"))
        self.a_label.setText(self.__tr("Answer:"))
        self.addViceVersa.setText(self.__tr("Add &vice versa too"))
        self.addViceVersa.setAccel(QKeySequence(self.__tr("Alt+V")))
        self.grades.setTitle(self.__tr("Select initial grade:"))
        self.pushButton1.setText(self.__tr("&0"))
        self.pushButton1.setAccel(QKeySequence(self.__tr("Ctrl+0")))
        QToolTip.add(self.pushButton1,self.__tr("You don't remember this item yet or forgot it."))
        self.pushButton1_2.setText(self.__tr("&1"))
        self.pushButton1_2.setAccel(QKeySequence(self.__tr("Ctrl+1")))
        QToolTip.add(self.pushButton1_2,self.__tr("Like '0', but the item is getting more familiar."))
        self.pushButton1_3.setText(self.__tr("&2"))
        self.pushButton1_3.setAccel(QKeySequence(self.__tr("Ctrl+2")))
        QToolTip.add(self.pushButton1_3,self.__tr("You've memorised this item now, and will probably remember it for a few days."))
        self.pushButton1_4.setText(self.__tr("&3"))
        self.pushButton1_4.setAccel(QKeySequence(self.__tr("Ctrl+3")))
        QToolTip.add(self.pushButton1_4,self.__tr("Correct answer, but with much effort. The interval was probably too long"))
        self.pushButton1_5.setText(self.__tr("&4"))
        self.pushButton1_5.setAccel(QKeySequence(self.__tr("Ctrl+4")))
        QToolTip.add(self.pushButton1_5,self.__tr("Correct answer, with some effort. The interval was probably just right."))
        self.pushButton1_6.setText(self.__tr("&5"))
        self.pushButton1_6.setAccel(QKeySequence(self.__tr("Ctrl+5")))
        QToolTip.add(self.pushButton1_6,self.__tr("Correct answer, but without any difficulties. The interval was probably too short."))
        self.help_label.setText(self.__tr("Type question and answer, then pick initial grade."))
        self.preview_button.setText(self.__tr("Preview"))
        self.preview_button.setAccel(QKeySequence(self.__tr("Ctrl+P")))
        self.cancel.setText(self.__tr("E&xit"))
        self.cancel.setAccel(QKeySequence(self.__tr("Alt+X")))


    def __tr(self,s,c = None):
        return qApp.translate("AddItemsFrm",s,c)
