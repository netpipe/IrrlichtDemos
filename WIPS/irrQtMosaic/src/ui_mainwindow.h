/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *basePicLabel;
    QLineEdit *imagePath;
    QPushButton *getImagePath;
    QFormLayout *formLayout_3;
    QLabel *label_3;
    QLineEdit *pmWidth;
    QLabel *label_7;
    QLineEdit *pmHeight;
    QLabel *label_9;
    QLineEdit *numOfSub;
    QLabel *label_10;
    QLineEdit *subRatio;
    QLabel *label_11;
    QSpinBox *precisionBox;
    QPushButton *step2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *subPicLabel;
    QLineEdit *subImgDir;
    QPushButton *getSubImgDir;
    QGraphicsView *graphicsView;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(581, 350);
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        font.setPointSize(12);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        MainWindow->setFont(font);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        basePicLabel = new QLabel(centralwidget);
        basePicLabel->setObjectName(QString::fromUtf8("basePicLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        font1.setPointSize(10);
        basePicLabel->setFont(font1);

        horizontalLayout->addWidget(basePicLabel);

        imagePath = new QLineEdit(centralwidget);
        imagePath->setObjectName(QString::fromUtf8("imagePath"));
        QFont font2;
        font2.setPointSize(10);
        imagePath->setFont(font2);

        horizontalLayout->addWidget(imagePath);

        getImagePath = new QPushButton(centralwidget);
        getImagePath->setObjectName(QString::fromUtf8("getImagePath"));
        QFont font3;
        font3.setPointSize(8);
        getImagePath->setFont(font3);

        horizontalLayout->addWidget(getImagePath);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 3);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font1);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_3);

        pmWidth = new QLineEdit(centralwidget);
        pmWidth->setObjectName(QString::fromUtf8("pmWidth"));
        pmWidth->setFont(font2);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, pmWidth);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font1);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_7);

        pmHeight = new QLineEdit(centralwidget);
        pmHeight->setObjectName(QString::fromUtf8("pmHeight"));
        pmHeight->setFont(font2);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, pmHeight);

        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font1);

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_9);

        numOfSub = new QLineEdit(centralwidget);
        numOfSub->setObjectName(QString::fromUtf8("numOfSub"));
        numOfSub->setFont(font2);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, numOfSub);

        label_10 = new QLabel(centralwidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font1);

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_10);

        subRatio = new QLineEdit(centralwidget);
        subRatio->setObjectName(QString::fromUtf8("subRatio"));
        subRatio->setFont(font2);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, subRatio);

        label_11 = new QLabel(centralwidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font1);

        formLayout_3->setWidget(4, QFormLayout::LabelRole, label_11);

        precisionBox = new QSpinBox(centralwidget);
        precisionBox->setObjectName(QString::fromUtf8("precisionBox"));

        formLayout_3->setWidget(4, QFormLayout::FieldRole, precisionBox);

        step2 = new QPushButton(centralwidget);
        step2->setObjectName(QString::fromUtf8("step2"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Tahoma"));
        step2->setFont(font4);

        formLayout_3->setWidget(5, QFormLayout::SpanningRole, step2);


        gridLayout->addLayout(formLayout_3, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        subPicLabel = new QLabel(centralwidget);
        subPicLabel->setObjectName(QString::fromUtf8("subPicLabel"));
        subPicLabel->setFont(font1);

        horizontalLayout_2->addWidget(subPicLabel);

        subImgDir = new QLineEdit(centralwidget);
        subImgDir->setObjectName(QString::fromUtf8("subImgDir"));
        subImgDir->setFont(font2);

        horizontalLayout_2->addWidget(subImgDir);

        getSubImgDir = new QPushButton(centralwidget);
        getSubImgDir->setObjectName(QString::fromUtf8("getSubImgDir"));
        getSubImgDir->setFont(font3);

        horizontalLayout_2->addWidget(getSubImgDir);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 3);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);

        gridLayout->addWidget(graphicsView, 2, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 1, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(false);
        progressBar->setValue(0);
        progressBar->setTextVisible(true);

        gridLayout->addWidget(progressBar, 3, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 581, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        basePicLabel->setText(QApplication::translate("MainWindow", "Base Picture:                        ", 0, QApplication::UnicodeUTF8));
        getImagePath->setText(QApplication::translate("MainWindow", "   Select Image  ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Photo Mosaic Width:              ", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Photo Mosaic Height: ", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "Number of Sub Images:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "Sub Image Aspect Ratio (X:Y)", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Precision", 0, QApplication::UnicodeUTF8));
        step2->setText(QApplication::translate("MainWindow", "Generate Photo Mosaic", 0, QApplication::UnicodeUTF8));
        subPicLabel->setText(QApplication::translate("MainWindow", "Sub Picture Directory             ", 0, QApplication::UnicodeUTF8));
        getSubImgDir->setText(QApplication::translate("MainWindow", "Select Directory", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
