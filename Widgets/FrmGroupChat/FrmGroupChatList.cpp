#include "FrmGroupChatList.h"
#include "ui_FrmGroupChatList.h"
#include "../../Global.h"
#include "FrmGroupChatFind.h"
#include "../../MainWindow.h"

CFrmGroupChatList::CFrmGroupChatList(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChatList)
{
    ui->setupUi(this);

    //setContextMenuPolicy(Qt::CustomContextMenu);

    bool check = false;
    m_pAction = NULL;

    InitMenu();

    QList<QXmppMucRoom*> rooms = CGlobal::Instance()->GetXmppClient()->m_MucManager.rooms();
    QList<QXmppMucRoom*>::iterator it;
    for(it = rooms.begin(); it != rooms.end(); it++)
    {
        LOG_MODEL_DEBUG("GroupChat", "roomjid:%s", (*it)->jid().toStdString().c_str());
    }

    check = connect(&CGlobal::Instance()->GetXmppClient()->m_MucManager, SIGNAL(invitationReceived(QString,QString,QString)),
                    SLOT(slotInvitationReceived(QString,QString,QString)));
    Q_ASSERT(check);

    check = connect(&m_Menu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));
    Q_ASSERT(check);
}

CFrmGroupChatList::~CFrmGroupChatList()
{
    delete ui;
}

void CFrmGroupChatList::contextMenuEvent(QContextMenuEvent * event)
{
    Q_UNUSED(event);
    m_Menu.exec(QCursor::pos());
}

void CFrmGroupChatList::slotInvitationReceived(const QString &roomJid, const QString &inviter, const QString &reason)
{
    LOG_MODEL_DEBUG("CFrmGroupChatList", "roomJid:%s, inviter:%s, reason:%s",
                    roomJid.toStdString().c_str(), 
                    inviter.toStdString().c_str(),
                    reason.toStdString().c_str());
}

void CFrmGroupChatList::slotRoomAdded(QXmppMucRoom *room)
{
    LOG_MODEL_DEBUG("CFrmGroupChatList", "CFrmGroupChatList::slotRoomAdded:jid:%s,name:%s,nick:%s,subject:%s",
                    room->jid().toStdString().c_str(),
                    room->name().toStdString().c_str(),
                    room->nickName().toStdString().c_str(),
                    room->subject().toStdString().c_str());
}

int CFrmGroupChatList::InitMenu()
{
    m_Menu.setTitle(tr("Operator group chat(&G)"));
    m_Menu.addAction(QIcon(":/icon/Search"), tr("Search group chat rooms"), this, SLOT(slotFindGroup()));

    bool check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigInitLoginedMenu(QMenu*)),
                    SLOT(slotAddToMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRemoveMenu(QMenu*)),
                    SLOT(slotRemoveFromMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(&m_Menu, SIGNAL(aboutToShow()), SLOT(slotUpdateMenu()));
    Q_ASSERT(check);

    return 0;
}

void CFrmGroupChatList::slotAddToMainMenu(QMenu *pMenu)
{
    m_pAction = pMenu->addMenu(&m_Menu);
}

void CFrmGroupChatList::slotRemoveFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pAction);
}

void CFrmGroupChatList::slotFindGroup()
{
    CFrmGroupChatFind * pFrm = new CFrmGroupChatFind();//窗体自己释放内存，不会泄漏  
    pFrm->show();
}

void CFrmGroupChatList::slotUpdateMenu()
{
}
