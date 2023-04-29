#include "FrmGroupChatList.h"
#include "ui_FrmGroupChatList.h"
#include "../../Global/Global.h"
#include "FrmGroupChatFind.h"
#include "../../MainWindow.h"
#include "DlgCreateGroupChatRoom.h"
#include <QDesktopWidget>
#include "ManageGroupChat.h"
#include "Global/Global.h"
#include "DlgJoinGroupChat.h"
#include "DlgGroupChatInfo.h"
#include "DlgInviter.h"

#ifdef WIN32
#undef GetMessage
#endif

CFrmGroupChatList::CFrmGroupChatList(QWidget *parent) :
    QWidget(parent),
    m_GroupList(this),
    ui(new Ui::CFrmGroupChatList)
{
    ui->setupUi(this);
    ui->gridLayout->addWidget(&m_GroupList);

    QDesktopWidget *pDesk = QApplication::desktop();
    move((pDesk->width() - width()) / 2,
         (pDesk->height() - height()) / 2);

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
    //m_GroupList.setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_GroupList.setModel(m_pModel);
    m_GroupList.show();

    check = connect(&m_GroupList, SIGNAL(customContextMenuRequested(QPoint)),
                    SLOT(slotCustomContextMenuRequested(QPoint)));
    Q_ASSERT(check);

    check = connect(&m_GroupList, SIGNAL(clicked(QModelIndex)),
                    SLOT(slotClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(&m_GroupList, SIGNAL(doubleClicked(QModelIndex)),
                    SLOT(slotDoubleClicked(QModelIndex)));
    Q_ASSERT(check);

    check = connect(GETMANAGER->GetManageGroupChat().data(),
                     SIGNAL(sigJoined(QString)),
                     this, SLOT(slotJoinedGroup(QString)));
    Q_ASSERT(check);

    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigLeave(QString)),
                    this, SLOT(slotLeave(QString)));
    Q_ASSERT(check);

    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigUpdateMessage(QString)),
                    SLOT(slotUpdateMessage(QString)));
    Q_ASSERT(check);
    
    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigMessageClean(QString)),
                    SLOT(slotUpdateMessage(QString)));
    Q_ASSERT(check);
    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigInvitation(QString,QString,QString)),
                    SLOT(slotInvitation(QString,QString,QString)));
    Q_ASSERT(check);
    check = connect(GETMANAGER->GetManageGroupChat().data(),
                    SIGNAL(sigGroupChatInformationChange(QString)),
                    SLOT(slotUpdateMessage(QString)));
    Q_ASSERT(check);
}

CFrmGroupChatList::~CFrmGroupChatList()
{
    if(m_pModel)
        delete m_pModel;

    delete ui;
}

void CFrmGroupChatList::resizeEvent(QResizeEvent* e)
{
    Q_UNUSED(e);
    /*LOG_MODEL_DEBUG("CFrmGroupChatList", "CFrmGroupChatList::resizeEvent:e.size:%d;genmetry.size:%d",
                    e->size().width(),
                    geometry().size().width());*/
    //m_GroupList.resize(this->geometry().size());

    //调整列的宽度  
    int nWidth = m_GroupList.geometry().width() * 4/ 5;
    m_GroupList.setColumnWidth(0, nWidth);
    m_GroupList.setColumnWidth(1, m_GroupList.geometry().width() - nWidth - 5);
}

void CFrmGroupChatList::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

