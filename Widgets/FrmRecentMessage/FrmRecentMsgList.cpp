#include "FrmRecentMsgList.h"
#include "ui_FrmRecentMsgList.h"
#include "Global/Global.h"
#include "MainWindow.h"

#include <QSettings>

#ifdef WIN32
#undef GetMessage
#endif

CFrmRecentMsgList::CFrmRecentMsgList(QWidget *parent) :
    QWidget(parent),
    m_MsgList(this),
    ui(new Ui::CFrmRecentMsgList)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(&m_MsgList);

    m_pModel = new QStandardItemModel(this);//这里不会产生内存泄漏，控件在romve操作时会自己释放内存。  
    if(m_pModel)
    {
        //增加头，只有增加了这个后，下面才会显示内容  
        m_pModel->setHorizontalHeaderLabels(QStringList() << tr("Recent Message")<< tr("Information"));
    }

    m_MsgList.setModel(m_pModel);
    m_MsgList.show(); 

    bool check = connect(&m_MsgList, SIGNAL(clicked(QModelIndex)),
                         SLOT(clicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_MsgList, SIGNAL(doubleClicked(QModelIndex)),
                         SLOT(doubleClicked(QModelIndex)));
    Q_ASSERT(check);

    //连接好友消息  
    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageUpdate(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageClean(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigChangedStatus(const QString&)),
                    SLOT(SlotChangedStatus(const QString&)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigRemoveRosterUserInfo(QString)),
                    SLOT(slotRemove(QString)));
    Q_ASSERT(check);
    
    check = connect(GETMANAGER->GetManageGroupChat().data(), SIGNAL(sigLeave(QString)),
                    SLOT(slotRemove(QString)));
    Q_ASSERT(check);

    //连接组消息  
    check = connect(GETMANAGER->GetManageGroupChat().data(), SIGNAL(sigUpdateMessage(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);
    
    check = connect(GETMANAGER->GetManageGroupChat().data(), SIGNAL(sigMessageClean(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigRefresh()),
                    SLOT(slotRefresh()));
    Q_ASSERT(check);

    InitMenu();
    LoadFromStorage();
}

CFrmRecentMsgList::~CFrmRecentMsgList()
{
    GET_CLIENT->disconnect(this);
    GETMANAGER->GetManageGroupChat()->disconnect(this);
    GETMANAGER->GetRecentMessage()->disconnect(this);
    delete ui;
    delete m_pModel;
}

void CFrmRecentMsgList::LoadFromStorage()
{
    QList<QString> lstMembers;
    lstMembers = GETMANAGER->GetRecentMessage()->GetAllMembers();
    int nRow = 0;
    foreach (QString szId, lstMembers) 
    {
        //是好友消息  
        InsertItem(szId, nRow++);
    }
}

int CFrmRecentMsgList::InsertItem(const QString &szId, int nRow)
{
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                szId,
                                                1,
                                                Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    if(!lstIndexs.isEmpty())
    {
        return UpdateItem(szId);
    }
    
    int nRet = 0;
    QString szName; 
    int nCount = 0;
    QIcon icon;
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    if(!roster.isNull())//是好友消息  
    {
        szName = roster->GetInfo()->GetShowName() 
                + roster->GetInfo()->GetSubscriptionTypeStr(roster->GetInfo()->GetSubScriptionType());
        nCount = roster->GetMessage()->GetUnReadCount();
        QPixmap pmp;
        MainWindow::ComposeAvatarStatus(roster->GetInfo(), pmp);
        icon = QIcon(pmp);
    }//是组消息  
    else if(!gc.isNull())
    {
        szName = gc->ShowName();
        nCount = gc->GetMessage()->GetUnReadCount();
        icon = gc->Icon();
    }
    else
    {
        LOG_MODEL_DEBUG("CFrmRecentMsgList", "CFrmRecentMsgList::InsertItem: don't exist:%s", qPrintable(szId));
        return -1;
    }

    //呢称条目  
    QStandardItem* pItem = new QStandardItem(szName);
    pItem->setData(szId, USERLIST_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ID, USERLIST_ITEM_ROLE_PROPERTIES);
    pItem->setEditable(false);
    //改变item图标  
    pItem->setData(icon, Qt::DecorationRole);    
#ifdef DEBUG
            pItem->setToolTip(szId);
#endif 

    //消息条目  
    QStandardItem* pMessageCountItem = new QStandardItem();
    if(nCount)
        pMessageCountItem->setText(QString::number(nCount));
    else
        pMessageCountItem->setText("");
    pMessageCountItem->setData(szId, USERLIST_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;

    m_pModel->insertRow(nRow, lstItems);
    return nRet;
}

int CFrmRecentMsgList::RemoveItem(const QString &szId)
{
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                szId,
                                                1,
                                                Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    if(!lstIndexs.isEmpty())
    {
        QModelIndex index;
        foreach(index, lstIndexs)
        {
            LOG_MODEL_DEBUG("CFrmRecentMsgList", "index:row:%d;column:%d;id:%s", index.row(), index.column(), qPrintable(szId));
            m_pModel->removeRow(index.row());
        }
    }
    return 0;
}

int CFrmRecentMsgList::UpdateItem(const QString &szId)
{
    QString szShowText;
    int nCount = 0;
    //改变item图标  
    QPixmap pmp;
    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(!roster.isNull())
    {
        szShowText = roster->GetInfo()->GetShowName() 
                + roster->GetInfo()->GetSubscriptionTypeStr(roster->GetInfo()->GetSubScriptionType());
        nCount = roster->GetMessage()->GetUnReadCount();

        MainWindow::ComposeAvatarStatus(roster->GetInfo(), pmp);
    }
    else if(!gc.isNull())
    {
        szShowText = gc->ShowName();
        nCount = gc->GetMessage()->GetUnReadCount();
        pmp = gc->Icon().pixmap(RABBITIM_ICON_SIZE, RABBITIM_ICON_SIZE);
    }
    else
    {
        LOG_MODEL_WARNING("CFrmRecentMsgList", "CFrmRecentMsgList::UpdateItem: don't exist:%s", qPrintable(szId));
        return -1;
    }

    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                szId, 
                                                1,
                                                Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    if(lstIndexs.isEmpty())
    {
       LOG_MODEL_ERROR("CFrmRecentMsgList", "Dn't the roster item:%s", qPrintable(szId));
       return -1;
    }

    QModelIndex index;
    foreach(index, lstIndexs)
    {
        //LOG_MODEL_DEBUG("FrmUserList", "index:row:%d;column:%d;id:%s", index.row(), index.column(), qPrintable(szId));
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(!pItem) continue;
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ID)
        {
            pItem->setData(szShowText, Qt::DisplayRole); //改变item文本  
#ifdef DEBUG
            pItem->setToolTip(szId);
#endif 
            //改变item图标  
            pItem->setData(pmp.scaled(RABBITIM_ICON_SIZE, RABBITIM_ICON_SIZE), Qt::DecorationRole);
        }

        if(NULL == pItem || NULL == pItem->parent()) continue;
        QStandardItem* pItemUnReadMessageCount = pItem->parent()->child(index.row(), index.column() + 1);
        if(pItemUnReadMessageCount->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_UNREAD_MESSAGE_COUNT)
        {
            //设置未读消息数  
            if(nCount)
                pItemUnReadMessageCount->setText(QString::number(nCount));
            else
                pItemUnReadMessageCount->setText(QString(""));
            pItemUnReadMessageCount->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
        }
    }
    return 0;
}

void CFrmRecentMsgList::slotMessageUpdate(const QString &szId)
{
    int nCount = 0;
    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(!roster.isNull())
    {
        nCount = roster->GetMessage()->GetUnReadCount();
    }
    else if(!gc.isNull())
    {
        nCount = gc->GetMessage()->GetUnReadCount();
    }
    else
    {
        LOG_MODEL_DEBUG("CFrmRecentMsgList", "CFrmRecentMsgList::slotMessageUpdate is exist:%s", qPrintable(szId));
        return;
    }

    RemoveItem(szId);

    //判断是否到达列表最大值  
    QSharedPointer<CManageRecentMessage> recentMessage = CGlobal::Instance()->GetManager()->GetRecentMessage();
    int nRowCount = m_pModel->rowCount();
    if(nRowCount >= recentMessage->GetMaxCount())
    {
        m_pModel->removeRow(nRowCount - 1);
    }

    //数据层在界面之前初始化,见CClientXmpp::slotClientConnected()  
    if(nCount > 0)
        InsertItem(szId, 0);
    else
    {
        int row = recentMessage->GetUnreadCount();
        InsertItem(szId, row);
    }
}

void CFrmRecentMsgList::SlotChangedStatus(const QString &szId)
{
    UpdateItem(szId);
    return;
}

void CFrmRecentMsgList::resizeEvent(QResizeEvent *e)
{
    LOG_MODEL_DEBUG("CFrmRecentMsgList", "CFrmUserList::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());
    //m_MsgList.resize(this->geometry().size());

    //调整列的宽度  
    int nWidth = m_MsgList.geometry().width() * 4/ 5;
    m_MsgList.setColumnWidth(0, nWidth);
    m_MsgList.setColumnWidth(1, m_MsgList.geometry().width() - nWidth - 5);
}

void CFrmRecentMsgList::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CFrmRecentMsgList::clicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("CFrmRecentMsgList", "CFrmRecentMsgList::Clicked, row:%d; column:%d",
           index.row(), index.column());
#ifdef ANDROID
    const QAbstractItemModel *m = index.model();
    if(!m)return;

    QVariant v = m->data(index, USERLIST_ITEM_ROLE_JID);
    QString szId = v.value<QString>();
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId);
#endif
}

void CFrmRecentMsgList::doubleClicked(const QModelIndex &index)
{
    LOG_MODEL_DEBUG("CFrmRecentMsgList", "CFrmRecentMsgList::doubleClicked, row:%d; column:%d",
           index.row(), index.column());

#ifndef ANDROID
    const QAbstractItemModel *m = index.model();
    if(!m)return;

    QVariant v = m->data(index, USERLIST_ITEM_ROLE_JID);
    QString szId = v.value<QString>();
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId);
#endif
}

void CFrmRecentMsgList::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(m_pModel->rowCount() <= 0)
        return;
    m_Menu.exec(QCursor::pos());
}

