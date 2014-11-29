#include "DlgJoinGroupChat.h"
#include "ui_DlgJoinGroupChat.h"
#include "Global/Global.h"
#include "ManageGroupChat.h"

CDlgJoinGroupChat::CDlgJoinGroupChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgJoinGroupChat)
{
    ui->setupUi(this);
}

CDlgJoinGroupChat::~CDlgJoinGroupChat()
{
    delete ui;
}

void CDlgJoinGroupChat::on_buttonBox_accepted()
{
    GETMANAGER->GetManageGroupChat()->Join(ui->txtId->text(), ui->txtNick->text());
}

void CDlgJoinGroupChat::on_txtId_textEdited(const QString &arg1)
{
    
}

void CDlgJoinGroupChat::on_txtId_editingFinished()
{
    if(ui->txtNick->text().isEmpty())
    {
        ui->txtNick->setText(ui->txtId->text());
    }
}
