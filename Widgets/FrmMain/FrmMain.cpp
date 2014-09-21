#include "FrmMain.h"
#include "ui_FrmMain.h"
#include "../../Global/Global.h"

CFrmMain::CFrmMain(QWidget *parent) :
    QFrame(parent),
    m_UserList(parent),
    //m_GroupChatList(parent),
    ui(new Ui::CFrmMain)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    
    ui->tabWidget->addTab(&m_UserList, QIcon(":/icon/User"), tr("Rosters"));
    //ui->tabWidget->addTab(&m_GroupChatList, QIcon(":/icon/Users"), tr("Group Chat"));
}

CFrmMain::~CFrmMain()
{
    delete ui;
}

int CFrmMain::Init()
{
    int nRet = 0;
  
    return 0;
}

void CFrmMain::resizeEvent(QResizeEvent *e)
{
    LOG_MODEL_DEBUG("CWdgTableMain", "CWdgTableMain::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());

    m_UserList.resize(ui->tabWidget->currentWidget()->geometry().size());
    //m_GroupChatList.resize(ui->tabWidget->currentWidget()->geometry().size());
}

void CFrmMain::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_UserList), tr("Rosters"));
        //ui->tabWidget->setTabText(ui->tabWidget->indexOf(&m_GroupChatList), tr("Group Chat"));
        break;
    }
}
