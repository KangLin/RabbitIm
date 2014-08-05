#include "WdgTableMain.h"
#include "ui_WdgTableMain.h"

CWdgTableMain::CWdgTableMain(QWidget *parent) :
    QTabWidget(parent),
    m_UserList(parent),
    ui(new Ui::CWdgTableMain)
{
    ui->setupUi(this);

    this->addTab(&m_UserList, tr("Rosters"));
}

CWdgTableMain::~CWdgTableMain()
{
    delete ui;
}

CRoster* CWdgTableMain::GetRoster(QString jid)
{
    return m_UserList.GetRoster(jid);
}