void CFrmRecentMsgList::slotRefresh()
{
    int nIndex = 0;
    for(nIndex; nIndex < m_pModel->rowCount(); nIndex++)
    {
        QStandardItem* pItem = m_pModel->itemFromIndex(m_pModel->index(nIndex, 0));
        QString szId = pItem->data(USERLIST_ITEM_ROLE_JID).value<QString>();
        UpdateItem(szId);
    }
}

int CFrmRecentMsgList::InitMenu()
{
    bool check = false;
    check = connect(&m_MsgList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigMenuInitOperator(QMenu*)),
                    SLOT(slotAddToMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigMenuRemoveOperator(QMenu*)),
                    SLOT(slotDeleteFromMainMenu(QMenu*)));
    Q_ASSERT(check);

    m_pMenuAction = NULL;
    m_Menu.addAction(ui->actionOpenMessage);
    check = connect(ui->actionOpenMessage, SIGNAL(triggered()),
                    SLOT(slotOpenMessage()));
    Q_ASSERT(check);
    
    m_Menu.addAction(ui->actionRemoveMessage);
    check = connect(ui->actionRemoveMessage, SIGNAL(triggered()),
                    SLOT(slotRemoveMessage()));
    Q_ASSERT(check);
    return 0;
}

void CFrmRecentMsgList::slotAddToMainMenu(QMenu* pMenu)
{
    m_pMenuAction = pMenu->addMenu(&m_Menu);
    bool check = connect(pMenu, SIGNAL(aboutToShow()),
            SLOT(slotUpdateMainMenu()));
    Q_ASSERT(check);
}

void CFrmRecentMsgList::slotDeleteFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pMenuAction);
    pMenu->disconnect(this);
}

void CFrmRecentMsgList::slotUpdateMainMenu()
{
    m_Menu.setTitle(tr("Operator recent message(&O)"));
    m_Menu.setIcon(QIcon(":/icon/Operate"));
    if(isHidden())
        m_Menu.setEnabled(false);
    else
        m_Menu.setEnabled(true);
}

void CFrmRecentMsgList::slotOpenMessage()
{
    QModelIndex index = m_MsgList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;
    QVariant v = m->data(index, USERLIST_ITEM_ROLE_JID);
    QString szId = v.value<QString>();
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId);
}

void CFrmRecentMsgList::slotRemoveMessage()
{
    QModelIndex index = m_MsgList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;
    QVariant v = m->data(index, USERLIST_ITEM_ROLE_JID);
    QString szId = v.value<QString>();
    slotRemove(szId);
}

void CFrmRecentMsgList::slotRemove(const QString &szId)
{
    RemoveItem(szId);
    QSharedPointer<CManageRecentMessage> recentMessage = CGlobal::Instance()->GetManager()->GetRecentMessage();
    recentMessage->Remove(szId);
}
