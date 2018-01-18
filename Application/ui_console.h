/********************************************************************************
** Form generated from reading UI file 'console.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Console
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QTextBrowser *textBrowser;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *Console)
    {
        if (Console->objectName().isEmpty())
            Console->setObjectName(QStringLiteral("Console"));
        Console->resize(860, 550);
        gridLayout = new QGridLayout(Console);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listWidget = new QListWidget(Console);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        gridLayout->addWidget(listWidget, 0, 1, 1, 1);

        textBrowser = new QTextBrowser(Console);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout->addWidget(textBrowser, 1, 1, 1, 1);

        plainTextEdit = new QPlainTextEdit(Console);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 0, 0, 2, 1);


        retranslateUi(Console);

        QMetaObject::connectSlotsByName(Console);
    } // setupUi

    void retranslateUi(QWidget *Console)
    {
        Console->setWindowTitle(QApplication::translate("Console", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Console: public Ui_Console {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSOLE_H