int CFrmGroupChatList::InitMenu()
{
    m_Menu.setTitle(tr("Operator group chat(&G)"));
    m_Menu.addAction(ui->actionOpen_chat_room);
    m_Menu.addAction(ui->actionCreate_chat_room);
    m_Menu.addAction(ui->actionJoin_chat_room);
    m_Menu.addAction(ui->actionLeave_room);
    m_Menu.addAction(ui->actionInviter);
    m_Menu.addAction(ui->actionRoom_infomation);
    //TODO:1、增加到菜单  
    
    bool check = connect(GET_MAINWINDOW, SIGNAL(sigMenuInitOperator(QMenu*)),
                    SLOT(slotAddToMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(CGlobal::Instance()->GetMainWindow(), SIGNAL(sigMenuRemoveOperator(QMenu*)),
                    SLOT(slotRemoveFromMainMenu(QMenu*)));
    Q_ASSERT(check);

    check = connect(&m_Menu, SIGNAL(aboutToShow()), SLOT(slotUpdateMenu()));
    Q_ASSERT(check);

    return 0;
}

void CFrmGroupChatList::slotAddToMainMenu(QMenu *pMenu)
{
    m_pAction = pMenu->addMenu(&m_Menu);
    bool check = connect(pMenu, SIGNAL(aboutToShow()),
                    SLOT(slotUpdateMenu()));//slotUpdateMainMenu
    Q_ASSERT(check);
}

void CFrmGroupChatList::slotRemoveFromMainMenu(QMenu *pMenu)
{
    pMenu->removeAction(m_pAction);
    pMenu->disconnect(this);
}

void CFrmGroupChatList::slotUpdateMenu()
{
    if(this->isHidden())
    {
        m_Menu.setEnabled(false);
        return;
    }

    m_Menu.setEnabled(true);

    ui->actionCreate_chat_room->setVisible(true);
    ui->actionJoin_chat_room->setVisible(true);

    QString room = GetCurrentRoom();
    if(room.isEmpty())//列表是空  
    {
        ui->actionLeave_room->setVisible(false);
        ui->actionOpen_chat_room->setVisible(false);
        ui->actionRoom_infomation->setVisible(false);
        ui->actionInviter->setVisible(false);
        return;
    }

    //TODO:2、设置是否可以显示  
    ui->actionLeave_room->setVisible(true);
    ui->actionOpen_chat_room->setVisible(true);
    ui->actionRoom_infomation->setVisible(true);
    ui->actionInviter->setVisible(true);
}

void CFrmGroupChatList::slotCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    m_Menu.exec(QCursor::pos());
}

void CFrmGroupChatList::on_actionCreate_chat_room_triggered()
{
    CDlgCreateGroupChatRoom room;
    room.exec();
}

void CFrmGroupChatList::on_actionJoin_chat_room_triggered()
{
    CDlgJoinGroupChat dlg(this);
    dlg.exec();
}

void CFrmGroupChatList::on_actionOpen_chat_room_triggered()
{
    QString szId = GetCurrentRoom();
    if(szId.isEmpty())
        return;
    //是用户结点，打开消息对话框  
    MANAGE_MESSAGE_DIALOG->ShowDialog(szId);
}

void CFrmGroupChatList::on_actionLeave_room_triggered()
{
    QString szId = GetCurrentRoom();
    if(szId.isEmpty())
        return;
    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    if(gc.isNull())
        return;
    gc->Leave();
    ItemRemove(szId);
}

void CFrmGroupChatList::on_actionRoom_infomation_triggered()
{
    QString szId = GetCurrentRoom();
    if(szId.isEmpty())
        return;
    CDlgGroupChatInfo info(szId, this);
    info.exec();
}

void CFrmGroupChatList::on_actionInviter_triggered()
{
    QString szId = GetCurrentRoom();
    if(szId.isEmpty())
        return;
    CDlgInviter dlg(szId, this);
    dlg.exec();
}

void CFrmGroupChatList::slotClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
#ifdef MOBILE
    this->on_actionOpen_chat_room_triggered();
#endif
}

void CFrmGroupChatList::slotDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
#ifndef ANDROID
    this->on_actionOpen_chat_room_triggered();
#endif
}

QString CFrmGroupChatList::GetCurrentRoom()
{
    QModelIndex index = m_GroupList.currentIndex();
    const QAbstractItemModel *m = index.model();
    if(!m)//列表是空  
    {
       return QString();
    }

    //是用户结点
    QVariant v = m->data(index, GROUP_ITEM_ROLE_JID);
    if(v.canConvert<QString>())
    {
        return v.value<QString>();
    }
    return QString();
}

