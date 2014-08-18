#include "FrmMain.h"
#include "ui_FrmMain.h"
#include "../../Global.h"

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
}

CFrmMain::~CFrmMain()
{
    delete ui;
}

CRoster* CFrmMain::GetRoster(QString jid)
{
    return m_UserList.GetRoster(jid);
}

int CFrmMain::ShowMessageDialog()
{
    return m_UserList.ShowMessageDialog();
}

void CFrmMain::resizeEvent(QResizeEvent *e)
{
    LOG_MODEL_DEBUG("CWdgTableMain", "CWdgTableMain::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());

    m_UserList.resize(ui->tabWidget->geometry().size());
    m_GroupChatList.resize(ui->tabWidget->geometry().size());
}

void CFrmMain::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}
