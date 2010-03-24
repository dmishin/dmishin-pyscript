from qtextedit2 import *
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'preview_item_frm.ui'
#
# Created: Sun Apr 5 08:11:33 2009
#      by: The PyQt User Interface Compiler (pyuic) 3.17.4
#
# WARNING! All changes made in this file will be lost!


from qt import *


class PreviewItemFrm(QDialog):
    def __init__(self,parent = None,name = None,modal = 0,fl = 0):
        QDialog.__init__(self,parent,name,modal,fl)

        if not name:
            self.setName("PreviewItemFrm")


        PreviewItemFrmLayout = QVBoxLayout(self,11,6,"PreviewItemFrmLayout")

        layout15 = QVBoxLayout(None,0,6,"layout15")

        self.question_label = QLabel(self,"question_label")
        layout15.addWidget(self.question_label)

        self.question = QLabel(self,"question")
        self.question.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.MinimumExpanding,0,0,self.question.sizePolicy().hasHeightForWidth()))
        self.question.setPaletteBackgroundColor(QColor(255,255,255))
        self.question.setFrameShape(QLabel.LineEditPanel)
        self.question.setFrameShadow(QLabel.Sunken)
        self.question.setTextFormat(QLabel.RichText)
        self.question.setScaledContents(1)
        self.question.setAlignment(QLabel.WordBreak | QLabel.AlignCenter)
        layout15.addWidget(self.question)

        self.answer_label = QLabel(self,"answer_label")
        layout15.addWidget(self.answer_label)

        self.answer = QLabel(self,"answer")
        self.answer.setSizePolicy(QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.MinimumExpanding,0,0,self.answer.sizePolicy().hasHeightForWidth()))
        self.answer.setPaletteBackgroundColor(QColor(255,255,255))
        self.answer.setFrameShape(QLabel.LineEditPanel)
        self.answer.setFrameShadow(QLabel.Sunken)
        self.answer.setTextFormat(QLabel.RichText)
        self.answer.setScaledContents(1)
        self.answer.setAlignment(QLabel.WordBreak | QLabel.AlignCenter)
        layout15.addWidget(self.answer)
        PreviewItemFrmLayout.addLayout(layout15)

        layout6 = QHBoxLayout(None,0,6,"layout6")

        self.ok_button = QPushButton(self,"ok_button")
        self.ok_button.setDefault(1)
        layout6.addWidget(self.ok_button)
        PreviewItemFrmLayout.addLayout(layout6)

        self.languageChange()

        self.resize(QSize(352,253).expandedTo(self.minimumSizeHint()))
        self.clearWState(Qt.WState_Polished)

        self.connect(self.ok_button,SIGNAL("clicked()"),self.accept)



    def languageChange(self):
        self.setCaption(self.__tr("Preview card"))
        self.question_label.setText(self.__tr("Question:"))
        self.question.setText(QString.null)
        self.answer_label.setText(self.__tr("Answer:"))
        self.answer.setText(QString.null)
        self.ok_button.setText(self.__tr("&OK"))
        self.ok_button.setAccel(QKeySequence(self.__tr("Alt+O")))


    def __tr(self,s,c = None):
        return qApp.translate("PreviewItemFrm",s,c)
