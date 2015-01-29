#include "FrmMain.h"
#ifdef MOBILE
    #include "ui_FrmMainAndroid.h"
#else
    #include "ui_FrmMain.h"
#endif
#include "../../Global/Global.h"
#include "MainWindow.h"
#include <QSettings>

CFrmMain::CFrmMain(QWidget *parent) :
    QFrame(parent),
    m_UserList(parent),
    m_GroupChatList(parent),
    ui(new Ui::CFrmMain)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
#ifndef MOBILE
    ui->lbAvatar->setScaledContents(true);
    ui->lbName->clear();
#endif

    bool check = connect(GET_CLIENT.data(), SIGNAL(sigUpdateLocaleUserInfo()),
                    SLOT(slotUpdateLocaleUserInfo()));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRefresh()),
                    SLOT(slotUpdateLocaleUserInfo()));
    Q_ASSERT(check);
#ifndef MOBILE
    check = connect(ui->lbAvatar, SIGNAL(clicked()),
                    GET_MAINWINDOW, SLOT(slotEditInformation()));
    Q_ASSERT(check);
    check = connect(ui->lbName, SIGNAL(clicked()),
                    GET_MAINWINDOW, SLOT(slotEditInformation()));
    Q_ASSERT(check);
#endif
    ui->tabWidget->addTab(&m_UserList, QIcon(":/icon/User"), tr("Rosters"));
    ui->tabWidget->addTab(&m_GroupChatList, QIcon(":/icon/Users"), tr("Group Chat"));
    ui->tabWidget->addTab(&m_MessageList, QIcon(":/icon/Message"), tr("Recent messages"));

    if(USER_INFO_LOCALE.isNull() || USER_INFO_LOCALE->GetInfo().isNull())
    {
        LOG_MODEL_ERROR("CFrmMain", "locale user is null");
        return;
    }
    QSettings conf(CGlobal::Instance()->GetUserConfigureFile(USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
    int nIndex = conf.value("Widgets/Main", 0).toInt();
    ui->tabWidget->setCurrentIndex(nIndex);
}

CFrmMain::~CFrmMain()
{
    if(!USER_INFO_LOCALE.isNull())
    {
        QSettings conf(CGlobal::Instance()->GetUserConfigureFile(USER_INFO_LOCALE->GetInfo()->GetId()), QSettings::IniFormat);
        conf.setValue("Widgets/Main", ui->tabWidget->currentIndex());
    }

    delete ui;
}

void CFrmMain::resizeEvent(QResizeEvent *e)
{
    /*LOG_MODEL_DEBUG("CWdgTableMain", "CWdgTableMain::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());*/

    //ui->tabWidget->resize(geometry().size().width(), geometry().size().height() - ui->lbAvatar->frameGeometry().height());
    //ui->tabWidget->currentWidget()->resize(ui->tabWidget->geometry().size());
}

void CFrmMain::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_UserList), tr("Rosters"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_MessageList), tr("Recent messages"));
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_GroupChatList), tr("Group Chat"));
        break;
    }
}

void CFrmMain::showEvent(QShowEvent *)
{
    slotUpdateLocaleUserInfo();
}

void CFrmMain::slotUpdateLocaleUserInfo()
{
    QSharedPointer<CUser> user = USER_INFO_LOCALE;
    if(user.isNull())
    {
        LOG_MODEL_ERROR("CFrmMain", "User is null");
        return;
    }
    QSharedPointer<CUserInfo> info = user->GetInfo();
    if(info.isNull())
    {
        LOG_MODEL_ERROR("CFrmMain", "User is null");
        return;
    }
#ifndef MOBILE
    ui->lbName->setText(info->GetShowName());
    QPixmap pmp;
    MainWindow::ComposeAvatarStatus(info, pmp);
    ui->lbAvatar->clear();
    ui->lbAvatar->setPixmap(pmp);
#endif
}
