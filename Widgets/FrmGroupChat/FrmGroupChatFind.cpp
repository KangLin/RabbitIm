#include "FrmGroupChatFind.h"
#include "ui_FrmGroupChatFind.h"
#include "../../Global.h"
#include "qxmpp/QXmppDiscoveryManager.h"
#include "../FrmUserList/Roster.h"
#include <QMessageBox>
#include "../../MainWindow.h"
#include "FrmCreateGroupChatRoom.h"

CFrmGroupChatFind::CFrmGroupChatFind(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmGroupChatFind)
{
    ui->setupUi(this);

    //禁止文本框编辑  
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    InitMenu();

    m_pTreeConferenceModel = new QStandardItemModel(this);
    if(m_pTreeConferenceModel)
    {
        QItemSelectionModel *m = ui->treeView->selectionModel();
        ui->treeView->setModel(m_pTreeConferenceModel);
    }

    m_pListRoomModel = new QStandardItemModel(this);
    if(m_pListRoomModel)
    {
        ui->listView->setModel(m_pListRoomModel);
    }

    bool check = connect(&m_Conference, SIGNAL(sigFoundServer(QString,QString)), 
                         SLOT(slotFoundServer(QString,QString)));
    Q_ASSERT(check);

    check = connect(&m_Conference, SIGNAL(sigFoundRoom(QList<QXmppDiscoveryIq::Item>)),
                    SLOT(slotFoundRoom(QList<QXmppDiscoveryIq::Item>)));
    Q_ASSERT(check);

    check = connect(&m_Conference, SIGNAL(sigFoundRoomInfo(QString,QXmppDataForm)),
                    SLOT(slotFoundRoomInfo(QString,QXmppDataForm)));
    Q_ASSERT(check);
}

CFrmGroupChatFind::~CFrmGroupChatFind()
{
    delete ui;
    if(m_pTreeConferenceModel)
        delete m_pTreeConferenceModel;
    if(m_pListRoomModel)
        delete m_pListRoomModel;
}

CFrmGroupChatFind* CFrmGroupChatFind::Instance()
{
    static CFrmGroupChatFind* pFind = NULL;
    if(!pFind)
        pFind = new CFrmGroupChatFind();
    return pFind;
}

void CFrmGroupChatFind::showEvent(QShowEvent *)
{
    CGlobal::Instance()->GetMainWindow()->setEnabled(false);
    on_pbRefresh_clicked();
}

void CFrmGroupChatFind::closeEvent(QCloseEvent *)
{
    CGlobal::Instance()->GetMainWindow()->setEnabled(true);
}

void CFrmGroupChatFind::on_pbJoin_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::critical(this, tr("Join room error"),
                              tr("Don't select room, please select room."));
        return;
    }
    QVariant v = index.model()->data(index, ROLE_JID);
    QString szJid = v.value<QString>();

    CFrmGroupChat* pGroupChat = new CFrmGroupChat();
    if(pGroupChat)
    {
        bool check = connect(pGroupChat, SIGNAL(sigJoined(const QString&,CFrmGroupChat*)),
                             SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)));
        Q_ASSERT(check);

        if(!pGroupChat->Join(szJid))
            delete pGroupChat;
    }

    close();
}

void CFrmGroupChatFind::on_pbCancel_clicked()
{
    close();
}

void CFrmGroupChatFind::on_pbRefresh_clicked()
{
    m_pTreeConferenceModel->clear();
    m_Conference.Request(QString(), CConference::REQUEST_TYPE_SEVER);
}

void CFrmGroupChatFind::on_treeView_clicked(const QModelIndex &index)
{
#ifdef MOBILE
    on_treeView_doubleClicked(index);
#endif
}

void CFrmGroupChatFind::on_treeView_doubleClicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = index.model();
    if(!m)
        return;

    m_pListRoomModel->clear();
    QVariant v = m->data(index, ROLE_JID);
    QString jid = v.value<QString>();
    m_Conference.Request(jid, CConference::REQUEST_TYPE_ROOMS);
}

void CFrmGroupChatFind::slotFoundServer(const QString& jid, const QString& name)
{
     QStandardItem* pItem = new QStandardItem(QIcon(":/icon/Server"), name);
     pItem->setData(jid, ROLE_JID);
     pItem->setToolTip(jid);
     m_pTreeConferenceModel->appendRow(pItem);
}

void CFrmGroupChatFind::slotFoundRoom(const QList<QXmppDiscoveryIq::Item> &Rooms)
{
    QXmppDiscoveryIq::Item item;
    foreach(item, Rooms)
    {
        QStandardItem* pItem = new QStandardItem(QIcon(":/icon/Conference"), item.name());
        pItem->setData(item.jid(), ROLE_JID);
        pItem->setToolTip(item.jid());
        m_pListRoomModel->appendRow(pItem);
        m_Conference.Request(item.jid(), CConference::REQUEST_TYPE_ROOMS_INFO);
    }
}

//TODO:聊天室的信息显示  
void CFrmGroupChatFind::slotFoundRoomInfo(const QString &jid, const QXmppDataForm &form)
{
    LOG_MODEL_DEBUG("Group chat", "CFrmGroupChatFind::slotFoundRoomInfo");
}

void CFrmGroupChatFind::on_listView_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    m_Conference.Request();
    on_pbJoin_clicked();
}

void CFrmGroupChatFind::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QModelIndex index = ui->treeView->currentIndex();
    if(index.isValid())
    {
        m_Menu.exec(QCursor::pos());
    }
}

int CFrmGroupChatFind::InitMenu()
{
    m_Menu.addAction(QIcon(":/icon/Add"), tr("New room"), this, SLOT(slotActionNewRoom()));
    return 0;
}

void CFrmGroupChatFind::slotActionNewRoom()
{
    m_Conference.Request();
    QModelIndex index = ui->treeView->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::information(this, tr("Information"), tr("Please select group chat server"));
        return;
    }

    QVariant v = index.model()->data(index, ROLE_JID);
    QString szJid = v.value<QString>();
    CFrmCreateGroupChatRoom* pRoom = CFrmCreateGroupChatRoom::Instance(szJid);
    bool check = connect(pRoom, SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)),
                         SIGNAL(sigJoinedGroup(const QString&,CFrmGroupChat*)));
    Q_ASSERT(check);
    pRoom->show();
}

void CFrmGroupChatFind::on_pbNew_clicked()
{
    slotActionNewRoom();
}
