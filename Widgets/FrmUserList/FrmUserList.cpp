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

    check = connect(XMPP_CLIENT.data(), SIGNAL(sigChangedStatus(const QXmppPresence)),
                    SLOT(SlotChangedStatus(QXmppPresence)));
    Q_ASSERT(check);

    check = connect(XMPP_CLIENT.data(), SIGNAL(sigLoadRosterFromStorage()),
                    SLOT(slotLoadRosterFromStorage()));
    Q_ASSERT(check);

    check = connect(XMPP_CLIENT.data(), SIGNAL(sigUpdateRosterUserInfo(QString)),
                    SLOT(slotUpdateRosterUserInfo(QString)));
    Q_ASSERT(check);

    check = connect(XMPP_CLIENT.data(), SIGNAL(sigRosterSubscriptionReceived(QString)),
                    SLOT(slotRosterSubscriptionReceived(QString)));
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
    return nRet;
}

int CFrmUserList::Clean()
{
    XMPP_CLIENT->disconnect(this);
    CGlobal::Instance()->GetMainWindow()->disconnect(this);
    return 0;
}

int CFrmUserList::ProcessRoster(QSharedPointer<CUserInfo> roster, void *para)
{
    int nRet = 0;
    int * p = (int*)para;
    int type = *((int*)p);
    switch(type)
    {
    case OPERATE_TYPE_INSERT_ROSTER:
        nRet = InsertRosterItem(roster);
        break;
    case OPERATE_TYPE_UPDATE_ROSTER:
        nRet = UpdateRosterItem(roster->GetId());
        break;
    default:
        LOG_MODEL_ERROR("FrmUserList", "Operate type is error");
        nRet = -1;
        break;
    }
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
    QString bareJid = GetCurrentRoster();
    if(bareJid.isEmpty())
    {
        //TODO:新建组  
    }
    else
    {
        //增加订阅  
        if(QXmppRosterIq::Item::None == GLOBAL_USER->GetUserInfoRoster(bareJid)->GetSubScriptionType()
             || QXmppRosterIq::Item::From == GLOBAL_USER->GetUserInfoRoster(bareJid)->GetSubScriptionType())
            EnableAction(ui->actionAgreeAddRoster);

        //如果是好友上,显示删除好友  
        EnableAction(ui->actionRemoveRoster_R);

        //查看好友信息  
        EnableAction(ui->actionInformation_I);
        //TODO: 移动到组  
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
    QString bareJid = GetCurrentRoster();
    if(!bareJid.isEmpty())
       XMPP_CLIENT->rosterManager().subscribe(bareJid);
}

void CFrmUserList::slotRemoveRoster()
{
    QString bareJid = GetCurrentRoster();
    if(!bareJid.isEmpty())
        XMPP_CLIENT->rosterManager().removeItem(bareJid);
}

void CFrmUserList::slotInformationRoster()
{
    QString bareJid = GetCurrentRoster();
    CFrmUservCard* pvCard = new CFrmUservCard(GLOBAL_USER->GetUserInfoRoster(bareJid));
    pvCard->show();
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
    lstGroup->setData("", USERLIST_ITEM_ROLE_JID);
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

int CFrmUserList::InsertRosterItem(const QString& szId)
{
    int nRet = 0;
    QSharedPointer<CUserInfo> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster:%s", qPrintable(szId));
        return -1;
    }

    //呢称条目  
    QStandardItem* pItem = new QStandardItem(roster->GetShowName() + roster->GetSubscriptionTypeStr(roster->GetSubScriptionType()));
    pItem->setEditable(true);//允许双击编辑  
    pItem->setData(roster->GetId(), USERLIST_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
    //改变item背景颜色  
    pItem->setData(CGlobal::Instance()->GetRosterStatusColor(roster->GetStatus()), Qt::BackgroundRole);
    QString szText;
    szText = roster->GetShowName()
            + "[" + CGlobal::Instance()->GetRosterStatusText(roster->GetStatus()) + "]"
            +  roster->GetSubscriptionTypeStr(roster->GetSubScriptionType());
    pItem->setData(szText, Qt::DisplayRole); //改变item文本,或者直接用 pItem->setText(szText);  

    //改变item图标  
    pItem->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(roster->GetStatus())), Qt::DecorationRole);

    //消息条目  
    QStandardItem* pMessageCountItem = new QStandardItem("");
    pMessageCountItem->setData(roster->GetId(), USERLIST_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;

    UpdateGroup(lstItems, roster->GetGroups());
    return nRet;
}

int CFrmUserList::UpdateRosterItem(const QString &szId)
{
    QSharedPointer<CUserInfo> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster:%s", qPrintable(szId));
        return -1;
    }

    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                roster->GetId(), 
                                                -1,
                                                Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    if(lstIndexs.isEmpty())
        return InsertRosterItem(szId);

    QModelIndex index;
    foreach(index, lstIndexs)
    {
        LOG_MODEL_DEBUG("FrmUserList", "index:row:%d;column:%d", index.row(), index.column());
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        QStandardItem* pItemUnReadMessageCount = pItem->parent()->child(index.row(), index.column() + 1);
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ROSTER)
        {
            //pItem->setEditable(true);//允许双击编辑  
            //pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            //pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
            //改变item背景颜色  
            pItem->setData(CGlobal::Instance()->GetRosterStatusColor(roster->GetStatus()), Qt::BackgroundRole);
            QString szText;
            szText = roster->GetShowName()
                    + "[" + CGlobal::Instance()->GetRosterStatusText(roster->GetStatus()) + "]"
                    +  roster->GetSubscriptionTypeStr(roster->GetSubScriptionType());
            pItem->setData(szText, Qt::DisplayRole); //改变item文本  
    
            //改变item图标  
            pItem->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(roster->GetStatus())), Qt::DecorationRole);
        }
        if(pItemUnReadMessageCount->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_UNREAD_MESSAGE_COUNT)
        {
            if(roster->GetUnReadMessageCount() == 0)
                pItemUnReadMessageCount->setText("");
            else
                pItemUnReadMessageCount->setText(QString::number(roster->GetUnReadMessageCount()));//pItemUnReadMessageCount->setData(QString::number(roster->GetUnReadMessageCount()), Qt::DisplayRole);
            pItemUnReadMessageCount->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
            //pItemUnReadMessageCount->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            //pItemUnReadMessageCount->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
            //pItemUnReadMessageCount->setEditable(false);//禁止双击编辑  
        }
    }
    return 0;
}

