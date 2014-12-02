#include "DlgGroupChatInfo.h"
#include "ui_DlgGroupChatInfo.h"
#include "GroupChat.h"
#include "Global/Global.h"
#include "ManageGroupChat.h"

CDlgGroupChatInfo::CDlgGroupChatInfo(const QString &szId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgGroupChatInfo)
{
    ui->setupUi(this);

    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    if(gc.isNull())
        return;

    ui->txtName->setText(gc->ShowName());
    ui->txtSubject->setText(gc->Subject());
    ui->txtNick->setText(gc->Nick());
    
}

CDlgGroupChatInfo::~CDlgGroupChatInfo()
{
    delete ui;
}

void CDlgGroupChatInfo::on_pushButton_clicked()
{
    this->accept();
}
