# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'pjmass_select.ui'
#
# Created: Fri Oct 19 11:01:34 2012
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName(_fromUtf8("Dialog"))
        Dialog.resize(190, 522)
        self.label = QtGui.QLabel(Dialog)
        self.label.setGeometry(QtCore.QRect(10, 80, 62, 17))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(Dialog)
        self.label_2.setGeometry(QtCore.QRect(80, 80, 62, 17))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.EPICSAppTitleWidget = EPICSAppTitleWidget(Dialog)
        self.EPICSAppTitleWidget.setGeometry(QtCore.QRect(0, 0, 191, 41))
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.EPICSAppTitleWidget.sizePolicy().hasHeightForWidth())
        self.EPICSAppTitleWidget.setSizePolicy(sizePolicy)
        self.EPICSAppTitleWidget.setStyleSheet(_fromUtf8("background-color: rgb(202, 223, 159);"))
        self.EPICSAppTitleWidget.setAlignment(QtCore.Qt.AlignCenter)
        self.EPICSAppTitleWidget.setObjectName(_fromUtf8("EPICSAppTitleWidget"))
        self.label_3 = QtGui.QLabel(Dialog)
        self.label_3.setGeometry(QtCore.QRect(10, 50, 171, 17))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.layoutWidget = QtGui.QWidget(Dialog)
        self.layoutWidget.setGeometry(QtCore.QRect(11, 101, 124, 370))
        self.layoutWidget.setObjectName(_fromUtf8("layoutWidget"))
        self.horizontalLayout = QtGui.QHBoxLayout(self.layoutWidget)
        self.horizontalLayout.setMargin(0)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.labelPeak = QtGui.QLabel(self.layoutWidget)
        self.labelPeak.setObjectName(_fromUtf8("labelPeak"))
        self.verticalLayout.addWidget(self.labelPeak)
        self.labelPeak_2 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_2.setObjectName(_fromUtf8("labelPeak_2"))
        self.verticalLayout.addWidget(self.labelPeak_2)
        self.labelPeak_3 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_3.setObjectName(_fromUtf8("labelPeak_3"))
        self.verticalLayout.addWidget(self.labelPeak_3)
        self.labelPeak_4 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_4.setObjectName(_fromUtf8("labelPeak_4"))
        self.verticalLayout.addWidget(self.labelPeak_4)
        self.labelPeak_5 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_5.setObjectName(_fromUtf8("labelPeak_5"))
        self.verticalLayout.addWidget(self.labelPeak_5)
        self.labelPeak_6 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_6.setObjectName(_fromUtf8("labelPeak_6"))
        self.verticalLayout.addWidget(self.labelPeak_6)
        self.labelPeak_7 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_7.setObjectName(_fromUtf8("labelPeak_7"))
        self.verticalLayout.addWidget(self.labelPeak_7)
        self.labelPeak_8 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_8.setObjectName(_fromUtf8("labelPeak_8"))
        self.verticalLayout.addWidget(self.labelPeak_8)
        self.labelPeak_9 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_9.setObjectName(_fromUtf8("labelPeak_9"))
        self.verticalLayout.addWidget(self.labelPeak_9)
        self.labelPeak_10 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_10.setObjectName(_fromUtf8("labelPeak_10"))
        self.verticalLayout.addWidget(self.labelPeak_10)
        self.labelPeak_11 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_11.setObjectName(_fromUtf8("labelPeak_11"))
        self.verticalLayout.addWidget(self.labelPeak_11)
        self.labelPeak_12 = QtGui.QLabel(self.layoutWidget)
        self.labelPeak_12.setObjectName(_fromUtf8("labelPeak_12"))
        self.verticalLayout.addWidget(self.labelPeak_12)
        self.horizontalLayout.addLayout(self.verticalLayout)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.verticalLayout_2 = QtGui.QVBoxLayout()
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.editMass_1 = EpicsTextEdit(self.layoutWidget)
        self.editMass_1.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_1.setStyleSheet(_fromUtf8(""))
        self.editMass_1.setObjectName(_fromUtf8("editMass_1"))
        self.verticalLayout_2.addWidget(self.editMass_1)
        self.editMass_2 = EpicsTextEdit(self.layoutWidget)
        self.editMass_2.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_2.setStyleSheet(_fromUtf8(""))
        self.editMass_2.setObjectName(_fromUtf8("editMass_2"))
        self.verticalLayout_2.addWidget(self.editMass_2)
        self.editMass_3 = EpicsTextEdit(self.layoutWidget)
        self.editMass_3.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_3.setStyleSheet(_fromUtf8(""))
        self.editMass_3.setObjectName(_fromUtf8("editMass_3"))
        self.verticalLayout_2.addWidget(self.editMass_3)
        self.editMass_4 = EpicsTextEdit(self.layoutWidget)
        self.editMass_4.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_4.setStyleSheet(_fromUtf8(""))
        self.editMass_4.setObjectName(_fromUtf8("editMass_4"))
        self.verticalLayout_2.addWidget(self.editMass_4)
        self.editMass_5 = EpicsTextEdit(self.layoutWidget)
        self.editMass_5.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_5.setStyleSheet(_fromUtf8(""))
        self.editMass_5.setObjectName(_fromUtf8("editMass_5"))
        self.verticalLayout_2.addWidget(self.editMass_5)
        self.editMass_6 = EpicsTextEdit(self.layoutWidget)
        self.editMass_6.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_6.setStyleSheet(_fromUtf8(""))
        self.editMass_6.setObjectName(_fromUtf8("editMass_6"))
        self.verticalLayout_2.addWidget(self.editMass_6)
        self.editMass_7 = EpicsTextEdit(self.layoutWidget)
        self.editMass_7.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_7.setStyleSheet(_fromUtf8(""))
        self.editMass_7.setObjectName(_fromUtf8("editMass_7"))
        self.verticalLayout_2.addWidget(self.editMass_7)
        self.editMass_8 = EpicsTextEdit(self.layoutWidget)
        self.editMass_8.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_8.setStyleSheet(_fromUtf8(""))
        self.editMass_8.setObjectName(_fromUtf8("editMass_8"))
        self.verticalLayout_2.addWidget(self.editMass_8)
        self.editMass_9 = EpicsTextEdit(self.layoutWidget)
        self.editMass_9.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_9.setStyleSheet(_fromUtf8(""))
        self.editMass_9.setObjectName(_fromUtf8("editMass_9"))
        self.verticalLayout_2.addWidget(self.editMass_9)
        self.editMass_10 = EpicsTextEdit(self.layoutWidget)
        self.editMass_10.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_10.setStyleSheet(_fromUtf8(""))
        self.editMass_10.setObjectName(_fromUtf8("editMass_10"))
        self.verticalLayout_2.addWidget(self.editMass_10)
        self.editMass_11 = EpicsTextEdit(self.layoutWidget)
        self.editMass_11.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_11.setStyleSheet(_fromUtf8(""))
        self.editMass_11.setObjectName(_fromUtf8("editMass_11"))
        self.verticalLayout_2.addWidget(self.editMass_11)
        self.editMass_12 = EpicsTextEdit(self.layoutWidget)
        self.editMass_12.setMaximumSize(QtCore.QSize(50, 25))
        self.editMass_12.setStyleSheet(_fromUtf8(""))
        self.editMass_12.setObjectName(_fromUtf8("editMass_12"))
        self.verticalLayout_2.addWidget(self.editMass_12)
        self.horizontalLayout.addLayout(self.verticalLayout_2)
        self.btnApply = QtGui.QPushButton(Dialog)
        self.btnApply.setGeometry(QtCore.QRect(10, 480, 51, 27))
        self.btnApply.setObjectName(_fromUtf8("btnApply"))
        self.btnOK = QtGui.QPushButton(Dialog)
        self.btnOK.setGeometry(QtCore.QRect(70, 480, 51, 27))
        self.btnOK.setObjectName(_fromUtf8("btnOK"))
        self.btnCancel = QtGui.QPushButton(Dialog)
        self.btnCancel.setGeometry(QtCore.QRect(130, 480, 51, 27))
        self.btnCancel.setObjectName(_fromUtf8("btnCancel"))

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QtGui.QApplication.translate("Dialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Peak</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Mass</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.EPICSAppTitleWidget.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.EPICSAppTitleWidget.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EPICSAppTitleWidget widget provides EPICS domain aware title", None, QtGui.QApplication.UnicodeUTF8))
        self.EPICSAppTitleWidget.setText(QtGui.QApplication.translate("Dialog", "ddnnx-VA-RGA-0x", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Dialog", "Peak Jump Mass Selection", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak.setText(QtGui.QApplication.translate("Dialog", "1", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_2.setText(QtGui.QApplication.translate("Dialog", "2", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_3.setText(QtGui.QApplication.translate("Dialog", "3", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_4.setText(QtGui.QApplication.translate("Dialog", "4", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_5.setText(QtGui.QApplication.translate("Dialog", "5", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_6.setText(QtGui.QApplication.translate("Dialog", "6", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_7.setText(QtGui.QApplication.translate("Dialog", "7", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_8.setText(QtGui.QApplication.translate("Dialog", "8", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_9.setText(QtGui.QApplication.translate("Dialog", "9", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_10.setText(QtGui.QApplication.translate("Dialog", "10", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_11.setText(QtGui.QApplication.translate("Dialog", "11", None, QtGui.QApplication.UnicodeUTF8))
        self.labelPeak_12.setText(QtGui.QApplication.translate("Dialog", "12", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_1.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_1.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_1.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN1MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_1.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_2.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_2.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_2.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN2MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_2.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_3.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_3.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_3.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN3MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_3.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_4.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_4.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_4.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN4MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_4.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_5.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_5.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_5.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN5MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_5.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_6.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_6.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_6.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN6MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_6.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_7.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_7.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_7.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN7MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_7.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_8.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_8.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_8.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN8MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_8.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_9.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_9.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_9.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN9MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_9.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_10.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_10.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_10.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN10MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_10.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_11.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_11.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_11.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN11MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_11.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_12.setToolTip(QtGui.QApplication.translate("Dialog", "Click and drag here", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_12.setWhatsThis(QtGui.QApplication.translate("Dialog", "The EpicsTextEdit widget provides EPICS aware text edit", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_12.setPv(QtGui.QApplication.translate("Dialog", "FE02I-VA-RGA-01:PJ:BIN12MASS", None, QtGui.QApplication.UnicodeUTF8))
        self.editMass_12.setProperty("pvfield", QtGui.QApplication.translate("Dialog", "The specific record name", None, QtGui.QApplication.UnicodeUTF8))
        self.btnApply.setText(QtGui.QApplication.translate("Dialog", "Apply", None, QtGui.QApplication.UnicodeUTF8))
        self.btnOK.setText(QtGui.QApplication.translate("Dialog", "OK", None, QtGui.QApplication.UnicodeUTF8))
        self.btnCancel.setText(QtGui.QApplication.translate("Dialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))

from epics_app_title_widget import EPICSAppTitleWidget
from epicsedittextwidget import EpicsTextEdit