/********************************************************************************
** Form generated from reading UI file 'FrmLbsMain.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMLBSMAIN_H
#define UI_FRMLBSMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CFrmLbsMain
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;

    void setupUi(QFrame *CFrmLbsMain)
    {
        if (CFrmLbsMain->objectName().isEmpty())
            CFrmLbsMain->setObjectName(QStringLiteral("CFrmLbsMain"));
        CFrmLbsMain->resize(700, 500);
        QIcon icon;
        icon.addFile(QStringLiteral(":/png/motioin"), QSize(), QIcon::Normal, QIcon::Off);
        CFrmLbsMain->setWindowIcon(icon);
        gridLayout = new QGridLayout(CFrmLbsMain);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, -1, 4);
        tabWidget = new QTabWidget(CFrmLbsMain);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::South);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(CFrmLbsMain);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CFrmLbsMain);
    } // setupUi

    void retranslateUi(QFrame *CFrmLbsMain)
    {
        CFrmLbsMain->setWindowTitle(QApplication::translate("CFrmLbsMain", "Motion", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CFrmLbsMain", "Tab 1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CFrmLbsMain", "Tab 2", 0));
    } // retranslateUi

};

namespace Ui {
    class CFrmLbsMain: public Ui_CFrmLbsMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMLBSMAIN_H
