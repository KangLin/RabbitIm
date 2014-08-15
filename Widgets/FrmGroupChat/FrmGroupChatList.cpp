#include "FrmGroupChatList.h"
#include "ui_FrmGroupChatList.h"
#include "../../Global.h"
#include "FrmGroupChatFind.h"
#include "../../MainWindow.h"
#include "../FrmUserList/Roster.h"

#define ROLE_JID Qt::UserRole + 1
#define ROLE_FORM_GROUPCHAT ROLE_JID + 1

CFrmGroupChatList::CFrmGroupChatList(QWidget *parent) :
    QFrame(parent),
    m_GroupList(this),
    ui(new Ui::CFrmGroupChatList)
{
    ui->setupUi(this);

    bool check = false;
    m_pAction = NULL;

    InitMenu();

    m_pModel = new QStandardItemModel(this);
    if(m_pModel)
    {
        //增加头，只有增加了这个后，下面才会显示内容  
        m_pModel->setHorizontalHeaderLabels(QStringList() << tr("Rooms")<< tr("Information"));
    }

    //禁止列表文本框编辑  
    m_GroupList.setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_GroupList.setModel(m_pModel);
    m_GroupList.show();

    check = connect(&m_GroupList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);
    
    check = connect(&CGlobal::Instance()->GetXmppClient()->m_MucManager, SIGNAL(invitationReceived(QString,QString,QString)),
                    SLOT(slotInvitationReceived(QString,QString,QString)));
    Q_ASSERT(check);

    /*check = connect(&CGlobal::Instance()->GetXmppClient()->m_MucManager, SIGNAL(roomAdded(QXmppMucRoom*)),
                    SLOT(slotRoomAdded(QXmppMucRoom*)));
    Q_ASSERT(check);*/

    check = connect(&m_GroupList, SIGNAL(clicked(QModelIndex)),
                    SLOT(slotClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_GroupList, SIGNAL(doubleClicked(QModelIndex)),
                    SLOT(slotDoubleClicked(QModelIndex)));
    Q_ASSERT(check);
}

CFrmGroupChatList::~CFrmGroupChatList()
{
    QMap<QString, CFrmGroupChat*>::iterator it;
    for(it = m_Group.begin(); it != m_Group.end(); it++)
    {
        delete *it;
    }

    delete ui;
    if(m_pModel)
        delete m_pModel;
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
    m_Menu.addAction(QIcon(":/icon/Search"), tr("New/Search group chat rooms"), this, SLOT(slotActionFindGroup()));
    m_Menu.addAction(QIcon(":/icon/Left"), tr("Leave room"), this, SLOT(slotActionLeave()));

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

void CFrmGroupChatList::resizeEvent(QResizeEvent* e)
{
    LOG_MODEL_DEBUG("CFrmGroupChatList", "CFrmGroupChatList::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());
    m_GroupList.resize(this->geometry().size());
    
    //调整列的宽度  
    int nWidth = m_GroupList.geometry().width() * 4/ 5;
    m_GroupList.setColumnWidth(0, nWidth);
    m_GroupList.setColumnWidth(1, m_GroupList.geometry().width() - nWidth);
}

void CFrmGroupChatList::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    m_Menu.exec(QCursor::pos());
}

void CFrmGroupChatList::slotAddToMainMenu(QMenu *pMenu)
{
    m_pAction = pMenu->addMenu(&m_Menu);
    bool check = connect(pMenu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMainMenu()));
    Q_ASSERT(check);
}

void CFrmGroupChatList::slotUpdateMenu()
{
    if(this->isHidden())
        m_Menu.setEnabled(false);
    else
        m_Menu.setEnabled(true);
}

void CFrmGroupChatList::slotUpdateMainMenu()
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChatList isHidden:%s",
                    qPrintable(QString::number(isHidden())));
    if(this->isHidden())
        m_Menu.setEnabled(false);
    else
        m_Menu.setEnabled(true);
}

//TODO:如果此对象析构,主菜单上的菜单如何删除?  
void CFrmGroupChatList::slotRemoveFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pAction);
    pMenu->disconnect(this);
}

void CFrmGroupChatList::slotActionFindGroup()
{
    CFrmGroupChatFind * pFrm = CFrmGroupChatFind::Instance();
    bool check = connect(pFrm, SIGNAL(sigJoinedGroup(QString,CFrmGroupChat*)),
                         SLOT(slotJoinedGroup(QString,CFrmGroupChat*)));
    Q_ASSERT(check);

    pFrm->show();
}

void CFrmGroupChatList::slotActionLeave()
{
    QModelIndex index = m_GroupList.currentIndex();
    if(!index.isValid())
        return;
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_OBJECT);
    CFrmGroupChat* chat = v.value<CFrmGroupChat*>();
    chat->Leave();//当聊天室离开后,会触发 sigLeft 事件,然后在 slotLeft 中处理  
}

void CFrmGroupChatList::slotJoinedGroup(const QString &jid, CFrmGroupChat *pChat)
{
    if(m_Group.find(jid) != m_Group.end())
    {
        LOG_MODEL_DEBUG("Group chat", "group chat [%s] is exist", jid.toStdString().c_str());
        return;
    }

    bool check = connect(pChat, SIGNAL(sigLeft(QString,CFrmGroupChat*)),
                         SLOT(slotLeft(QString,CFrmGroupChat*)));
    Q_ASSERT(check);
    
    QList<QStandardItem*> pItem = pChat->GetItem();
    m_pModel->appendRow(pItem);
    m_Group[jid] = pChat;
}

void CFrmGroupChatList::slotLeft(const QString &jid, CFrmGroupChat *pChat)
{
    QList<QStandardItem* > item = pChat->GetItem();
    if(!item.isEmpty())
    {
        QStandardItem* pItem = *item.begin();
        if(pItem)
            if(-1 != pItem->row())
            {
                m_pModel->removeRow(pItem->row());
            }
    }
    m_Group.remove(jid);
}

void CFrmGroupChatList::slotClicked(const QModelIndex &index)
{
#ifdef MOBILE
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_OBJECT);
    CFrmGroupChat* chat = v.value<CFrmGroupChat*>();
    chat->show();
#endif
}

void CFrmGroupChatList::slotDoubleClicked(const QModelIndex &index)
{
#ifndef ANDROID
    const QAbstractItemModel* m = index.model();
    QVariant v = m->data(index, CFrmGroupChat::ROLE_GROUPCHAT_OBJECT);
    CFrmGroupChat* chat = v.value<CFrmGroupChat*>();
    chat->show();
    chat->activateWindow();
#endif
}
