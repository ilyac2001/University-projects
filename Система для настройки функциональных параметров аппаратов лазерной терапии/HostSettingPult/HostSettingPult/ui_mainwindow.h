/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionV1_0;
    QWidget *centralwidget;
    QLabel *lable_Name;
    QProgressBar *progressBar;
    QLabel *label_ConstVersion;
    QRadioButton *radioButton_MaxPower;
    QLabel *lable_Version;
    QPushButton *pushButton_APP;
    QRadioButton *radioButton_MinPower;
    QLabel *label_ValueProgressBar;
    QPushButton *pushButton_STOP;
    QLabel *label_ConstName;
    QPushButton *pushButton_DOWN;
    QLabel *lable_ValueMaxPower;
    QLabel *lable_ValueMinPower;
    QMenuBar *menubar;
    QMenu *menuSetting_Pult;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(636, 437);
        MainWindow->setCursor(QCursor(Qt::ArrowCursor));
        MainWindow->setAnimated(true);
        actionV1_0 = new QAction(MainWindow);
        actionV1_0->setObjectName(QString::fromUtf8("actionV1_0"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        lable_Name = new QLabel(centralwidget);
        lable_Name->setObjectName(QString::fromUtf8("lable_Name"));
        lable_Name->setGeometry(QRect(10, 30, 624, 46));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(24);
        font.setBold(false);
        font.setUnderline(false);
        font.setWeight(50);
        lable_Name->setFont(font);
        lable_Name->setCursor(QCursor(Qt::ArrowCursor));
        lable_Name->setAcceptDrops(false);
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setGeometry(QRect(70, 270, 490, 60));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(20);
        sizePolicy.setVerticalStretch(22);
        sizePolicy.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Times New Roman"));
        font1.setPointSize(24);
        font1.setBold(true);
        font1.setWeight(75);
        progressBar->setFont(font1);
        progressBar->setMaximum(255);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        label_ConstVersion = new QLabel(centralwidget);
        label_ConstVersion->setObjectName(QString::fromUtf8("label_ConstVersion"));
        label_ConstVersion->setGeometry(QRect(10, 80, 95, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Times New Roman"));
        font2.setPointSize(16);
        font2.setUnderline(false);
        label_ConstVersion->setFont(font2);
        radioButton_MaxPower = new QRadioButton(centralwidget);
        radioButton_MaxPower->setObjectName(QString::fromUtf8("radioButton_MaxPower"));
        radioButton_MaxPower->setGeometry(QRect(10, 220, 259, 38));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Times New Roman"));
        font3.setPointSize(18);
        radioButton_MaxPower->setFont(font3);
        radioButton_MaxPower->setCursor(QCursor(Qt::ArrowCursor));
        lable_Version = new QLabel(centralwidget);
        lable_Version->setObjectName(QString::fromUtf8("lable_Version"));
        lable_Version->setGeometry(QRect(10, 110, 624, 46));
        lable_Version->setFont(font);
        lable_Version->setCursor(QCursor(Qt::ArrowCursor));
        lable_Version->setAcceptDrops(false);
        pushButton_APP = new QPushButton(centralwidget);
        pushButton_APP->setObjectName(QString::fromUtf8("pushButton_APP"));
        pushButton_APP->setGeometry(QRect(560, 270, 60, 60));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Times New Roman"));
        font4.setPointSize(24);
        pushButton_APP->setFont(font4);
        pushButton_APP->setCursor(QCursor(Qt::PointingHandCursor));
        radioButton_MinPower = new QRadioButton(centralwidget);
        radioButton_MinPower->setObjectName(QString::fromUtf8("radioButton_MinPower"));
        radioButton_MinPower->setGeometry(QRect(10, 170, 255, 38));
        radioButton_MinPower->setFont(font3);
        radioButton_MinPower->setCursor(QCursor(Qt::ArrowCursor));
        label_ValueProgressBar = new QLabel(centralwidget);
        label_ValueProgressBar->setObjectName(QString::fromUtf8("label_ValueProgressBar"));
        label_ValueProgressBar->setGeometry(QRect(70, 270, 490, 60));
        label_ValueProgressBar->setFont(font1);
        label_ValueProgressBar->setTextFormat(Qt::AutoText);
        label_ValueProgressBar->setAlignment(Qt::AlignCenter);
        pushButton_STOP = new QPushButton(centralwidget);
        pushButton_STOP->setObjectName(QString::fromUtf8("pushButton_STOP"));
        pushButton_STOP->setGeometry(QRect(10, 340, 611, 55));
        pushButton_STOP->setFont(font4);
        pushButton_STOP->setCursor(QCursor(Qt::PointingHandCursor));
        label_ConstName = new QLabel(centralwidget);
        label_ConstName->setObjectName(QString::fromUtf8("label_ConstName"));
        label_ConstName->setGeometry(QRect(10, 0, 126, 31));
        label_ConstName->setFont(font2);
        pushButton_DOWN = new QPushButton(centralwidget);
        pushButton_DOWN->setObjectName(QString::fromUtf8("pushButton_DOWN"));
        pushButton_DOWN->setGeometry(QRect(10, 270, 60, 60));
        pushButton_DOWN->setFont(font4);
        pushButton_DOWN->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_DOWN->setIconSize(QSize(20, 20));
        lable_ValueMaxPower = new QLabel(centralwidget);
        lable_ValueMaxPower->setObjectName(QString::fromUtf8("lable_ValueMaxPower"));
        lable_ValueMaxPower->setGeometry(QRect(270, 220, 45, 34));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Times New Roman"));
        font5.setPointSize(18);
        font5.setBold(false);
        font5.setUnderline(true);
        font5.setWeight(50);
        lable_ValueMaxPower->setFont(font5);
        lable_ValueMaxPower->setAcceptDrops(false);
        lable_ValueMinPower = new QLabel(centralwidget);
        lable_ValueMinPower->setObjectName(QString::fromUtf8("lable_ValueMinPower"));
        lable_ValueMinPower->setGeometry(QRect(270, 170, 45, 34));
        lable_ValueMinPower->setFont(font5);
        lable_ValueMinPower->setAcceptDrops(false);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 636, 26));
        menuSetting_Pult = new QMenu(menubar);
        menuSetting_Pult->setObjectName(QString::fromUtf8("menuSetting_Pult"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuSetting_Pult->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionV1_0->setText(QCoreApplication::translate("MainWindow", "V1.0", nullptr));
        lable_Name->setText(QString());
        progressBar->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        label_ConstVersion->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\321\200\321\201\320\270\321\217 :", nullptr));
        radioButton_MaxPower->setText(QCoreApplication::translate("MainWindow", "MAX \320\274\320\276\321\211\320\275\320\276\321\201\321\202\321\214 =", nullptr));
        lable_Version->setText(QString());
        pushButton_APP->setText(QCoreApplication::translate("MainWindow", "\342\226\262", nullptr));
        radioButton_MinPower->setText(QCoreApplication::translate("MainWindow", "MIN \320\274\320\276\321\211\320\275\320\276\321\201\321\202\321\214 = ", nullptr));
        label_ValueProgressBar->setText(QString());
        pushButton_STOP->setText(QCoreApplication::translate("MainWindow", "\320\241\321\202\320\276\320\277", nullptr));
        label_ConstName->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 : ", nullptr));
        pushButton_DOWN->setText(QCoreApplication::translate("MainWindow", "\342\226\274", nullptr));
        lable_ValueMaxPower->setText(QString());
        lable_ValueMinPower->setText(QString());
        menuSetting_Pult->setTitle(QCoreApplication::translate("MainWindow", "V1.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
