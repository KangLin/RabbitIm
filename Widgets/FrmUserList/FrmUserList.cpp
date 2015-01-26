#include "FrmUserList.h"
#include "ui_FrmUserList.h"
#include "../../MainWindow.h"
#include "../DlgUservCard/DlgUservCard.h"
#include "FileTransfer/ManageFileTransfer.h"
#include "Client/Client.h"
#include "Global/Global.h"
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <fstream>
#include <memory> 

#ifdef WIN32
#undef GetMessage
#endif

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

    check = connect(GET_CLIENT.data(), SIGNAL(sigUpdateRosterUserInfo(QString,QSharedPointer<CUser>)),
                    SLOT(slotUpdateRosterUserInfo(QString,QSharedPointer<CUser>)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigRosterAddReceived(const QString&, const CClient::SUBSCRIBE_TYPE &)),
                    SLOT(slotRosterAddReceived(const QString&, const CClient::SUBSCRIBE_TYPE &)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigRemoveRosterUserInfo(QString)),
                    SLOT(slotRemoveRosterUserInfo(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageUpdate(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageClean(QString)),
                    SLOT(slotMessageUpdate(QString)));
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
    m_Menu.addAction(ui->actionSendMessage);
    check = connect(ui->actionSendMessage, SIGNAL(triggered()),
                    SLOT(slotSendMessage()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionSendFile);
    check = connect(ui->actionSendFile, SIGNAL(triggered()),
                    SLOT(slotSendFile()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionVideo);
    check = connect(ui->actionVideo, SIGNAL(triggered()),
                    SLOT(slotCallVideo()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionAudio);
    check = connect(ui->actionAudio, SIGNAL(triggered()),
                    SLOT(slotCallAudio()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionAllowMonitor);
    ui->actionAllowMonitor->setCheckable(true);
    check = connect(ui->actionAllowMonitor, SIGNAL(triggered()),
                    SLOT(slotMonitor()));
    Q_ASSERT(check);
    //TODO:1.新增菜单,总共三步  
    
    m_Menu.addSeparator();

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

    m_Menu.addAction(ui->actionRemove_Group);
    check = connect(ui->actionRemove_Group, SIGNAL(triggered()),
                    SLOT(slotRemoveGroup()));
    Q_ASSERT(check);

    return 0;
}

int CFrmUserList::EnableAllActioins(bool bEnable)
{
    EnableAction(ui->actionAddRoster_A, bEnable);
    EnableAction(ui->actionAgreeAddRoster, bEnable);
    EnableAction(ui->actionRemoveRoster_R, bEnable);
    EnableAction(ui->actionInformation_I, bEnable);
    EnableAction(ui->actionRemove_Group, bEnable);
    EnableAction(ui->actionRename, bEnable);
    EnableAction(ui->actionSendMessage, bEnable);
    EnableAction(ui->actionSendFile, bEnable);
    EnableAction(ui->actionVideo, bEnable);
    EnableAction(ui->actionAudio, bEnable);
    EnableAction(ui->actionAllowMonitor, bEnable);
    //TODO:2.新增菜单  
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

        //TODO:判断子节点是否为空  
        QModelIndex index = m_UserList.currentIndex();
        if(!m_pModel->hasChildren(index))
            EnableAction(ui->actionRemove_Group);
    }
    else
    {
        QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(bareJid);
        if(user.isNull())
        {
            LOG_MODEL_ERROR("FrmUserList", "Don't roster:%s", bareJid.toStdString().c_str());
            return;
        }
        
        QSharedPointer<CUserInfo> info = user->GetInfo();
        if(info.isNull())
        {
            LOG_MODEL_ERROR("FrmUserList", "Don't roster:%s", bareJid.toStdString().c_str());
            return;
        }

        //增加订阅  
        if(CUserInfo::From == info->GetSubScriptionType())
            EnableAction(ui->actionAgreeAddRoster);
        
        //显示重命名菜单  
        EnableAction(ui->actionRename);
        //如果是好友上,显示删除好友  
        EnableAction(ui->actionRemoveRoster_R);

        //查看好友信息  
        EnableAction(ui->actionInformation_I);
        //TODO: 移动到组  

        EnableAction(ui->actionSendMessage);
        EnableAction(ui->actionSendFile);
        EnableAction(ui->actionVideo);
        EnableAction(ui->actionAudio);
        if(CGlobal::Instance()->GetIsMonitor())
         {
            ui->actionAllowMonitor->setChecked(info->GetIsMonitor());
            EnableAction(ui->actionAllowMonitor);
        }
        //TODO:3新增菜单  
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
    CDlgAddRoster dlgAddRoster;
    QSet<QString> groups;
    groups = GetGroupsName();
    QString szId = GetCurrentRoster();
    if(!szId.isEmpty() && CUserInfo::None == GLOBAL_USER->GetUserInfoRoster(szId)->GetInfo()->GetSubScriptionType())
        dlgAddRoster.Init(groups, szId);
    else
        dlgAddRoster.Init(groups);
    dlgAddRoster.exec();
}

void CFrmUserList::slotRenameRoster()
{
    QString szName;
    if(GLOBAL_USER->GetUserInfoRoster(GetCurrentRoster()).isNull())
        return;
    szName = GLOBAL_USER->GetUserInfoRoster(GetCurrentRoster())->GetInfo()->GetShowName();
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
    CDlgUservCard pvCard(GLOBAL_USER->GetUserInfoRoster(bareJid)->GetInfo());
    pvCard.exec();
}

void CFrmUserList::slotRemoveGroup()
{
    m_pModel->removeRow(m_UserList.currentIndex().row());
}

void CFrmUserList::slotSendMessage()
{
    if(!GetCurrentRoster().isEmpty())
    {
        //是用户结点，打开消息对话框  
        MANAGE_MESSAGE_DIALOG->ShowDialog(GetCurrentRoster()); 
    }
}

void CFrmUserList::slotSendFile()
{
    QString szId = GetCurrentRoster();
    if(szId.isEmpty())
        return;

    QString szFile = CTool::FileDialog(this, QString(), QString(), tr("Open File"));
    if(szFile.isEmpty())
        return;
    QSharedPointer<CManageFileTransfer> file = CGlobal::Instance()->GetManager()->GetFileTransfer();
    file->SendFile(szId, szFile);
    //弹出消息对话框  
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId); 
}

void CFrmUserList::slotCallVideo()
{
    QString szId = GetCurrentRoster();
    if(szId.isEmpty())
        return;
    GETMANAGER->GetCall()->Call(szId, true);
    //弹出消息对话框  
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId); 
}

void CFrmUserList::slotCallAudio()
{
    QString szId = GetCurrentRoster();
    if(szId.isEmpty())
        return;
    GETMANAGER->GetCall()->Call(szId);
    //弹出消息对话框  
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId); 
}

void CFrmUserList::slotMonitor()
{
    QString szId = GetCurrentRoster();
    if(szId.isEmpty())
        return;
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
        return;
    roster->GetInfo()->SetMonitor(!roster->GetInfo()->GetIsMonitor());
}

void CFrmUserList::slotRosterAddReceived(const QString &szId, const CClient::SUBSCRIBE_TYPE &type)
{
    LOG_MODEL_DEBUG("Roster", "CFrmUserList::subscriptionReceived:%s", qPrintable(szId));
    Q_UNUSED(type);
    CDlgAddRoster dlgAddRoster;
    dlgAddRoster.Init( GetGroupsName(), szId, true);
    dlgAddRoster.exec();
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

int CFrmUserList::ItemUpdateGroup(QSharedPointer<CUserInfo> info)
{
    QSet<QString> groups = info->GetGroups();
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

        QList<QStandardItem *> lstItems = NewItemRoster(info);
        if(lstItems.isEmpty())
            continue;
        lstGroup->appendRow(lstItems);
    }

    return 0;
}

int CFrmUserList::ItemInsertRoster(const QString& szId)
{
    int nRet = 0;
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster:%s", qPrintable(szId));
        return -1;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();
    if(info.isNull())
        return -2;

    ItemUpdateGroup(info);
    return nRet;
}

QList<QStandardItem *> CFrmUserList::NewItemRoster(QSharedPointer<CUserInfo> info)
{
    //呢称条目  
    QStandardItem* pItem = new QStandardItem(info->GetShowName() 
                                             + info->GetSubscriptionTypeStr(info->GetSubScriptionType()));
    pItem->setData(info->GetId(), USERLIST_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
    //改变item背景颜色  
    //pItem->setData(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus()), Qt::BackgroundRole);
    // pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus())));
    pItem->setEditable(false);
    QString szText;
    szText = info->GetShowName()
        #ifdef DEBUG
            + "[" + CGlobal::Instance()->GetRosterStatusText(info->GetStatus()) + "]"
            +  info->GetSubscriptionTypeStr(info->GetSubScriptionType())
        #endif
            ;
    pItem->setData(szText, Qt::DisplayRole); //改变item文本,或者直接用 pItem->setText(szText);  
#ifdef DEBUG
    pItem->setToolTip(info->GetId());
#endif 

    //改变item图标  
    pItem->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(info->GetStatus())), Qt::DecorationRole);

    //消息条目  
    QStandardItem* pMessageCountItem = new QStandardItem("");
    pMessageCountItem->setData(info->GetId(), USERLIST_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;
    return lstItems;
}

int CFrmUserList::ItemUpdateRoster(const QString &szId)
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster:%s", qPrintable(szId));
        return -1;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();

    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                info->GetId(), 
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
                                                       info->GetId(), 
                                                       -1,
                                                       Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    }

    QModelIndex index;
    foreach(index, lstIndexs)
    {
        LOG_MODEL_DEBUG("FrmUserList", "index:row:%d;column:%d;id:%s", index.row(), index.column(), qPrintable(info->GetId()));
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(!pItem) continue;
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ROSTER)
        {
            //pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            //pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
            //改变item背景颜色  
            //pItem->setData(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus()), Qt::BackgroundRole);
            //pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus())));
            QString szText;
            szText = info->GetShowName()
        #ifdef DEBUG
                    + "[" + CGlobal::Instance()->GetRosterStatusText(info->GetStatus()) + "]"
                    +  info->GetSubscriptionTypeStr(info->GetSubScriptionType())
        #endif
                    ;
            pItem->setData(szText, Qt::DisplayRole); //改变item文本  
#ifdef DEBUG
            pItem->setToolTip(info->GetId());
#endif 
            //改变item图标  
            QPixmap pmp;
            MainWindow::ComposeAvatarStatus(info, pmp);
            pItem->setData(pmp.scaled(RABBITIM_ICON_SIZE, RABBITIM_ICON_SIZE), Qt::DecorationRole);
        }

        if(NULL == pItem || NULL == pItem->parent()) continue;
        QStandardItem* pItemUnReadMessageCount = pItem->parent()->child(index.row(), index.column() + 1);
        if(pItemUnReadMessageCount->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_UNREAD_MESSAGE_COUNT)
        {
            //设置未读消息数  
            int nCount = roster->GetMessage()->GetUnReadCount();
            if(nCount)
                pItemUnReadMessageCount->setText(QString::number(nCount));
            else
                pItemUnReadMessageCount->setText(QString(""));
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

void CFrmUserList::slotUpdateRosterUserInfo(const QString &szId, QSharedPointer<CUser> userInfo)
{
    ItemUpdateRoster(szId);
}

void CFrmUserList::slotRemoveRosterUserInfo(const QString &szId)
{
    ItemRemoveRoster(szId);
}

void CFrmUserList::slotMessageUpdate(const QString &szId)
{
    ItemUpdateRoster(szId);
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

    slotSendMessage();
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

    slotSendMessage();
#endif
}

void CFrmUserList::resizeEvent(QResizeEvent *e)
{
    /*LOG_MODEL_DEBUG("CFrmUserList", "CFrmUserList::resizeEvent:e.size:%d;genmetry.size:%d;userlist.framewidth:%d;width:%d",
                    e->size().width(),
                    geometry().size().width(),
                    m_UserList.frameGeometry().width(),
                    m_UserList.width());*/
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
