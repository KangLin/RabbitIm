#include "DlgJoinGroupChat.h"
#include "ui_DlgJoinGroupChat.h"
#include "Global/Global.h"
#include "ManageGroupChat.h"

CDlgJoinGroupChat::CDlgJoinGroupChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgJoinGroupChat)
{
    ui->setupUi(this);
    ui->txtNick->setText(USER_INFO_LOCALE->GetInfo()->GetName());
}

CDlgJoinGroupChat::~CDlgJoinGroupChat()
{
    delete ui;
}

void CDlgJoinGroupChat::on_buttonBox_accepted()
{
    GETMANAGER->GetManageGroupChat()->Join(ui->txtId->text(), ui->txtNick->text());
}
