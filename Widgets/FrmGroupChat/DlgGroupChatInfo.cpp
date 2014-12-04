#include "DlgGroupChatInfo.h"
#include "ui_DlgGroupChatInfo.h"
#include "GroupChat.h"
#include "Global/Global.h"
#include "ManageGroupChat.h"
#include <QDesktopWidget>

CDlgGroupChatInfo::CDlgGroupChatInfo(const QString &szId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgGroupChatInfo)
{
    ui->setupUi(this);
    
    QDesktopWidget *pDesk = QApplication::desktop();
#ifdef MOBILE
    this->setGeometry(pDesk->geometry());
#else
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);
#endif

    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    if(gc.isNull())
        return;

    ui->txtName->setText(gc->ShowName());
    ui->txtSubject->setText(gc->Subject());
    ui->txtNick->setText(gc->Nick());
    ui->txtDescription->setText(gc->Description());
    ui->cbPrivate->setCheckable(true);
    ui->cbPrivate->setChecked(gc->IsPrivate());
    ui->cbProtracted->setCheckable(true);
    ui->cbProtracted->setChecked(gc->IsProtracted());
}

CDlgGroupChatInfo::~CDlgGroupChatInfo()
{
    delete ui;
}

void CDlgGroupChatInfo::on_pushButton_clicked()
{
    this->accept();
}
