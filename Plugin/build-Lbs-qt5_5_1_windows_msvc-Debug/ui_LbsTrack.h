/********************************************************************************
** Form generated from reading UI file 'LbsTrack.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LBSTRACK_H
#define UI_LBSTRACK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include "QtQuickWidgets/QQuickWidget"

QT_BEGIN_NAMESPACE

class Ui_CLbsTrack
{
public:
    QGridLayout *gridLayout;
    QTextBrowser *txtLogger;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbStart;
    QSpacerItem *horizontalSpacer_2;
    QQuickWidget *quickWidget;

    void setupUi(QFrame *CLbsTrack)
    {
        if (CLbsTrack->objectName().isEmpty())
            CLbsTrack->setObjectName(QStringLiteral("CLbsTrack"));
        CLbsTrack->resize(600, 400);
        gridLayout = new QGridLayout(CLbsTrack);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        txtLogger = new QTextBrowser(CLbsTrack);
        txtLogger->setObjectName(QStringLiteral("txtLogger"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(txtLogger->sizePolicy().hasHeightForWidth());
        txtLogger->setSizePolicy(sizePolicy);

        gridLayout->addWidget(txtLogger, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pbStart = new QPushButton(CLbsTrack);
        pbStart->setObjectName(QStringLiteral("pbStart"));

        horizontalLayout->addWidget(pbStart);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        quickWidget = new QQuickWidget(CLbsTrack);
        quickWidget->setObjectName(QStringLiteral("quickWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(4);
        sizePolicy1.setHeightForWidth(quickWidget->sizePolicy().hasHeightForWidth());
        quickWidget->setSizePolicy(sizePolicy1);
        quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

        gridLayout->addWidget(quickWidget, 0, 0, 1, 1);


        retranslateUi(CLbsTrack);

        QMetaObject::connectSlotsByName(CLbsTrack);
    } // setupUi

    void retranslateUi(QFrame *CLbsTrack)
    {
        CLbsTrack->setWindowTitle(QApplication::translate("CLbsTrack", "CLbsMain", 0));
        pbStart->setText(QApplication::translate("CLbsTrack", "Start", 0));
    } // retranslateUi

};

namespace Ui {
    class CLbsTrack: public Ui_CLbsTrack {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LBSTRACK_H
