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
#include "Tool.h"

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(logUserList, "UserList")

#ifdef WIN32
#undef GetMessage
#endif

CFrmUserList::CFrmUserList(QWidget *parent) :
    QWidget(parent),
    COperateRoster(),
    m_UserList(this),
    ui(new Ui::CFrmUserList)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(&m_UserList);

    InitMenu();

    m_pModel = new QStandardItemModel(this);
    if(m_pModel)
    {
        //增加头，只有增加了这个后，下面才会显示内容  
        m_pModel->setHorizontalHeaderLabels(QStringList() << tr("User name or group")<< tr("Information"));
    }

    m_UserList.setModel(m_pModel);
    m_UserList.show();

    LoadGroupNodeStateFromStorage();

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

    SaveGroupNodeStateToStorage();

    delete ui;

    if(m_pModel)
        delete m_pModel;
}

QString CFrmUserList::GetNodeStateFile()
{
    QString szId;
    if(!GLOBAL_USER.isNull()
            && !USER_INFO_LOCALE.isNull())
    {
        szId = USER_INFO_LOCALE->GetInfo()->GetId();
    }
    if(szId.isEmpty())
    {
        return QString();
    }
    return CGlobalDir::Instance()->GetDirUserData(szId)
            + QDir::separator() + "UserListGroupNodeState.dat";
}

int CFrmUserList::LoadGroupNodeStateFromStorage()
{
    QString szFile = GetNodeStateFile();
    QFile in(szFile);
    if(!in.open(QFile::ReadOnly))
    {
        qWarning(logUserList, "Don't open file:%s", szFile.toStdString().c_str());
        return -1;
    }

    QDataStream s(&in);
    try{
        //版本号  
        int nVersion = 0;
        s >> nVersion;
        int nRowCount = 0;
        s >> nRowCount;
        if(0 == nRowCount)
        {
            ItemInsertGroup(tr("My friends"));
        }
        else
            while(nRowCount--)
            {
                //本地用户信息  
                qDebug(logUserList, "Version:%d", nVersion);
                QString szGroup;
                s >> szGroup;
                ItemInsertGroup(szGroup);
                bool bState;
                s >> bState;
                QModelIndexList lstIndexs = m_pModel->match(
                            m_pModel->index(0, 0),
                            USERLIST_ITEM_ROLE_PROPERTIES, 
                            PROPERTIES_GROUP, 
                            -1,
                            Qt::MatchStartsWith
                            | Qt::MatchWrap
                            | Qt::MatchRecursive
                            | Qt::MatchCaseSensitive);
                QModelIndex index;
                foreach(index, lstIndexs)
                {
                    QStandardItem* pItem = m_pModel->itemFromIndex(index);
                    if(!pItem) continue;
                    if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES)
                            != PROPERTIES_GROUP)
                        continue;
                    if(pItem->text() != szGroup)
                        continue;
                    if(bState)
                        m_UserList.expand(index);
                    else
                        m_UserList.collapse(index);
                }
            }
    }
    catch(...)
    {
        qCritical(logUserList) <<
               "CFrmUserList::LoadGroupNodeStateFromStorage exception";
        return -2;
    }

    in.close();    

    return 0;
}

int CFrmUserList::SaveGroupNodeStateToStorage()
{
    QString szFile = GetNodeStateFile();
    QFile out(szFile);
    if(!out.open(QFile::WriteOnly))
    {
        qWarning(logUserList) << "Don't open file:" << szFile;
        return -3;
    }

    try
    {
        QDataStream s(&out);
        //版本号  
        int nVersion = 1;
        s << nVersion;

        int nRowCount = m_pModel->rowCount();
        s << nRowCount;
        for(int i = 0; i < nRowCount; i++)
        {
            QModelIndex index;
            index = m_pModel->index(i, 0);
            QStandardItem* item = m_pModel->itemFromIndex(index);
            if(item->data(USERLIST_ITEM_ROLE_PROPERTIES) != PROPERTIES_GROUP)
                continue;
            qDebug(logUserList) << "text:" << item->text();
            s << item->text()
              << m_UserList.isExpanded(index);
        }
    }
    catch(...)
    {
        qCritical(logUserList) <<
                "CFrmUserList::SaveGroupNodeStateToStorage exception";
        return -4;
    }

    out.close();
    return 0;
}

