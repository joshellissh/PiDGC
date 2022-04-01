/********************************************************************************
** Form generated from reading UI file 'hwdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HWDIALOG_H
#define UI_HWDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_HWDialog
{
public:
    QPlainTextEdit *plainTextEdit;
    QPushButton *closeButton;
    QPushButton *saveButton;
    QGroupBox *groupBox;
    QLabel *label_4;
    QLineEdit *ppmCurrent;
    QSpinBox *ppmSpinBox;
    QLabel *label_5;
    QCheckBox *chimeBox;
    QCheckBox *blinkerBox;

    void setupUi(QDialog *HWDialog)
    {
        if (HWDialog->objectName().isEmpty())
            HWDialog->setObjectName(QString::fromUtf8("HWDialog"));
        HWDialog->resize(983, 446);
        plainTextEdit = new QPlainTextEdit(HWDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 541, 421));
        QFont font;
        font.setPointSize(11);
        plainTextEdit->setFont(font);
        closeButton = new QPushButton(HWDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(880, 390, 89, 41));
        saveButton = new QPushButton(HWDialog);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(780, 390, 89, 41));
        groupBox = new QGroupBox(HWDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(560, 6, 411, 91));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 46, 67, 17));
        ppmCurrent = new QLineEdit(groupBox);
        ppmCurrent->setObjectName(QString::fromUtf8("ppmCurrent"));
        ppmCurrent->setGeometry(QRect(80, 31, 113, 51));
        ppmCurrent->setReadOnly(true);
        ppmSpinBox = new QSpinBox(groupBox);
        ppmSpinBox->setObjectName(QString::fromUtf8("ppmSpinBox"));
        ppmSpinBox->setGeometry(QRect(260, 30, 141, 51));
        ppmSpinBox->setAutoFillBackground(true);
        ppmSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        ppmSpinBox->setAccelerated(true);
        ppmSpinBox->setMaximum(1000000);
        ppmSpinBox->setSingleStep(10);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(210, 47, 41, 17));
        chimeBox = new QCheckBox(HWDialog);
        chimeBox->setObjectName(QString::fromUtf8("chimeBox"));
        chimeBox->setGeometry(QRect(560, 110, 191, 23));
        blinkerBox = new QCheckBox(HWDialog);
        blinkerBox->setObjectName(QString::fromUtf8("blinkerBox"));
        blinkerBox->setGeometry(QRect(560, 140, 171, 23));

        retranslateUi(HWDialog);
        QObject::connect(saveButton, SIGNAL(pressed()), HWDialog, SLOT(accept()));
        QObject::connect(closeButton, SIGNAL(pressed()), HWDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(HWDialog);
    } // setupUi

    void retranslateUi(QDialog *HWDialog)
    {
        HWDialog->setWindowTitle(QCoreApplication::translate("HWDialog", "Hardware Interface", nullptr));
        closeButton->setText(QCoreApplication::translate("HWDialog", "Close", nullptr));
        saveButton->setText(QCoreApplication::translate("HWDialog", "Save", nullptr));
        groupBox->setTitle(QCoreApplication::translate("HWDialog", "Pulses Per Mile", nullptr));
        label_4->setText(QCoreApplication::translate("HWDialog", "Current:", nullptr));
        label_5->setText(QCoreApplication::translate("HWDialog", "New:", nullptr));
        chimeBox->setText(QCoreApplication::translate("HWDialog", "Enable Chime Sound", nullptr));
        blinkerBox->setText(QCoreApplication::translate("HWDialog", "Enable Blinker Sound", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HWDialog: public Ui_HWDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HWDIALOG_H
