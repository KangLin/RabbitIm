#include "FrmMain.h"
#include "ui_FrmMain.h"
#include "../../Global/Global.h"
#include <QSettings>

CFrmMain::CFrmMain(QWidget *parent) :
    QFrame(parent),
    m_UserList(parent),
    m_GroupChatList(parent),
    ui(new Ui::CFrmMain)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    

    ui->tabWidget->addTab(&m_UserList, QIcon(":/icon/User"), tr("Rosters"));
    ui->tabWidget->addTab(&m_GroupChatList, QIcon(":/icon/Users"), tr("Group Chat"));
    ui->tabWidget->addTab(&m_MessageList, QIcon(":/icon/Message"), tr("Recent messages"));
    
    if(USER_INFO_LOCALE.isNull() || USER_INFO_LOCALE->GetInfo().isNull())
        return;
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
    LOG_MODEL_DEBUG("CWdgTableMain", "CWdgTableMain::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());

    ui->tabWidget->resize(geometry().size());
    ui->tabWidget->currentWidget()->resize(ui->tabWidget->geometry().size());
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
