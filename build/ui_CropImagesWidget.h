/********************************************************************************
** Form generated from reading UI file 'CropImagesWidget.ui'
**
** Created: Tue Jul 24 11:50:29 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CROPIMAGESWIDGET_H
#define UI_CROPIMAGESWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CropImagesWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QRadioButton *radioButton_6;
    QRadioButton *radioButton_8;
    QPushButton *crop;
    QPushButton *save;

    void setupUi(QWidget *CropImagesWidget)
    {
        if (CropImagesWidget->objectName().isEmpty())
            CropImagesWidget->setObjectName(QString::fromUtf8("CropImagesWidget"));
        CropImagesWidget->resize(141, 176);
        verticalLayoutWidget = new QWidget(CropImagesWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(30, 10, 71, 88));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        radioButton_4 = new QRadioButton(verticalLayoutWidget);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        verticalLayout->addWidget(radioButton_4);

        radioButton_5 = new QRadioButton(verticalLayoutWidget);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));

        verticalLayout->addWidget(radioButton_5);

        radioButton_6 = new QRadioButton(verticalLayoutWidget);
        radioButton_6->setObjectName(QString::fromUtf8("radioButton_6"));

        verticalLayout->addWidget(radioButton_6);

        radioButton_8 = new QRadioButton(verticalLayoutWidget);
        radioButton_8->setObjectName(QString::fromUtf8("radioButton_8"));

        verticalLayout->addWidget(radioButton_8);

        crop = new QPushButton(CropImagesWidget);
        crop->setObjectName(QString::fromUtf8("crop"));
        crop->setGeometry(QRect(30, 110, 75, 23));
        save = new QPushButton(CropImagesWidget);
        save->setObjectName(QString::fromUtf8("save"));
        save->setGeometry(QRect(10, 140, 121, 23));
        verticalLayoutWidget->raise();
        crop->raise();
        save->raise();
        radioButton_6->raise();

        retranslateUi(CropImagesWidget);
        QObject::connect(crop, SIGNAL(clicked()), CropImagesWidget, SLOT(crop()));
        QObject::connect(save, SIGNAL(clicked()), CropImagesWidget, SLOT(save()));

        QMetaObject::connectSlotsByName(CropImagesWidget);
    } // setupUi

    void retranslateUi(QWidget *CropImagesWidget)
    {
        CropImagesWidget->setWindowTitle(QApplication::translate("CropImagesWidget", "CropImagesWidget", 0, QApplication::UnicodeUTF8));
        radioButton_4->setText(QApplication::translate("CropImagesWidget", "Range 4", 0, QApplication::UnicodeUTF8));
        radioButton_5->setText(QApplication::translate("CropImagesWidget", "Range 5", 0, QApplication::UnicodeUTF8));
        radioButton_6->setText(QApplication::translate("CropImagesWidget", "Range 6", 0, QApplication::UnicodeUTF8));
        radioButton_8->setText(QApplication::translate("CropImagesWidget", "Range 8", 0, QApplication::UnicodeUTF8));
        crop->setText(QApplication::translate("CropImagesWidget", "Crop", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("CropImagesWidget", "Save Cropped Images", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CropImagesWidget: public Ui_CropImagesWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CROPIMAGESWIDGET_H