int CFrmUserList::ProcessRoster(QSharedPointer<CUserInfo> roster, void *para)
{
    Q_UNUSED(para);
    int nRet = 0;
    nRet = ItemUpdateRoster(roster->GetId());
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

    m_Menu.addAction(ui->actionMove_roster);
    check = connect(ui->actionMove_roster, SIGNAL(triggered()),
                    SLOT(slotMoveRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionRemoveRoster_R);
    check = connect(ui->actionRemoveRoster_R, SIGNAL(triggered()),
                    SLOT(slotRemoveRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionAgreeAddRoster);
    check = connect(ui->actionAgreeAddRoster, SIGNAL(triggered()),
                    SLOT(slotAgreeAddRoster()));
    Q_ASSERT(check);

    m_Menu.addAction(ui->actionRefresh_roster_list);
    check = connect(ui->actionRefresh_roster_list, SIGNAL(triggered()),
                    SLOT(slotRefreshRosterList()));
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
    EnableAction(ui->actionMove_roster, bEnable);
    EnableAction(ui->actionRefresh_roster_list, bEnable);
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
    //更新好友列表  
    EnableAction(ui->actionRefresh_roster_list);

    //判断是在组上还是在好友上  
    QString bareJid = GetCurrentRoster();
    if(bareJid.isEmpty())
    {
        //TODO:新建组  

        //判断子节点是否为空  
        QModelIndex index = m_UserList.currentIndex();
        if(!m_pModel->hasChildren(index))
            EnableAction(ui->actionRemove_Group);
    }
    else
    {
        QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(bareJid);
        if(user.isNull())
        {
            qCritical(logUserList) << "Don't roster:" << bareJid;
            return;
        }
        
        QSharedPointer<CUserInfo> info = user->GetInfo();
        if(info.isNull())
        {
            qCritical(logUserList) << "Don't roster:" << bareJid;
            return;
        }

        //增加订阅  
        if(CUserInfo::Both != info->GetSubScriptionType())
            EnableAction(ui->actionAgreeAddRoster);
        
        //显示重命名菜单  
        EnableAction(ui->actionRename);
        //如果是好友上,显示删除好友  
        EnableAction(ui->actionRemoveRoster_R);
        //查看好友信息  
        EnableAction(ui->actionInformation_I);
        //移动到组  
        EnableAction(ui->actionMove_roster);
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

void CFrmUserList::slotMoveRoster()
{
    QString szName;
    QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(GetCurrentRoster());
    if(user.isNull())
        return;
    szName = user->GetInfo()->GetShowName();
    QStringList items;
    int nIndex = 0, i = 0;
    QMap<QString, QStandardItem*>::iterator it;
    for(it = m_Groups.begin(); it != m_Groups.end(); it++)
    {
        items << it.key();
        if(user->GetInfo()->GetGroups().contains(it.key()))
            nIndex = i;
        i++;
    }

    bool ok;
    QString item = QInputDialog::getItem(this,
                                         tr("Move roster").arg(szName),
                                         tr("Move roster %1 to group:").arg(szName),
                                         items,
                                         nIndex,
                                         true,
                                         &ok);
    if (ok && !item.isEmpty())
    {
        QSet<QString> groups;
        groups << item;
        GET_CLIENT->RosterRemove(user->GetInfo()->GetId());
        GET_CLIENT->RosterAdd(user->GetInfo()->GetId(),
                              CClient::SUBSCRIBE_ACCEPT,
                              user->GetInfo()->GetNick(),
                              groups);
    }
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

void CFrmUserList::slotRefreshRosterList()
{
    GET_CLIENT->RefreshRosterList();
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
    if(!GETMANAGER->GetCall())
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
    if(!GETMANAGER->GetCall())
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
    qDebug(logUserList) <<  "CFrmUserList::subscriptionReceived:" << szId;
    Q_UNUSED(type);
    CDlgAddRoster dlgAddRoster;
    dlgAddRoster.Init( GetGroupsName(), szId, true);
    dlgAddRoster.exec();
}

QStandardItem* CFrmUserList::ItemInsertGroup(QString szGroup)
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
        qCritical(logUserList) <<  "Dn't the roster:" << szId;
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
        #endif
            +  info->GetSubscriptionTypeStr(info->GetSubScriptionType())
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
        qCritical(logUserList) << "Dn't the roster:" << szId;
        return -1;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();

    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                info->GetId(), 
                                                -1,
                                               Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchCaseSensitive);
    if(lstIndexs.isEmpty())
    {
        int nRet = 0;
        nRet = ItemInsertRoster(szId);
        if(nRet)
        {
            qCritical(logUserList, "Insert roster %s fail", qPrintable(szId));
            return nRet;
        }
        qDebug(logUserList, "Insert roster %s", qPrintable(szId));
        lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                    USERLIST_ITEM_ROLE_JID, 
                                    info->GetId(), 
                                    -1,
                                    Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchCaseSensitive);
    }

    QModelIndex index;
    foreach(index, lstIndexs)
    {
        qDebug(logUserList, "index:row:%d;column:%d;id:%s", index.row(), index.column(), qPrintable(info->GetId()));
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(!pItem) continue;
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ROSTER)
        {
            //pItem->setData(roster->GetBareJid(), USERLIST_ITEM_ROLE_JID);
            //pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
            //改变item背景颜色  
            //pItem->setData(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus()), Qt::BackgroundRole);
            //pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus())));
//            if(CGlobal::Instance()->GetRosterShowType() == CGlobal::E_ROSTER_SHOW_NICK)
//                pItem->setEditable(true);
//            else
//                pItem->setEditable(false);//禁止双击编辑  
            QString szText;
            szText = info->GetShowName()
        #ifdef DEBUG
                    + "[" + CGlobal::Instance()->GetRosterStatusText(info->GetStatus()) + "]"
        #endif
                    +  info->GetSubscriptionTypeStr(info->GetSubScriptionType())
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
                                                Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchCaseSensitive);
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
    qDebug(logUserList, "CFrmUserList:: Roster received");

    GLOBAL_USER->ProcessRoster(this);
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
    qDebug(logUserList, "CFrmUserList::clicked, row:%d; column:%d",
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
    qDebug(logUserList, "CFrmUserList::doubleClicked, row:%d; column:%d",
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
    Q_UNUSED(e);
    /*qDebug(logUserList, "CFrmUserList::resizeEvent:e.size:%d;genmetry.size:%d;userlist.framewidth:%d;width:%d",
                    e->size().width(),
                    geometry().size().width(),
                    m_UserList.frameGeometry().width(),
                    m_UserList.width());*/
    //m_UserList.resize(this->geometry().size());

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
    qDebug(logUserList) << "CFrmUserList::slotItemChanged";
}

void CFrmUserList::slotEntered(const QModelIndex &index)
{
    Q_UNUSED(index);
    qDebug(logUserList) << "CFrmUserList::slotEntered";
}

void CFrmUserList::slotRefresh()
{
    GLOBAL_USER->ProcessRoster(this);
}
