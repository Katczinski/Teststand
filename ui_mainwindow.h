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
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QCodeEditor.hpp"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *centralLayout;
    QGridLayout *gridLayout_3;
    QLineEdit *serialnum;
    QCheckBox *flash;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QLineEdit *mac;
    QCheckBox *test;
    QLabel *label_4;
    QComboBox *modification;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_5;
    QComboBox *module;
    QPushButton *start;
    QLineEdit *cipher;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *codeEditorMenu;
    QCodeEditor *codeEditor;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(829, 561);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        centralLayout = new QHBoxLayout();
        centralLayout->setObjectName(QString::fromUtf8("centralLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        serialnum = new QLineEdit(centralwidget);
        serialnum->setObjectName(QString::fromUtf8("serialnum"));

        gridLayout_3->addWidget(serialnum, 7, 0, 1, 2);

        flash = new QCheckBox(centralwidget);
        flash->setObjectName(QString::fromUtf8("flash"));

        gridLayout_3->addWidget(flash, 12, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 10, 0, 1, 2);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 0, 0, 1, 2);

        mac = new QLineEdit(centralwidget);
        mac->setObjectName(QString::fromUtf8("mac"));

        gridLayout_3->addWidget(mac, 9, 0, 1, 2);

        test = new QCheckBox(centralwidget);
        test->setObjectName(QString::fromUtf8("test"));
        test->setChecked(true);

        gridLayout_3->addWidget(test, 11, 0, 1, 2);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 6, 0, 1, 2);

        modification = new QComboBox(centralwidget);
        modification->addItem(QString());
        modification->setObjectName(QString::fromUtf8("modification"));

        gridLayout_3->addWidget(modification, 3, 0, 1, 2);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 2, 0, 1, 2);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 4, 0, 1, 2);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 8, 0, 1, 2);

        module = new QComboBox(centralwidget);
        module->addItem(QString());
        module->setObjectName(QString::fromUtf8("module"));

        gridLayout_3->addWidget(module, 1, 0, 1, 2);

        start = new QPushButton(centralwidget);
        start->setObjectName(QString::fromUtf8("start"));
        start->setMinimumSize(QSize(0, 20));

        gridLayout_3->addWidget(start, 13, 0, 1, 2);

        cipher = new QLineEdit(centralwidget);
        cipher->setObjectName(QString::fromUtf8("cipher"));
        cipher->setMaxLength(16);

        gridLayout_3->addWidget(cipher, 5, 0, 1, 2);


        centralLayout->addLayout(gridLayout_3);


        horizontalLayout->addLayout(centralLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        codeEditorMenu = new QHBoxLayout();
        codeEditorMenu->setObjectName(QString::fromUtf8("codeEditorMenu"));

        verticalLayout->addLayout(codeEditorMenu);

        codeEditor = new QCodeEditor(centralwidget);
        codeEditor->setObjectName(QString::fromUtf8("codeEditor"));

        verticalLayout->addWidget(codeEditor);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 829, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        serialnum->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        flash->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\321\210\320\270\320\262\320\272\320\260", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\264\321\203\320\273\321\214 (argv[1])", nullptr));
        mac->setText(QCoreApplication::translate("MainWindow", "00-FF-38-34-96-A5", nullptr));
        test->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\320\262\320\265\321\200\320\272\320\260", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\241\320\265\321\200\320\270\320\271\320\275\321\213\320\271 \320\275\320\276\320\274\320\265\321\200", nullptr));
        modification->setItemText(0, QCoreApplication::translate("MainWindow", "4", nullptr));

        label_3->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\264\320\270\321\204\320\270\320\272\320\260\321\206\320\270\321\217 (argv[2])", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\250\320\270\321\204\321\200", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "MAC-\320\260\320\264\321\200\320\265\321\201", nullptr));
        module->setItemText(0, QCoreApplication::translate("MainWindow", "4 - kB-EG", nullptr));

        start->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\321\203\321\201\320\272", nullptr));
        cipher->setText(QCoreApplication::translate("MainWindow", "4111150302111110", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
