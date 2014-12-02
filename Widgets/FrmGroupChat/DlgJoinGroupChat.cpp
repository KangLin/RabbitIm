#include "DlgJoinGroupChat.h"
#include "ui_DlgJoinGroupChat.h"
#include "Global/Global.h"
#include "ManageGroupChat.h"
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

CDlgJoinGroupChat::CDlgJoinGroupChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgJoinGroupChat)
{
    ui->setupUi(this);
    ui->txtNick->setText(USER_INFO_LOCALE->GetInfo()->GetName());
    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->geometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif
}

CDlgJoinGroupChat::~CDlgJoinGroupChat()
{
    delete ui;
}

void CDlgJoinGroupChat::on_buttonBox_accepted()
{
    if(ui->txtId->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill group chat name."));
        return;
    }
   
    if(ui->txtNick->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill group chat nick."));
        return;
    }
    GETMANAGER->GetManageGroupChat()->Join(ui->txtId->text(), ui->txtNick->text());
}
