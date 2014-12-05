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
    Init();
    ui->lbInviter->setVisible(false);
    ui->txtId->setEnabled(true);
}

CDlgJoinGroupChat::CDlgJoinGroupChat(const QString &szGroupChatId, const QString &szInviter, const QString &szReason, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CDlgJoinGroupChat)
{
    Init();
    ui->lbInviter->setVisible(true);
    QString szMsg;
    szMsg = tr("%1 invite you join group chat.").arg(szInviter);
    if(!szReason.isEmpty())
    {
        szMsg += "\nInviter message:" + szReason;
    }
    ui->lbInviter->setText(szMsg);
    ui->txtId->setEnabled(false);
    ui->txtId->setText(szGroupChatId);
}

CDlgJoinGroupChat::~CDlgJoinGroupChat()
{
    GETMANAGER->GetManageGroupChat()->disconnect(this);
    delete ui;
}

int CDlgJoinGroupChat::Init()
{
    ui->setupUi(this);
    CTool::SetWindowsGeometry(this);
    ui->txtNick->setText(USER_INFO_LOCALE->GetInfo()->GetName());

    bool check = connect(GETMANAGER->GetManageGroupChat().data(), 
                         SIGNAL(sigJoined(QString)),
                         SLOT(accept()));
    Q_ASSERT(check);
    
    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigError(QString,CGroupChat::Condition,int)),
                    SLOT(slotError(QString,CGroupChat::Condition, int)));
    Q_ASSERT(check);
    return 0;
}

void CDlgJoinGroupChat::on_buttonBox_accepted()
{
    if(ui->txtId->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill group chat name."));
        return;
    }

    if(ui->txtId->text().indexOf(QRegExp("[^a-zA-Z0-9@\.]")) != -1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Name must only be letters or numbers."));
        return;
    }

    if(ui->txtNick->text().indexOf(QRegExp("[^a-zA-Z0-9]")) != -1)
    {
        QMessageBox::critical(this, tr("Error"), tr("Nick must only be letters or numbers."));
        return;
    }

    bool bExist = GETMANAGER->GetManageGroupChat()->IsJoined(ui->txtId->text());
    if(bExist)
    {
        QMessageBox::warning(this, "Group chat", tr("%1 is existed").arg(ui->txtId->text()));
        return;
    }

    GETMANAGER->GetManageGroupChat()->Join(ui->txtId->text(), ui->txtPassword->text(), ui->txtNick->text());
}

void CDlgJoinGroupChat::slotError(const QString &szId, CGroupChat::Condition c, int errorno)
{
    QString szText = tr("Unknow error.");
    switch (c) {
    case CGroupChat::NotAuthorized:
        szText = tr("Not authorized.");
        break;
    default:
        szText += tr("Error code:") + QString::number(errorno);
        break;
    }
    QMessageBox::critical(this, tr("Error"), szText);
}
