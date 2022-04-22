/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_port;
    QLabel *label_ip;
    QLineEdit *lineEdit_ip;
    QLineEdit *lineEdit_port;
    QLabel *label_key;
    QLineEdit *lineEdit_key;
    QPushButton *pushButton_addClient;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(757, 418);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_port = new QLabel(groupBox);
        label_port->setObjectName(QStringLiteral("label_port"));

        gridLayout->addWidget(label_port, 1, 0, 1, 1);

        label_ip = new QLabel(groupBox);
        label_ip->setObjectName(QStringLiteral("label_ip"));

        gridLayout->addWidget(label_ip, 0, 0, 1, 1);

        lineEdit_ip = new QLineEdit(groupBox);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));

        gridLayout->addWidget(lineEdit_ip, 0, 1, 1, 1);

        lineEdit_port = new QLineEdit(groupBox);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));

        gridLayout->addWidget(lineEdit_port, 1, 1, 1, 1);

        label_key = new QLabel(groupBox);
        label_key->setObjectName(QStringLiteral("label_key"));

        gridLayout->addWidget(label_key, 2, 0, 1, 1);

        lineEdit_key = new QLineEdit(groupBox);
        lineEdit_key->setObjectName(QStringLiteral("lineEdit_key"));

        gridLayout->addWidget(lineEdit_key, 2, 1, 1, 1);

        pushButton_addClient = new QPushButton(groupBox);
        pushButton_addClient->setObjectName(QStringLiteral("pushButton_addClient"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_addClient->sizePolicy().hasHeightForWidth());
        pushButton_addClient->setSizePolicy(sizePolicy);

        gridLayout->addWidget(pushButton_addClient, 0, 3, 3, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_3->addWidget(pushButton_2, 1, 0, 1, 1);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_3->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout_3->addWidget(pushButton_3, 2, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 1, 0, 1, 1);

        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setMinimumSize(QSize(0, 0));
        textEdit->setMaximumSize(QSize(350, 16777215));

        gridLayout_2->addWidget(textEdit, 0, 1, 2, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 757, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "NPS", Q_NULLPTR));
        label_port->setText(QApplication::translate("MainWindow", "port", Q_NULLPTR));
        label_ip->setText(QApplication::translate("MainWindow", "ip", Q_NULLPTR));
        lineEdit_ip->setText(QApplication::translate("MainWindow", "saas.znfx.net", Q_NULLPTR));
        lineEdit_port->setText(QApplication::translate("MainWindow", "8006", Q_NULLPTR));
        label_key->setText(QApplication::translate("MainWindow", "key", Q_NULLPTR));
        lineEdit_key->setText(QApplication::translate("MainWindow", "45cb6383870d9e87a952c53daf22d3dc", Q_NULLPTR));
        pushButton_addClient->setText(QApplication::translate("MainWindow", "\346\263\250\345\206\214", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\346\234\215\345\212\241\345\231\250", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\350\277\234\347\250\213\346\241\214\351\235\242", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", " \350\207\252\345\256\232\344\271\211", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
