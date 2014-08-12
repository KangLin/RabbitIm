#include "FrmCreateGroupChatRoom.h"
#include "ui_FrmCreateGroupChatRoom.h"
#include <QMessageBox>

CFrmCreateGroupChatRoom::CFrmCreateGroupChatRoom(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmCreateGroupChatRoom)
{
    ui->setupUi(this);
}

CFrmCreateGroupChatRoom::~CFrmCreateGroupChatRoom()
{
    delete ui;
}

CFrmCreateGroupChatRoom* CFrmCreateGroupChatRoom::Instance(const QString &jid)
{
    static CFrmCreateGroupChatRoom* pChat = NULL;
    if(!pChat)
        pChat = new CFrmCreateGroupChatRoom();

    if(pChat)
        pChat->Init(jid);
    return pChat;
}

int CFrmCreateGroupChatRoom::Init(const QString &jid)
{
    m_szJid = jid;
    return 0;
}

void CFrmCreateGroupChatRoom::showEvent(QShowEvent *)
{
}

void CFrmCreateGroupChatRoom::closeEvent(QCloseEvent *)
{
    this->deleteLater();
}

void CFrmCreateGroupChatRoom::on_pbOK_clicked()
{
    if(ui->txtName->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill room name."));
        return;
    }
    if(ui->txtSubject->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Please fill room subject."));
        return;
    }

    emit sigJoinGroup(ui->txtName->text() + "@" + m_szJid);
    close();
}

void CFrmCreateGroupChatRoom::on_pbCancel_clicked()
{
    close();
}
