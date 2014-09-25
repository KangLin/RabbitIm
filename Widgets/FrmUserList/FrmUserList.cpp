#include "FrmUserList.h"
#include "ui_FrmUserList.h"
#include "../../MainWindow.h"
#include "../FrmUservCard/FrmUservCard.h"
#include "Client/Client.h"
#include "Global/Global.h"
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
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

    ItemInsertGroup(tr("My friends"));

    bool check = connect(&m_UserList, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);

    check = connect(&m_UserList, SIGNAL(entered(QModelIndex)),
                    SLOT(slotEntered(QModelIndex)));
    Q_ASSERT(check);

    check = connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)),
                    SLOT(slotItemChanged(QStandardItem*)));
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

    check = connect(GET_CLIENT.data(), SIGNAL(sigChangedStatus(const QString&)),
                    SLOT(SlotChangedStatus(const QString&)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigLoadRosterFromStorage()),
                    SLOT(slotLoadRosterFromStorage()));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigUpdateRosterUserInfo(QString,QSharedPointer<CUserInfo>)),
                    SLOT(slotUpdateRosterUserInfo(QString,QSharedPointer<CUserInfo>)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigRosterAddReceived(const QString&, const CClient::SUBSCRIBE_TYPE &)),
                    SLOT(slotRosterAddReceived(const QString&, const CClient::SUBSCRIBE_TYPE &)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigRemoveRosterUserInfo(QString)),
                    SLOT(slotRemoveRosterUserInfo(QString)));
    Q_ASSERT(check);
}

CFrmUserList::~CFrmUserList()
{
    GET_CLIENT->disconnect(this);
    CGlobal::Instance()->GetMainWindow()->disconnect(this);

    //删除组 m_Groups，不需要删，列表控件析构时会自己删除  

    delete ui;

    if(m_pModel)
        delete m_pModel;
}

int CFrmUserList::ProcessRoster(QSharedPointer<CUserInfo> roster, void *para)
{
    int nRet = 0;
    int * p = (int*)para;
    int type = *((int*)p);
    switch(type)
    {
    case OPERATE_TYPE_INSERT_ROSTER:
        nRet = ItemInsertRoster(roster->GetId());
        break;
    case OPERATE_TYPE_UPDATE_ROSTER:
        nRet = ItemUpdateRoster(roster->GetId());
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
    m_Menu.setIcon(QIcon(":/icon/Operate"));

    check = connect(&m_Menu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));
    Q_ASSERT(check);

    //菜单设置  
    m_pMenuAction = NULL;
    m_Menu.addAction(ui->actionAddRoster_A);
    check = connect(ui->actionAddRoster_A, SIGNAL(triggered()),
                    SLOT(slotAddRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionRename);
    check = connect(ui->actionRename, SIGNAL(triggered()),
                     SLOT(slotRenameRoster()));
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
    EnableAction(ui->actionRename, bEnable);
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
        if(CUserInfo::From == GLOBAL_USER->GetUserInfoRoster(bareJid)->GetSubScriptionType())
            EnableAction(ui->actionAgreeAddRoster);

        //显示重命名菜单  
        EnableAction(ui->actionRename);
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
    QString szId = GetCurrentRoster();
    if(!szId.isEmpty() && CUserInfo::None == GLOBAL_USER->GetUserInfoRoster(szId)->GetSubScriptionType())
            m_frmAddRoster.Init(groups, szId);
    else
        m_frmAddRoster.Init(groups);
    m_frmAddRoster.show();
    m_frmAddRoster.activateWindow();
}

void CFrmUserList::slotRenameRoster()
{
    QString szName;
    if(GLOBAL_USER->GetUserInfoRoster(GetCurrentRoster()).isNull())
        return;
    szName = GLOBAL_USER->GetUserInfoRoster(GetCurrentRoster())->GetShowName();
    bool ok;
    QString text = QInputDialog::getText(this, tr("Roster[%1] rename").arg(szName),
                                         tr("Roster[%1] rename:").arg(szName), QLineEdit::Normal,
                                         szName, &ok);
    if (ok && !text.isEmpty())
        GET_CLIENT->RosterRename(GetCurrentRoster(), text);
}

void CFrmUserList::slotRemoveRoster()
{
    QString szId = GetCurrentRoster();
    if(szId.isEmpty())
        return;
    GET_CLIENT->RosterRemove(szId);
}

void CFrmUserList::slotAgreeAddRoster()
{
    GET_CLIENT->RosterAdd(GetCurrentRoster(), CClient::SUBSCRIBE_REQUEST);
}

void CFrmUserList::slotInformationRoster()
{
    QString bareJid = GetCurrentRoster();
    CFrmUservCard* pvCard = new CFrmUservCard(GLOBAL_USER->GetUserInfoRoster(bareJid));
    pvCard->show();
}

void CFrmUserList::slotRosterAddReceived(const QString &szId, const CClient::SUBSCRIBE_TYPE &type)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::subscriptionReceived:%s", qPrintable(szId));
    m_frmAddRoster.Init( GetGroupsName(), szId, true);
    m_frmAddRoster.show();
    m_frmAddRoster.activateWindow();
}

QStandardItem*  CFrmUserList::ItemInsertGroup(QString szGroup)
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

