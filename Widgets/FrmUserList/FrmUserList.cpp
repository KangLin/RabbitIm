#include "FrmUserList.h"
#include "ui_FrmUserList.h"
#include "qxmpp/QXmppMessage.h"
#include "qxmpp/QXmppRosterManager.h"
#include "qxmpp/QXmppUtils.h"
#include "../../MainWindow.h"
#include "../FrmUservCard/FrmUservCard.h"
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include "Roster.h"
#include <fstream>
#include <memory> 

CFrmUserList::CFrmUserList(QWidget *parent) :
    QFrame(parent),
    COperateRoster(),
    m_UserList(this),
    ui(new Ui::CFrmUserList)
{
    ui->setupUi(this);

    InitMenu();

    m_pModel = new QStandardItemModel(this);
    if(m_pModel)
    {
        //增加头，只有增加了这个后，下面才会显示内容  
        m_pModel->setHorizontalHeaderLabels(QStringList() << tr("User name or group")<< tr("Information"));
    }

    m_UserList.setModel(m_pModel);
    m_UserList.show();

    InsertGroup(tr("My friends"));

    bool check = connect(&m_UserList, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigMenuInitOperator(QMenu*)),
                    SLOT(slotAddToMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRefresh()),
                    SLOT(slotRefresh()));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigMenuRemoveOperator(QMenu*)),
                    SLOT(slotDeleteFromMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(XMPP_CLIENT,
                         SIGNAL(presenceReceived(const QXmppPresence)),
                         SLOT(slotChangedPresence(QXmppPresence)));
    Q_ASSERT(check);

    check = connect(&XMPP_CLIENT->rosterManager(), SIGNAL(rosterReceived()),
                    SLOT(slotRosterReceived()));
    Q_ASSERT(check);

    check = connect(&XMPP_CLIENT->rosterManager(), SIGNAL(subscriptionReceived(QString)),
                    SLOT(slotSubscriptionReceived(QString)));
    Q_ASSERT(check);

    check = connect(&XMPP_CLIENT->rosterManager(), SIGNAL(itemAdded(QString)),
                    SLOT(slotItemAdded(QString)));
    Q_ASSERT(check);

    check = connect(&XMPP_CLIENT->rosterManager(), SIGNAL(itemChanged(QString)),
                    SLOT(slotItemChanged(QString)));
    Q_ASSERT(check);

    check = connect(&XMPP_CLIENT->rosterManager(), SIGNAL(itemRemoved(QString)),
                    SLOT(slotItemRemoved(QString)));
    Q_ASSERT(check);

    check = connect(&XMPP_CLIENT->vCardManager(), SIGNAL(vCardReceived(QXmppVCardIq)),
                    SLOT(slotvCardReceived(QXmppVCardIq)));
    Q_ASSERT(check);

    check = connect(XMPP_CLIENT, SIGNAL(messageReceived(QXmppMessage)),
                    SLOT(slotClientMessageReceived(QXmppMessage)));
    Q_ASSERT(check);
}

CFrmUserList::~CFrmUserList()
{
    Clean();

    QMap<QString, CRoster*>::iterator it;
    for(it = m_Rosters.begin(); it != m_Rosters.end(); it++)
    {
        delete it.value();
    }

    //删除组 m_Groups，不需要删，列表控件析构时会自己删除  

    delete ui;

    if(m_pModel)
        delete m_pModel;
}

int CFrmUserList::Init()
{
    int nRet = 0;
    GLOBAL_UER->ProcessRoster(this);
    return nRet;
}

int CFrmUserList::Clean()
{
    XMPP_CLIENT->rosterManager().disconnect(this);
    XMPP_CLIENT->vCardManager().disconnect(this);
    XMPP_CLIENT->disconnect(this);
    CGlobal::Instance()->GetMainWindow()->disconnect(this);
    return 0;
}

int CFrmUserList::ProcessRoster(QSharedPointer<CUserInfoRoster> roster, void *para)
{
    int nRet = 0;
      //呢称条目  
    QStandardItem* pItem = new QStandardItem(roster->GetShowName() + roster->GetSubscriptionTypeStr(roster->GetSubScriptionType()));
    pItem->setEditable(true);//允许双击编辑  
    pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
    pItem->setData(roster->GetShowName(), Qt::DisplayRole);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);

    //消息条目  
    QStandardItem* pMessageCountItem = new QStandardItem("");
    pMessageCountItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;

    UpdateGroup(lstItems, roster->GetGroups());
    return nRet;
}

int CFrmUserList::InitMenu()
{
    bool check = true;
    m_Menu.setTitle(tr("Operator roster(&O)"));

    check = connect(&m_Menu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));
    Q_ASSERT(check);

    //菜单设置  
    m_pMenuAction = NULL;
    m_Menu.addAction(ui->actionAddRoster_A);
    check = connect(ui->actionAddRoster_A, SIGNAL(triggered()),
                    SLOT(slotAddRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionRemoveRoster_R);
    check = connect(ui->actionRemoveRoster_R, SIGNAL(triggered()),
                    SLOT(slotRemoveRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionAgreeAddRoster);
    check = connect(ui->actionAgreeAddRoster, SIGNAL(triggered()),
                    SLOT(slotAgreeAddRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionInformation_I);
    check = connect(ui->actionInformation_I, SIGNAL(triggered()),
                    SLOT(slotInformationRoster()));
    Q_ASSERT(check);

    return 0;
}

int CFrmUserList::EnableAllActioins(bool bEnable)
{
    EnableAction(ui->actionAddRoster_A, bEnable);
    EnableAction(ui->actionAgreeAddRoster, bEnable);
    EnableAction(ui->actionRemoveRoster_R, bEnable);
    EnableAction(ui->actionInformation_I, bEnable);
    return 0;
}

int CFrmUserList::EnableAction(QAction *pAction, bool bEnable)
{
    pAction->setEnabled(bEnable);
    pAction->setVisible(bEnable);
    return 0;
}

void CFrmUserList::slotAddToMainMenu(QMenu *pMenu)
{
    m_pMenuAction = pMenu->addMenu(&m_Menu);
    bool check = connect(pMenu, SIGNAL(aboutToShow()),
            SLOT(slotUpdateMainMenu()));
    Q_ASSERT(check);
}

void CFrmUserList::slotDeleteFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pMenuAction);
    pMenu->disconnect(this);
}

void CFrmUserList::slotUpdateMainMenu()
{
    m_Menu.setTitle(tr("Operator roster(&O)"));
    if(isHidden())
        m_Menu.setEnabled(false);
    else
        m_Menu.setEnabled(true);
}

void CFrmUserList::slotUpdateMenu()
{
    m_Menu.setTitle(tr("Operator roster(&O)"));
    m_Menu.setEnabled(true);
    EnableAllActioins(false);
    if(this->isHidden())
        return;

    //如果是组上,显示增加好友  
    EnableAction(ui->actionAddRoster_A);

    //判断是在组上还是在好友上  
    CRoster* p = GetCurrentRoster();
    if(p)
    {
        //增加订阅  
        if(QXmppRosterIq::Item::None == p->GetSubScriptionType()
             || QXmppRosterIq::Item::From == p->GetSubScriptionType())
            EnableAction(ui->actionAgreeAddRoster);

        //如果是好友上,显示删除好友  
        EnableAction(ui->actionRemoveRoster_R);

        //查看好友信息  
        EnableAction(ui->actionInformation_I);
        //TODO: 移动到组  

    }
    else
    {
        //TODO:新建组  
    }
    return;
}

void CFrmUserList::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    m_Menu.exec(QCursor::pos());
}

void CFrmUserList::slotAddRoster()
{
    QSet<QString> groups;
    groups = GetGroupsName();

    m_frmAddRoster.Init(XMPP_CLIENT, groups);
    m_frmAddRoster.show();
    m_frmAddRoster.activateWindow();
}

void CFrmUserList::slotAgreeAddRoster()
{
    CRoster* p = GetCurrentRoster();
    if(p)
       XMPP_CLIENT->rosterManager().subscribe(p->BareJid());
}

void CFrmUserList::slotRemoveRoster()
{
    CRoster* p = GetCurrentRoster();
    if(p)
        XMPP_CLIENT->rosterManager().removeItem(p->BareJid());
}

void CFrmUserList::slotInformationRoster()
{
    CRoster* p = GetCurrentRoster();
    //MODIFY:
   // CFrmUservCard* pvCard = new CFrmUservCard(NULL);
   // pvCard->show();
}

void CFrmUserList::slotClientMessageReceived(const QXmppMessage &message)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::slotClientMessageReceived:type:%d;state:%d;from:%s;to:%s;body:%s",
           message.type(),
           message.state(), //消息的状态 0:消息内容，其它值表示这个消息的状态  
           qPrintable(message.from()),
           qPrintable(message.to()),
           qPrintable(message.body())
          );

    m_LastUser = message.from();//保存接收到最后消息的用户  
    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(message.from()));
    if(m_Rosters.end() != it)
    {
        if(QXmppMessage::None == message.state())
        {
            it.value()->AppendMessage(message.body());
        }
        //TODO:消息输入状态显示  
    }
}

QStandardItem*  CFrmUserList::InsertGroup(QString szGroup)
{
     QStandardItem* lstGroup = NULL;
    lstGroup = new QStandardItem(szGroup);
    lstGroup->setEditable(false);  //禁止双击编辑 
    lstGroup->setData(PROPERTIES_GROUP, USERLIST_ITEM_ROLE_PROPERTIES);
    m_pModel->appendRow(lstGroup);
    m_Groups.insert(szGroup, lstGroup);
    return lstGroup;
}

int CFrmUserList::UpdateGroup(QList<QStandardItem *> &lstItems, QSet<QString> groups)
{
    if(groups.isEmpty())
    {
        QString szDefaulGroup(tr("My friends"));
        groups.insert(szDefaulGroup);
    }

    for(QSet<QString>::iterator itGroup = groups.begin(); itGroup != groups.end(); itGroup++)
    {
        QString szGroup = *itGroup;
        QStandardItem* lstGroup = NULL;
        QMap<QString, QStandardItem*>::iterator it;
        it = m_Groups.find(szGroup);
        if(m_Groups.end() == it)
        {
            //新建立组条目 
            lstGroup = InsertGroup(szGroup);
        }
        else
            lstGroup = it.value();

        lstGroup->appendRow(lstItems);
    }

    return 0;
}

int CFrmUserList::InsertUser(QXmppRosterIq::Item rosterItem)
{
    int nRet = 0;
    LOG_MODEL_DEBUG("FrmUserList", "jid:%s", qPrintable(rosterItem.bareJid()));
    QSharedPointer<CUserInfoRoster> roster;
    roster = GLOBAL_UER->GetUserInfoRoster(rosterItem.bareJid());
    if(!roster.isNull())
        return nRet;

    GLOBAL_UER->UpdateUserInfoRoster(rosterItem);

    //得到好友信息（包括头像图片）  
    XMPP_CLIENT->vCardManager().requestVCard(rosterItem.bareJid());

    //更新列表控件  
    roster = GLOBAL_UER->GetUserInfoRoster(rosterItem.bareJid());
    if(!roster.isNull())
        ProcessRoster(roster);
    return nRet;
}

void CFrmUserList::slotSubscriptionReceived(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::subscriptionReceived:%s", qPrintable(bareJid));
    m_frmAddRoster.Init(XMPP_CLIENT, GetGroupsName(), bareJid);
    m_frmAddRoster.show();
    m_frmAddRoster.activateWindow();
}

void CFrmUserList::slotItemAdded(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemAdded jid:%s", qPrintable(bareJid));
    QXmppRosterIq::Item item = XMPP_CLIENT->rosterManager().getRosterEntry(bareJid);
    InsertUser(item);
}

void CFrmUserList::slotItemChanged(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemChanged jid:%s", qPrintable(bareJid));
    QSharedPointer<CUserInfoRoster> roster = GLOBAL_UER->GetUserInfoRoster(bareJid);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Isn't the roster:%s", qPrintable(bareJid));
        return;
    }

    QXmppRosterIq::Item item = XMPP_CLIENT->rosterManager().getRosterEntry(bareJid);
    GLOBAL_UER->UpdateUserInfoRoster(item);
    //TODO:
    //UpdateGroup(lstItems, item.groups());
}

void CFrmUserList::slotItemRemoved(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemRemoved jid:%s", qPrintable(bareJid));
    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(bareJid));
    if(m_Rosters.end() != it)
    {
        delete it.value();
        m_Rosters.remove(QXmppUtils::jidToBareJid(bareJid));
    }
}

//得到好友列表  
void CFrmUserList::slotRosterReceived()
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList:: Roster received");

    foreach (const QString &bareJid, XMPP_CLIENT->rosterManager().getRosterBareJids())
    {
        QXmppRosterIq::Item item = XMPP_CLIENT->rosterManager().getRosterEntry(bareJid);
        InsertUser(item);
    }
}

//好友出席状态改变  
void CFrmUserList::slotChangedPresence(const QXmppPresence &presence)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::ChangedPresence jid:%s;status:%d;status text:%s",
           qPrintable(presence.from()),
           presence.availableStatusType(),
           qPrintable(presence.statusText())
           );

    QString bareJid = QXmppUtils::jidToBareJid(presence.from());
    QSharedPointer<CUserInfoRoster> roster = GLOBAL_UER->GetUserInfoRoster(bareJid);
    if(!roster.isNull())
        roster->SetStatus(presence.from(), presence.availableStatusType());

    //TODO:更新列表控件状态  
    QModelIndexList lstIndex = m_pModel->match(QModelIndex(), USERLIST_ITEM_ROLE_JID, bareJid, -1);
    QModelIndex index;
    foreach(index, lstIndex)
    {
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ROSTER)
        {
            pItem->setEditable(true);//允许双击编辑  
            pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            pItem->setData(roster->GetShowName(), Qt::DisplayRole);
            pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
        }
        else if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_UNREAD_MESSAGE_COUNT)
        {
            pItem->setData(QString::number(roster->GetUnReadMessageCount()), Qt::DisplayRole);
            pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            pItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
            pItem->setEditable(false);//禁止双击编辑  
        }
    }

    return;
    QMap<QString, CRoster*>::iterator it = m_Rosters.find(bareJid);
    if(m_Rosters.end() != it)
    {
        it.value()->ChangedPresence(presence.from(), presence.availableStatusType());
    }
}

//得到好友形象信息  
void CFrmUserList::slotvCardReceived(const QXmppVCardIq& vCard)
{
    QString jid = vCard.from();
    if(jid.isEmpty())
    {
        LOG_MODEL_ERROR("FrmUserList", "jid is null");
        return;
        Q_ASSERT(false);
    }
    GLOBAL_UER->UpdateUserInfoRoster(vCard, jid);
    //TODO:更新列表控件  
    
    return;
}

void CFrmUserList::clicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::clicked, row:%d; column:%d",
           index.row(), index.column());

#ifdef ANDROID
    if(m_UserList.isExpanded(index))
       m_UserList.collapse(index);
    else
       m_UserList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //TODO:暂时根据是否有根节点来判断是组还是好友  
    if(m->parent(index).isValid())
    {
        //是用户结点，打开消息对话框  
        QVariant v = m->data(index, Qt::UserRole + 1);
        if(v.canConvert<CRoster*>())
        {
            CRoster* p = v.value<CRoster*>();
            p->ShowMessageDialog();
        }
    }
#endif
}

void CFrmUserList::doubleClicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

#ifndef ANDROID
    if(!m_UserList.isExpanded(index))
       m_UserList.collapse(index);
    else
       m_UserList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    //TODO:暂时根据是否有根节点来判断是组还是好友  
    if(m->parent(index).isValid())
    {
        //是用户结点，打开消息对话框  
        QVariant v = m->data(index, Qt::UserRole + 1);
        if(v.canConvert<CRoster*>())
        {
            CRoster* p = v.value<CRoster*>();
            p->ShowMessageDialog();
        }
    }
#endif
}

void CFrmUserList::resizeEvent(QResizeEvent *e)
{
    LOG_MODEL_DEBUG("CFrmUserList", "CFrmUserList::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());
    m_UserList.resize(this->geometry().size());
    //调整列的宽度  
    int nWidth = m_UserList.geometry().width() * 4/ 5;
    m_UserList.setColumnWidth(0, nWidth);
    m_UserList.setColumnWidth(1, m_UserList.geometry().width() - nWidth);
}

void CFrmUserList::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

QSet<QString> CFrmUserList::GetGroupsName()
{
    QSet<QString> groups;
    QMap<QString, QStandardItem*>::iterator it;
    for(it = m_Groups.begin(); it != m_Groups.end(); it++)
        groups << it.key();
    return groups;
}

CRoster* CFrmUserList::GetCurrentRoster()
{
    QModelIndex index = m_UserList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return NULL;

    //TODO:暂时根据是否有根节点来判断是组还是好友  
    if(m->parent(index).isValid())
    {
        //是用户结点
        QVariant v = m->data(index, Qt::UserRole + 1);
        if(v.canConvert<CRoster*>())
        {
            return v.value<CRoster*>();
        }
    }
    return NULL;
}

int CFrmUserList::ShowMessageDialog()
{
    if(m_LastUser.isEmpty())
        return -1;
    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(m_LastUser));
    if(m_Rosters.end() != it)
    {
        it.value()->ShowMessageDialog();
    }
    return 0;
}

CRoster* CFrmUserList::GetRoster(QString szJid)
{
    QMap<QString, CRoster*>::iterator it;
    it = m_Rosters.find(QXmppUtils::jidToBareJid(szJid));
    if(m_Rosters.end() != it)
        return *it;
    return NULL;
}

void CFrmUserList::slotRefresh()
{
    QMap<QString, CRoster*>::iterator it;
    for(it = m_Rosters.begin(); it != m_Rosters.end(); it++)
    {
        it.value()->slotRefresh();
    }
}