int CFrmUserList::RemoveRosterItem(const QString &szId)
{
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                szId, 
                                                -1,
                                                Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    QModelIndex index;
    foreach(index, lstIndexs)
    {
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        pItem->parent()->removeRow(index.row());
    }
    return 0;
}

void CFrmUserList::slotRosterSubscriptionReceived(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::subscriptionReceived:%s", qPrintable(bareJid));
    m_frmAddRoster.Init( GetGroupsName(), bareJid);
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
    QSharedPointer<CUserInfoRoster> roster = GLOBAL_USER->GetUserInfoRoster(bareJid);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Isn't the roster:%s", qPrintable(bareJid));
        return;
    }

    QXmppRosterIq::Item item = XMPP_CLIENT->rosterManager().getRosterEntry(bareJid);
    GLOBAL_USER->UpdateUserInfoRoster(item);
    UpdateRosterItem(bareJid);
}

void CFrmUserList::slotItemRemoved(const QString &bareJid)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::itemRemoved jid:%s", qPrintable(bareJid));
    GLOBAL_USER->RemoveUserInfoRoster(bareJid);
    RemoveRosterItem(bareJid);
}

//得到好友列表  
void CFrmUserList::slotLoadRosterFromStorage()
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList:: Roster received");

    int type = OPERATE_TYPE_INSERT_ROSTER;
    GLOBAL_USER->ProcessRoster(this, &type);
}

void CFrmUserList::slotUpdateRosterUserInfo(const QString &szJid)
{
    UpdateRosterItem(szJid);
}

//好友出席状态改变  
void CFrmUserList::SlotChangedStatus(const QString &szId)
{
    //更新列表控件状态  
    UpdateRosterItem(szId);
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
    LOG_MODEL_DEBUG("CFrmUserList", "CFrmUserList::resizeEvent:e.size:%d;genmetry.size:%d;userlist.framewidth:%d;width:%d",
                    e->size().width(),
                    geometry().size().width(),
                    m_UserList.frameGeometry().width(),
                    m_UserList.width());
    m_UserList.resize(this->geometry().size());
    //调整列的宽度  
    int nWidth = m_UserList.width() * 4/ 5;
    m_UserList.setColumnWidth(0, nWidth);
    m_UserList.setColumnWidth(1, m_UserList.width() - nWidth - 5);
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

QString CFrmUserList::GetCurrentRoster()
{
    QModelIndex index = m_UserList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return QString();

    if(PROPERTIES_ROSTER != m->data(index, USERLIST_ITEM_ROLE_PROPERTIES))
    {
        return QString();
    }

    //是用户结点
    QVariant v = m->data(index, USERLIST_ITEM_ROLE_JID);
    if(v.canConvert<QString>())
    {
        return v.value<QString>();
    }
    return QString();
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

//TODO:删除  
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
    int type = OPERATE_TYPE_UPDATE_ROSTER;
    GLOBAL_USER->ProcessRoster(this, &type);
}
