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
    
    bool check = connect(GETMANAGER->GetManageGroupChat().data(), 
                         SIGNAL(sigJoined(QString)),
                         SLOT(accept()));
    Q_ASSERT(check);
    
    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigError(QString,CGroupChat::Condition)),
                    SLOT(slotError(QString,CGroupChat::Condition)));
    Q_ASSERT(check);
}

CDlgJoinGroupChat::~CDlgJoinGroupChat()
{
    GETMANAGER->GetManageGroupChat()->disconnect(this);
    delete ui;
}

void CDlgJoinGroupChat::on_buttonBox_accepted()
{
    if(ui->txtId->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill group chat name."));
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

void CDlgJoinGroupChat::slotError(const QString &szId, CGroupChat::Condition c)
{
    QString szText = tr("Unknow error");
    switch (c) {
    case CGroupChat::NotAuthorized:
        szText = tr("Not authorized");
        break;
    default:
        break;
    }
    QMessageBox::critical(this, tr("Error"), szText);
}
