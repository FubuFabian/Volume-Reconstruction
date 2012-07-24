/********************************************************************************
** Form generated from reading UI file 'VolumeReconstructionWidget.ui'
**
** Created: Tue Jul 24 11:50:29 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLUMERECONSTRUCTIONWIDGET_H
#define UI_VOLUMERECONSTRUCTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VolumeReconstructionWidget
{
public:
    QPushButton *generate;
    QPushButton *save;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QRadioButton *voxelMethod;
    QRadioButton *pixelMethod;
    QSlider *resolution;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QWidget *VolumeReconstructionWidget)
    {
        if (VolumeReconstructionWidget->objectName().isEmpty())
            VolumeReconstructionWidget->setObjectName(QString::fromUtf8("VolumeReconstructionWidget"));
        VolumeReconstructionWidget->resize(352, 174);
        generate = new QPushButton(VolumeReconstructionWidget);
        generate->setObjectName(QString::fromUtf8("generate"));
        generate->setGeometry(QRect(110, 110, 131, 23));
        save = new QPushButton(VolumeReconstructionWidget);
        save->setObjectName(QString::fromUtf8("save"));
        save->setGeometry(QRect(130, 140, 101, 23));
        horizontalLayoutWidget = new QWidget(VolumeReconstructionWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(20, 10, 312, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        voxelMethod = new QRadioButton(horizontalLayoutWidget);
        voxelMethod->setObjectName(QString::fromUtf8("voxelMethod"));
        voxelMethod->setChecked(true);

        horizontalLayout->addWidget(voxelMethod);

        pixelMethod = new QRadioButton(horizontalLayoutWidget);
        pixelMethod->setObjectName(QString::fromUtf8("pixelMethod"));

        horizontalLayout->addWidget(pixelMethod);

        resolution = new QSlider(VolumeReconstructionWidget);
        resolution->setObjectName(QString::fromUtf8("resolution"));
        resolution->setGeometry(QRect(60, 80, 231, 22));
        resolution->setOrientation(Qt::Horizontal);
        label = new QLabel(VolumeReconstructionWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(120, 70, 121, 16));
        label_2 = new QLabel(VolumeReconstructionWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(280, 100, 16, 16));
        label_3 = new QLabel(VolumeReconstructionWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 100, 16, 16));

        retranslateUi(VolumeReconstructionWidget);
        QObject::connect(generate, SIGNAL(clicked()), VolumeReconstructionWidget, SLOT(generate()));
        QObject::connect(save, SIGNAL(clicked()), VolumeReconstructionWidget, SLOT(save()));
        QObject::connect(resolution, SIGNAL(valueChanged(int)), VolumeReconstructionWidget, SLOT(setResolution(int)));

        QMetaObject::connectSlotsByName(VolumeReconstructionWidget);
    } // setupUi

    void retranslateUi(QWidget *VolumeReconstructionWidget)
    {
        VolumeReconstructionWidget->setWindowTitle(QApplication::translate("VolumeReconstructionWidget", "VolumeReconstructionWidget", 0, QApplication::UnicodeUTF8));
        generate->setText(QApplication::translate("VolumeReconstructionWidget", "Generate Volume", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("VolumeReconstructionWidget", "Save Volume", 0, QApplication::UnicodeUTF8));
        voxelMethod->setText(QApplication::translate("VolumeReconstructionWidget", "Voxel Based Method", 0, QApplication::UnicodeUTF8));
        pixelMethod->setText(QApplication::translate("VolumeReconstructionWidget", "Pixel Based Method", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("VolumeReconstructionWidget", "Volume Resolution", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("VolumeReconstructionWidget", "-", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("VolumeReconstructionWidget", "+", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VolumeReconstructionWidget: public Ui_VolumeReconstructionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMERECONSTRUCTIONWIDGET_H
