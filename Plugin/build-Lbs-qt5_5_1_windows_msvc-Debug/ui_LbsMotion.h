/********************************************************************************
** Form generated from reading UI file 'LbsMotion.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LBSMOTION_H
#define UI_LBSMOTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "QtQuickWidgets/QQuickWidget"

QT_BEGIN_NAMESPACE

class Ui_CLbsMotion
{
public:
    QGridLayout *gridLayout_2;
    QWidget *wdgPrompt;
    QGridLayout *gridLayout;
    QLabel *lbDistance;
    QHBoxLayout *horizontalLayout;
    QLabel *lbDuration;
    QLabel *lbSpeed;
    QLabel *lbDistance_1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *lbAccuracy;
    QLabel *lbRealTimeSpeed;
    QHBoxLayout *lyStart;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pbStart;
    QPushButton *pbPause;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *lyShow;
    QPushButton *pbCamera;
    QSpacerItem *horizontalSpacer;
    QPushButton *pbShow;
    QSpacerItem *horizontalSpacer_2;
    QQuickWidget *qwMap;

    void setupUi(QFrame *CLbsMotion)
    {
        if (CLbsMotion->objectName().isEmpty())
            CLbsMotion->setObjectName(QStringLiteral("CLbsMotion"));
        CLbsMotion->resize(400, 300);
        QIcon icon;
        icon.addFile(QStringLiteral(":/png/motioin"), QSize(), QIcon::Normal, QIcon::Off);
        CLbsMotion->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(CLbsMotion);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        wdgPrompt = new QWidget(CLbsMotion);
        wdgPrompt->setObjectName(QStringLiteral("wdgPrompt"));
        gridLayout = new QGridLayout(wdgPrompt);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lbDistance = new QLabel(wdgPrompt);
        lbDistance->setObjectName(QStringLiteral("lbDistance"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbDistance->sizePolicy().hasHeightForWidth());
        lbDistance->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        lbDistance->setFont(font);
        lbDistance->setText(QStringLiteral("0.00"));
        lbDistance->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lbDistance, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbDuration = new QLabel(wdgPrompt);
        lbDuration->setObjectName(QStringLiteral("lbDuration"));
        lbDuration->setText(QStringLiteral("00:00:00"));
        lbDuration->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lbDuration);

        lbSpeed = new QLabel(wdgPrompt);
        lbSpeed->setObjectName(QStringLiteral("lbSpeed"));
        lbSpeed->setText(QStringLiteral("0.00"));
        lbSpeed->setScaledContents(false);
        lbSpeed->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lbSpeed);

        lbDistance_1 = new QLabel(wdgPrompt);
        lbDistance_1->setObjectName(QStringLiteral("lbDistance_1"));
        lbDistance_1->setText(QStringLiteral("0.00"));
        lbDistance_1->setScaledContents(false);
        lbDistance_1->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lbDistance_1);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        lbAccuracy = new QLabel(wdgPrompt);
        lbAccuracy->setObjectName(QStringLiteral("lbAccuracy"));
        lbAccuracy->setText(QStringLiteral("0.00"));
        lbAccuracy->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(lbAccuracy);

        lbRealTimeSpeed = new QLabel(wdgPrompt);
        lbRealTimeSpeed->setObjectName(QStringLiteral("lbRealTimeSpeed"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbRealTimeSpeed->sizePolicy().hasHeightForWidth());
        lbRealTimeSpeed->setSizePolicy(sizePolicy1);
        lbRealTimeSpeed->setText(QStringLiteral("0.00"));
        lbRealTimeSpeed->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(lbRealTimeSpeed);


        gridLayout->addLayout(horizontalLayout_4, 2, 0, 1, 1);

        lyStart = new QHBoxLayout();
        lyStart->setObjectName(QStringLiteral("lyStart"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        lyStart->addItem(horizontalSpacer_3);

        pbStart = new QPushButton(wdgPrompt);
        pbStart->setObjectName(QStringLiteral("pbStart"));

        lyStart->addWidget(pbStart);

        pbPause = new QPushButton(wdgPrompt);
        pbPause->setObjectName(QStringLiteral("pbPause"));

        lyStart->addWidget(pbPause);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        lyStart->addItem(horizontalSpacer_4);


        gridLayout->addLayout(lyStart, 3, 0, 1, 1);

        lyShow = new QHBoxLayout();
        lyShow->setObjectName(QStringLiteral("lyShow"));
        pbCamera = new QPushButton(wdgPrompt);
        pbCamera->setObjectName(QStringLiteral("pbCamera"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/Video"), QSize(), QIcon::Normal, QIcon::Off);
        pbCamera->setIcon(icon1);

        lyShow->addWidget(pbCamera);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        lyShow->addItem(horizontalSpacer);

        pbShow = new QPushButton(wdgPrompt);
        pbShow->setObjectName(QStringLiteral("pbShow"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/png/up"), QSize(), QIcon::Normal, QIcon::Off);
        pbShow->setIcon(icon2);
        pbShow->setCheckable(false);

        lyShow->addWidget(pbShow);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        lyShow->addItem(horizontalSpacer_2);


        gridLayout->addLayout(lyShow, 4, 0, 1, 1);


        gridLayout_2->addWidget(wdgPrompt, 0, 0, 1, 1);

        qwMap = new QQuickWidget(CLbsMotion);
        qwMap->setObjectName(QStringLiteral("qwMap"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(qwMap->sizePolicy().hasHeightForWidth());
        qwMap->setSizePolicy(sizePolicy2);
        qwMap->setResizeMode(QQuickWidget::SizeRootObjectToView);

        gridLayout_2->addWidget(qwMap, 1, 0, 1, 1);


        retranslateUi(CLbsMotion);

        QMetaObject::connectSlotsByName(CLbsMotion);
    } // setupUi

    void retranslateUi(QFrame *CLbsMotion)
    {
        CLbsMotion->setWindowTitle(QApplication::translate("CLbsMotion", "Motion", 0));
        pbStart->setText(QApplication::translate("CLbsMotion", "Start", 0));
        pbPause->setText(QApplication::translate("CLbsMotion", "Pause", 0));
        pbCamera->setText(QString());
        pbShow->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CLbsMotion: public Ui_CLbsMotion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LBSMOTION_H