int CFrmUserList::ItemUpdateGroup(QList<QStandardItem *> &lstItems, QSet<QString> groups)
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
            lstGroup = ItemInsertGroup(szGroup);
        }
        else
            lstGroup = it.value();

        lstGroup->appendRow(lstItems);
    }

    return 0;
}

int CFrmUserList::ItemInsertRoster(const QString& szId)
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
    pItem->setData(roster->GetId(), USERLIST_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
    //改变item背景颜色  
    pItem->setData(CGlobal::Instance()->GetRosterStatusColor(roster->GetStatus()), Qt::BackgroundRole);
    pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(roster->GetStatus())));
    pItem->setEditable(false);
    QString szText;
    
    szText = roster->GetShowName()
        #ifdef DEBUG
            + "[" + CGlobal::Instance()->GetRosterStatusText(roster->GetStatus()) + "]"
            +  roster->GetSubscriptionTypeStr(roster->GetSubScriptionType())
        #endif
            ;
    
    pItem->setData(szText, Qt::DisplayRole); //改变item文本,或者直接用 pItem->setText(szText);  
    pItem->setToolTip(szText);

    //改变item图标  
    pItem->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(roster->GetStatus())), Qt::DecorationRole);

    //消息条目  
    QStandardItem* pMessageCountItem = new QStandardItem("");
    pMessageCountItem->setData(roster->GetId(), USERLIST_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;

    ItemUpdateGroup(lstItems, roster->GetGroups());
    return nRet;
}

int CFrmUserList::ItemUpdateRoster(const QString &szId)
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
    {
        int nRet = 0;
        nRet = ItemInsertRoster(szId);
        if(nRet)
            return nRet;
        lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                       USERLIST_ITEM_ROLE_JID, 
                                                       roster->GetId(), 
                                                       -1,
                                                       Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    }

    QModelIndex index;
    foreach(index, lstIndexs)
    {
        LOG_MODEL_DEBUG("FrmUserList", "index:row:%d;column:%d", index.row(), index.column());
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(!pItem) continue;
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ROSTER)
        {
            //pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            //pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
            //改变item背景颜色  
            pItem->setData(CGlobal::Instance()->GetRosterStatusColor(roster->GetStatus()), Qt::BackgroundRole);
            pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(roster->GetStatus())));
            QString szText;
            
            szText = roster->GetShowName()
        #ifdef DEBUG
                    + "[" + CGlobal::Instance()->GetRosterStatusText(roster->GetStatus()) + "]"
                    +  roster->GetSubscriptionTypeStr(roster->GetSubScriptionType())
        #endif
                    ;
            
            pItem->setData(szText, Qt::DisplayRole); //改变item文本  
            pItem->setToolTip(szText);
            //改变item图标  
            pItem->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(roster->GetStatus())), Qt::DecorationRole);
        }

        if(NULL == pItem || NULL == pItem->parent()) continue;
        QStandardItem* pItemUnReadMessageCount = pItem->parent()->child(index.row(), index.column() + 1);
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

int CFrmUserList::ItemRemoveRoster(const QString &szId)
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

//得到好友列表  
void CFrmUserList::slotLoadRosterFromStorage()
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList:: Roster received");

    int type = OPERATE_TYPE_INSERT_ROSTER;
    GLOBAL_USER->ProcessRoster(this, &type);
}

void CFrmUserList::slotUpdateRosterUserInfo(const QString &szId, QSharedPointer<CUserInfo> userInfo)
{
    ItemUpdateRoster(szId);
}

void CFrmUserList::slotRemoveRosterUserInfo(const QString &szId)
{
    ItemRemoveRoster(szId);
}

//好友出席状态改变  
void CFrmUserList::SlotChangedStatus(const QString &szId)
{
    //更新列表控件状态  
    ItemUpdateRoster(szId);
    return;
}

void CFrmUserList::clicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::clicked, row:%d; column:%d",
           index.row(), index.column());

#ifdef MOBILE
    if(m_UserList.isExpanded(index))
       m_UserList.collapse(index);
    else
       m_UserList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    if(!GetCurrentRoster().isEmpty())
    {
        //是用户结点，打开消息对话框  
        MANAGE_MESSAGE_DIALOG->ShowDialog(GetCurrentRoster()); 
    }
#endif
}

void CFrmUserList::doubleClicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

#ifndef MOBILE
    if(!m_UserList.isExpanded(index))
       m_UserList.collapse(index);
    else
       m_UserList.expand(index);

    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    if(!GetCurrentRoster().isEmpty())
    {
        //是用户结点，打开消息对话框  
        MANAGE_MESSAGE_DIALOG->ShowDialog(GetCurrentRoster());
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

void CFrmUserList::slotItemChanged(QStandardItem *item)
{
    Q_UNUSED(item);
    LOG_MODEL_DEBUG("CFrmUserList", "CFrmUserList::slotItemChanged");
}

void CFrmUserList::slotEntered(const QModelIndex &index)
{
    Q_UNUSED(index);
    LOG_MODEL_DEBUG("CFrmUserList", "CFrmUserList::slotEntered");
}

void CFrmUserList::slotRefresh()
{
    int type = OPERATE_TYPE_UPDATE_ROSTER;
    GLOBAL_USER->ProcessRoster(this, &type);
}
