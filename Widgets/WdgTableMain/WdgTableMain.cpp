#include "WdgTableMain.h"
#include "ui_WdgTableMain.h"

CWdgTableMain::CWdgTableMain(QWidget *parent) :
    QTabWidget(parent),
    m_UserList(parent),
    ui(new Ui::CWdgTableMain)
{
    ui->setupUi(this);

    this->addTab(&m_UserList, QIcon(":/icon/User"), tr("Rosters"));
    addTab(&m_GroupChatList, QIcon(":/icon/Users"), tr("Group Chat"));
}

CWdgTableMain::~CWdgTableMain()
{
    delete ui;
}

CRoster* CWdgTableMain::GetRoster(QString jid)
{
    return m_UserList.GetRoster(jid);
}

int CWdgTableMain::ShowMessageDialog()
{
    return m_UserList.ShowMessageDialog();
}
