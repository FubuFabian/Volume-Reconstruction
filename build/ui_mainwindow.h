/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jul 24 11:50:29 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAdd_Images_Folder;
    QAction *actionQuit;
    QAction *actionUS_Probe_Calibration;
    QAction *actionOpen_Volume_Data;
    QAction *actionVolume_Reconstruction;
    QAction *actionOpen_Volume;
    QAction *actionCrop_Images;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTextEdit *textEdit;
    QWidget *imageWidget;
    QSlider *imageSlider;
    QSlider *opacitySlider;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuProcess;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(661, 660);
        MainWindow->setMinimumSize(QSize(340, 300));
        MainWindow->setIconSize(QSize(25, 25));
        actionAdd_Images_Folder = new QAction(MainWindow);
        actionAdd_Images_Folder->setObjectName(QString::fromUtf8("actionAdd_Images_Folder"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionUS_Probe_Calibration = new QAction(MainWindow);
        actionUS_Probe_Calibration->setObjectName(QString::fromUtf8("actionUS_Probe_Calibration"));
        actionOpen_Volume_Data = new QAction(MainWindow);
        actionOpen_Volume_Data->setObjectName(QString::fromUtf8("actionOpen_Volume_Data"));
        actionVolume_Reconstruction = new QAction(MainWindow);
        actionVolume_Reconstruction->setObjectName(QString::fromUtf8("actionVolume_Reconstruction"));
        actionOpen_Volume = new QAction(MainWindow);
        actionOpen_Volume->setObjectName(QString::fromUtf8("actionOpen_Volume"));
        actionCrop_Images = new QAction(MainWindow);
        actionCrop_Images->setObjectName(QString::fromUtf8("actionCrop_Images"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(8);
        sizePolicy.setVerticalStretch(6);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(320, 240));
        centralWidget->setSizeIncrement(QSize(8, 6));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(9);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(12);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 3, 0, 1, 1);

        imageWidget = new QWidget(centralWidget);
        imageWidget->setObjectName(QString::fromUtf8("imageWidget"));
        imageWidget->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(13);
        sizePolicy2.setVerticalStretch(10);
        sizePolicy2.setHeightForWidth(imageWidget->sizePolicy().hasHeightForWidth());
        imageWidget->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(imageWidget, 0, 0, 1, 1);

        imageSlider = new QSlider(centralWidget);
        imageSlider->setObjectName(QString::fromUtf8("imageSlider"));
        imageSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(imageSlider, 2, 0, 1, 1);

        opacitySlider = new QSlider(centralWidget);
        opacitySlider->setObjectName(QString::fromUtf8("opacitySlider"));
        opacitySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(opacitySlider, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 661, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuProcess = new QMenu(menuBar);
        menuProcess->setObjectName(QString::fromUtf8("menuProcess"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuProcess->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionAdd_Images_Folder);
        menuFile->addAction(actionOpen_Volume_Data);
        menuFile->addAction(actionOpen_Volume);
        menuFile->addAction(actionQuit);
        menuProcess->addAction(actionCrop_Images);
        menuProcess->addAction(actionUS_Probe_Calibration);
        menuProcess->addAction(actionVolume_Reconstruction);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionAdd_Images_Folder, SIGNAL(triggered()), MainWindow, SLOT(addImages()));
        QObject::connect(actionUS_Probe_Calibration, SIGNAL(triggered()), MainWindow, SLOT(probeCalibration()));
        QObject::connect(imageSlider, SIGNAL(valueChanged(int)), MainWindow, SLOT(displaySelectedImage(int)));
        QObject::connect(actionOpen_Volume_Data, SIGNAL(triggered()), MainWindow, SLOT(openVolumeData()));
        QObject::connect(actionVolume_Reconstruction, SIGNAL(triggered()), MainWindow, SLOT(volumeReconstruction()));
        QObject::connect(actionOpen_Volume, SIGNAL(triggered()), MainWindow, SLOT(openVolume()));
        QObject::connect(opacitySlider, SIGNAL(valueChanged(int)), MainWindow, SLOT(setSelectedOpacity(int)));
        QObject::connect(actionCrop_Images, SIGNAL(triggered()), MainWindow, SLOT(cropImages()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionAdd_Images_Folder->setText(QApplication::translate("MainWindow", "Open Image(s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAdd_Images_Folder->setToolTip(QApplication::translate("MainWindow", "Open Image or images", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionUS_Probe_Calibration->setText(QApplication::translate("MainWindow", "US Probe Calibration", 0, QApplication::UnicodeUTF8));
        actionOpen_Volume_Data->setText(QApplication::translate("MainWindow", "Open Volume Data", 0, QApplication::UnicodeUTF8));
        actionVolume_Reconstruction->setText(QApplication::translate("MainWindow", "Volume Reconstruction", 0, QApplication::UnicodeUTF8));
        actionOpen_Volume->setText(QApplication::translate("MainWindow", "Open Volume", 0, QApplication::UnicodeUTF8));
        actionCrop_Images->setText(QApplication::translate("MainWindow", "Crop Images", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuProcess->setTitle(QApplication::translate("MainWindow", "Process", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