int CFrmGroupChatList::ItemRemove(const QString &szId)
{
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                GROUP_ITEM_ROLE_JID, 
                                                szId, 
                                                1,
                                                Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchCaseSensitive);
    QModelIndex index;
    foreach(index, lstIndexs)
    {
        m_pModel->removeRow(index.row());
    }
    return 0;
}

void CFrmGroupChatList::slotJoinedGroup(const QString &szId)
{
    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                GROUP_ITEM_ROLE_JID, 
                                                szId, 
                                                1,
                                                Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchCaseSensitive);
    if(!lstIndexs.isEmpty())
        return;

    QSharedPointer<CManageGroupChat> mgc = GETMANAGER->GetManageGroupChat();
    if(mgc.isNull())
    {
        qCritical("GETMANAGER->GetManageGroupChat() is null");
        return;
    }
    QSharedPointer<CGroupChat> gc = mgc->Get(szId);
    //呢称条目  
    QStandardItem* pItem = new QStandardItem(gc->ShowName());
    pItem->setData(gc->Id(), GROUP_ITEM_ROLE_JID);
    pItem->setData(PROPERTIES_ITEM, GROUP_ITEM_ROLE_PROPERTIES);
    //设置item图标  
    pItem->setData(gc->Icon(), Qt::DecorationRole);
#ifdef DEBUG
    pItem->setToolTip(gc->Id());
#endif 

    //消息条目  
    QStandardItem* pMessageCountItem = new QStandardItem("");
    pMessageCountItem->setData(gc->Id(), GROUP_ITEM_ROLE_JID);
    pMessageCountItem->setData(PROPERTIES_UNREAD_MESSAGE_COUNT, GROUP_ITEM_ROLE_PROPERTIES);
    pMessageCountItem->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
    pMessageCountItem->setEditable(false);//禁止双击编辑  

    QList<QStandardItem *> lstItems;
    lstItems << pItem << pMessageCountItem;
    m_pModel->appendRow(lstItems);
}

void CFrmGroupChatList::slotLeave(const QString &szId)
{
    ItemRemove(szId);
}

void CFrmGroupChatList::slotUpdateMessage(const QString &szId)
{
    QSharedPointer<CGroupChat> gc = GETMANAGER->GetManageGroupChat()->Get(szId);
    if(gc.isNull())
        return;

    QModelIndexList lstIndexs = m_pModel->match(m_pModel->index(0, 0),
                                                GROUP_ITEM_ROLE_JID, 
                                                szId, 
                                                1,
                                                Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchCaseSensitive);
    QModelIndex index;
    foreach(index, lstIndexs)
    {
        qDebug("index:row:%d;column:%d;id:%s", index.row(), index.column(), qPrintable(szId));
        QStandardItem* pItem = m_pModel->itemFromIndex(index);
        if(NULL == pItem) continue;
        if(pItem->data(GROUP_ITEM_ROLE_PROPERTIES) == PROPERTIES_ITEM)
        {
            //呢称条目  
            pItem->setText(gc->ShowName());
            //设置item图标  
            pItem->setData(gc->Icon(), Qt::DecorationRole);
#ifdef DEBUG
            pItem->setToolTip(gc->Id());
#endif 
        }
        pItem = m_pModel->itemFromIndex(index.sibling(index.row(), index.column() + 1));
        if(NULL == pItem) continue;
        if(pItem->data(GROUP_ITEM_ROLE_PROPERTIES) == PROPERTIES_UNREAD_MESSAGE_COUNT)
        {
            //设置未读消息数  
            int nCount = gc->GetMessage()->GetUnReadCount();
            if(nCount)
                pItem->setText(QString::number(nCount));
            else
                pItem->setText(QString(""));
            pItem->setData(CGlobal::Instance()->GetUnreadMessageCountColor(), Qt::TextColorRole);
        }
    }
}

void CFrmGroupChatList::slotInvitation(const QString &szGroupChatId, const QString &szInviter, const QString &szReason)
{
    CDlgJoinGroupChat dlg(szGroupChatId, szInviter, szReason, this);
    dlg.exec();
}
