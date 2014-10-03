#include "FrmRecentMsgList.h"
#include "ui_FrmRecentMsgList.h"
#include "Global/Global.h"
#include "MainWindow.h"

#include <QSettings>

#ifdef WIN32
#undef GetMessage
#endif

CFrmRecentMsgList::CFrmRecentMsgList(QWidget *parent) :
    QFrame(parent),
    m_MsgList(this),
    ui(new Ui::CFrmRecentMsgList)
{
    ui->setupUi(this);
    m_pModel = new QStandardItemModel(this);//这里会产生内在泄漏，控件在romve操作时会自己释放内存。  
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

    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageUpdate(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigMessageClean(QString)),
                    SLOT(slotMessageUpdate(QString)));
    Q_ASSERT(check);

    check = connect(GET_CLIENT.data(), SIGNAL(sigChangedStatus(const QString&)),
                    SLOT(SlotChangedStatus(const QString&)));
    Q_ASSERT(check);

    LoadFromStorage();
}

CFrmRecentMsgList::~CFrmRecentMsgList()
{
    GET_CLIENT->disconnect(this);
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
        QSharedPointer<CUser> user = GLOBAL_USER->GetUserInfoRoster(szId);
        if(!user.isNull())
        {
            ItemInsertRoster(user, nRow++);
        }
        //是群消息  
        
        //是不在好友列表中的人发过来的消息  
        
    }
}

int CFrmRecentMsgList::ItemInsertRoster(QSharedPointer<CUser> roster, int nRow)
{
    int nRet = 0;
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster");
        return -1;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();
    //呢称条目  
    QStandardItem* pItem = new QStandardItem(info->GetShowName() 
                                             + info->GetSubscriptionTypeStr(info->GetSubScriptionType()));
    pItem->setData(info->GetId(), USERLIST_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ROSTER, USERLIST_ITEM_ROLE_PROPERTIES);
    //改变item背景颜色  
    pItem->setData(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus()), Qt::BackgroundRole);
    pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus())));
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
    QStandardItem* pMessageCountItem = new QStandardItem();
    int nCount = roster->GetMessage()->GetUnReadCount();
    if(nCount)
        pMessageCountItem->setText(QString::number(nCount));
    else
        pMessageCountItem->setText("");
    pMessageCountItem->setData(info->GetId(), USERLIST_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, USERLIST_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;

    m_pModel->insertRow(nRow, lstItems);
    return nRet;
}

void CFrmRecentMsgList::slotMessageUpdate(const QString &szId)
{
    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("CFrmRecentMsgList", "Dn't the roster:%s", qPrintable(szId));
        return;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();

    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                info->GetId(), 
                                                -1,
                                                Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    if(!lstIndexs.isEmpty())
    {
        QModelIndex index;
        foreach(index, lstIndexs)
        {
            LOG_MODEL_DEBUG("CFrmRecentMsgList", "index:row:%d;column:%d;id:%s", index.row(), index.column(), qPrintable(info->GetId()));
            QStandardItem* pItem = m_pModel->itemFromIndex(index);
            delete pItem;
            m_pModel->removeRow(index.row());
        }
    }

    //判断是否到达列表最大值  
    QSharedPointer<CManageRecentMessage> recentMessage = CGlobal::Instance()->GetManager()->GetRecentMessage();
    int nRowCount = m_pModel->rowCount();
    if(nRowCount >= recentMessage->GetMaxCount())
    {
        m_pModel->removeRow(nRowCount - 1);
    }

    if(roster->GetMessage()->GetUnReadCount() > 0)
        ItemInsertRoster(roster, 0);
    else
    {
        int row = recentMessage->GetUnreadCount();
        ItemInsertRoster(roster, row);
    }
}

void CFrmRecentMsgList::SlotChangedStatus(const QString &szId)
{
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                USERLIST_ITEM_ROLE_JID, 
                                                szId, 
                                                -1,
                                                Qt::MatchContains | Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive);
    if(lstIndexs.isEmpty())
    {
      return;
    }

    QSharedPointer<CUser> roster = GLOBAL_USER->GetUserInfoRoster(szId);
    if(roster.isNull())
    {
        LOG_MODEL_ERROR("FrmUserList", "Dn't the roster:%s", qPrintable(szId));
        return;
    }

    QSharedPointer<CUserInfo> info = roster->GetInfo();

    QModelIndex index;
    foreach(index, lstIndexs)
    {
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(!pItem) continue;
        if(pItem->data(USERLIST_ITEM_ROLE_PROPERTIES) == PROPERTIES_ROSTER)
        {
            //改变item背景颜色  
            pItem->setData(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus()), Qt::BackgroundRole);
            pItem->setBackground(QBrush(CGlobal::Instance()->GetRosterStatusColor(info->GetStatus())));
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
            pItem->setData(QIcon(CGlobal::Instance()->GetRosterStatusIcon(info->GetStatus())), Qt::DecorationRole);
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
        }
    }
    return;
}

void CFrmRecentMsgList::resizeEvent(QResizeEvent *e)
{
    LOG_MODEL_DEBUG("CFrmRecentMsgList", "CFrmUserList::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());
    m_MsgList.resize(this->geometry().size());
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
