#include "DlgCreateGroupChatRoom.h"
#include "ui_DlgCreateGroupChatRoom.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include "Global/Global.h"
#include "ManageGroupChat.h"
#include "Tool.h"

CDlgCreateGroupChatRoom::CDlgCreateGroupChatRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgCreateGroupChatRoom)
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);

    bool check = connect(GETMANAGER->GetManageGroupChat().data(), 
                         SIGNAL(sigJoined(QString)),
                         SLOT(accept()));
    Q_ASSERT(check);

    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigError(QString,CGroupChat::Condition, int)),
                    SLOT(slotError(QString,CGroupChat::Condition, int)));
    Q_ASSERT(check);
}

CDlgCreateGroupChatRoom::~CDlgCreateGroupChatRoom()
{
    delete ui;
}

void CDlgCreateGroupChatRoom::showEvent(QShowEvent *)
{
    ui->txtConfirmPassword->clear();
    ui->txtName->clear();
    ui->txtPassword->clear();
    ui->txtSubject->clear();
    ui->txtDescription->clear();
    ui->cbPrivated->setChecked(false);
    ui->cbProtracted->setChecked(false);
}

void CDlgCreateGroupChatRoom::closeEvent(QCloseEvent *)
{
}

void CDlgCreateGroupChatRoom::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CDlgCreateGroupChatRoom::on_pbOK_clicked()
{
    QString szName = ui->txtName->text();
    if(szName.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill room name."));
        return;
    }
    if(szName.indexOf(QRegExp("[^a-zA-Z0-9]")) != -1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Name must only be letters or numbers."));
        return;
    }
    if(ui->txtSubject->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill room subject."));
        return;
    }

    if(ui->txtConfirmPassword->text() != ui->txtPassword->text())
    {
        QMessageBox::critical(this, tr("Error"), tr("Enter the password is not the same twice."));
        return;
    }

    QSharedPointer<CManageGroupChat> groupChat = GETMANAGER->GetManageGroupChat();
    if(groupChat.isNull())
    {
        qCritical("CManageGroupChat is null");
        return;
    }
    
    bool bExist = GETMANAGER->GetManageGroupChat()->IsJoined(ui->txtName->text());
    if(bExist)
    {
        QMessageBox::warning(this, "Group chat", tr("%1 is existed").arg(ui->txtName->text()));
        return;
    }

    groupChat->Create(ui->txtName->text(), 
                      ui->txtSubject->text(), 
                      ui->txtPassword->text(),
                      ui->txtDescription->text(),
                      ui->cbProtracted->isChecked(),
                      ui->cbPrivated->isChecked(),
                      ui->txtNick->text());
}

void CDlgCreateGroupChatRoom::on_pbCancel_clicked()
{
    this->reject();
}

void CDlgCreateGroupChatRoom::slotError(const QString &szId, CGroupChat::Condition c, int ErrCode)
{
    Q_UNUSED(szId);
    QString szText = tr("Unknow error.");
    switch (c) {
    case CGroupChat::NotAuthorized:
        szText = tr("Not authorized.");
        break;
    default:
        szText += tr("Error code:") + QString::number(ErrCode);
        break;
    }
    QMessageBox::critical(this, tr("Error"), szText);
}
